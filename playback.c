#include <gst/gst.h>
#include <string.h>
#include <math.h>

#include "Player.h"

#define VOLUME_STEPS 20

GST_DEBUG_CATEGORY (play_debug);
#define GST_CAT_DEFAULT play_debug

typedef struct {
    gchar **uris;
    guint num_uris;
    gint cur_idx;

    Player *player;
    GstState desired_state;

    gboolean repeat;

    GMainLoop *loop;
} Playback;

static gboolean play_next(Playback *playback);

static gboolean play_prev(Playback *playback);

static void playback_reset(Playback *playback);

static void playback_set_relative_volume(Playback *playback, gdouble volume_step);

static void end_of_stream_cb(Player *player, Playback *playback) {
    g_print("\n");
    /* and switch to next item in list */
    if (!play_next(playback)) {
        g_print("Reached end of play list.\n");
        g_main_loop_quit(playback->loop);
    }
}

static void error_cb(Player *player, GError *err, Playback *playback) {
    g_printerr("ERROR %s for %s\n", err->message, playback->uris[playback->cur_idx]);

    /* if looping is enabled, then disable it else will keep looping forever */
    playback->repeat = FALSE;

    /* try next item in list then */
    if (!play_next(playback)) {
        g_print("Reached end of play list.\n");
        g_main_loop_quit(playback->loop);
    }
}

static void position_updated_cb(Player *player, GstClockTime pos, Playback *play) {
    GstClockTime dur = -1;
    gchar status[64] = {0,};

    g_object_get(play->player, "duration", &dur, NULL);

    memset (status, ' ', sizeof(status) - 1);

    if (pos != -1 && dur > 0 && dur != -1) {
        gchar dstr[32], pstr[32];

        /* FIXME: pretty print in nicer format */
        g_snprintf(pstr, 32, "%" GST_TIME_FORMAT, GST_TIME_ARGS (pos));
        pstr[9] = '\0';
        g_snprintf(dstr, 32, "%" GST_TIME_FORMAT, GST_TIME_ARGS (dur));
        dstr[9] = '\0';
        g_print("%s / %s %s\n", pstr, dstr, status);
    }
}

static void state_changed_cb(Player *player, PlayerState state, Playback *playback) {
    g_print("State changed: %s\n", player_state_get_name(state));
}

static void buffering_cb(Player *player, gint percent, Playback *playback) {
    g_print("Buffering: %d\n", percent);
}

static void print_one_tag(const GstTagList *list, const gchar *tag, gpointer user_data) {
    gint i, num;

    num = gst_tag_list_get_tag_size(list, tag);
    for (i = 0; i < num; ++i) {
        const GValue *val;

        val = gst_tag_list_get_value_index(list, tag, i);
        if (G_VALUE_HOLDS_STRING (val)) {
            g_print("    %s : %s \n", tag, g_value_get_string(val));
        } else if (G_VALUE_HOLDS_UINT (val)) {
            g_print("    %s : %u \n", tag, g_value_get_uint(val));
        } else if (G_VALUE_HOLDS_DOUBLE (val)) {
            g_print("    %s : %g \n", tag, g_value_get_double(val));
        } else if (G_VALUE_HOLDS_BOOLEAN (val)) {
            g_print("    %s : %s \n", tag,
                    g_value_get_boolean(val) ? "true" : "false");
        } else if (GST_VALUE_HOLDS_DATE_TIME (val)) {
            GstDateTime *dt = g_value_get_boxed(val);
            gchar *dt_str = gst_date_time_to_iso8601_string(dt);

            g_print("    %s : %s \n", tag, dt_str);
            g_free(dt_str);
        } else {
            g_print("    %s : tag of type '%s' \n", tag, G_VALUE_TYPE_NAME (val));
        }
    }
}

static void
print_audio_info(PlayerAudioInfo *info) {
    if (info == NULL)
        return;

    g_print("  sample rate : %d\n",
            player_audio_info_get_sample_rate(info));
    g_print("  channels : %d\n", player_audio_info_get_channels(info));
    g_print("  max_bitrate : %d\n",
            player_audio_info_get_max_bitrate(info));
    g_print("  bitrate : %d\n", player_audio_info_get_bitrate(info));
    g_print("  language : %s\n", player_audio_info_get_language(info));
}

static void
print_all_stream_info(PlayerMediaInfo *media_info) {
    guint count = 0;
    GList *list, *l;

    g_print("URI : %s\n", player_media_info_get_uri(media_info));
    g_print("Duration: %" GST_TIME_FORMAT "\n",
            GST_TIME_ARGS (player_media_info_get_duration(media_info)));

    list = player_media_info_get_stream_list(media_info);
    if (!list)
        return;

    g_print("All Stream information\n");
    for (l = list; l != NULL; l = l->next) {
        GstTagList *tags = NULL;
        PlayerStreamInfo *stream = (PlayerStreamInfo *) l->data;

        g_print(" Stream # %u \n", count++);
        g_print("  type : %s_%u\n",
                player_stream_info_get_stream_type(stream),
                player_stream_info_get_index(stream));
        tags = player_stream_info_get_tags(stream);
        g_print("  taglist : \n");
        if (tags) {
            gst_tag_list_foreach(tags, print_one_tag, NULL);
        }

        if (GST_IS_PLAYER_AUDIO_INFO (stream))
            print_audio_info((PlayerAudioInfo *) stream);
    }
}

static void
print_all_audio_stream(PlayerMediaInfo *media_info) {
    GList *list, *l;

    list = player_media_info_get_audio_streams(media_info);
    if (!list)
        return;

    g_print("All audio streams: \n");
    for (l = list; l != NULL; l = l->next) {
        PlayerAudioInfo *audio_info = (PlayerAudioInfo *) l->data;
        PlayerStreamInfo *stream_info = (PlayerStreamInfo *) audio_info;
        g_print(" %s_%d #\n", player_stream_info_get_stream_type(stream_info),
                player_stream_info_get_index(stream_info));
        print_audio_info(audio_info);
    }
}

static void
print_current_tracks(Playback *playback) {
    PlayerAudioInfo *audio = NULL;

    g_print("Current audio track: \n");
    audio = player_get_current_audio_track(playback->player);
    print_audio_info(audio);

    if (audio)
        g_object_unref(audio);
}

static void
print_media_info(PlayerMediaInfo *media_info) {
    print_all_stream_info(media_info);
    g_print("\n");
    print_all_audio_stream(media_info);
    g_print("\n");
}

static void media_info_cb(Player *player, PlayerMediaInfo *info, Playback *playback) {
    static int once = 0;

    if (!once) {
        print_media_info(info);
        print_current_tracks(playback);
        once = 1;
    }
}

static Playback *playback_new(gchar **uris, gdouble initial_volume) {
    Playback *playback;

    playback = g_new0 (Playback, 1);

    playback->uris = uris;
    playback->num_uris = g_strv_length(uris);
    playback->cur_idx = -1;

    playback->player =
            player_new(player_main_context_signal_dispatcher_new
                               (NULL));
    player_set_video_track_enabled(playback->player, FALSE);
    player_set_subtitle_track_enabled(playback->player, FALSE);

    g_signal_connect (playback->player, "position-updated",
                      G_CALLBACK(position_updated_cb), playback);
    g_signal_connect (playback->player, "state-changed",
                      G_CALLBACK(state_changed_cb), playback);
    g_signal_connect (playback->player, "buffering", G_CALLBACK(buffering_cb), playback);
    g_signal_connect (playback->player, "end-of-stream",
                      G_CALLBACK(end_of_stream_cb), playback);
    g_signal_connect (playback->player, "error", G_CALLBACK(error_cb), playback);

    g_signal_connect (playback->player, "media-info-updated",
                      G_CALLBACK(media_info_cb), playback);

    playback->loop = g_main_loop_new(NULL, FALSE);
    playback->desired_state = GST_STATE_PLAYING;

    playback_set_relative_volume(playback, initial_volume - 1.0);

    return playback;
}

static void playback_free(Playback *play) {
    playback_reset(play);

    gst_object_unref(play->player);

    g_main_loop_unref(play->loop);

    g_strfreev(play->uris);
    g_free(play);
}

/* reset for new file/stream */
static void playback_reset(Playback *playback) {

}

static void playback_set_relative_volume(Playback *playback, gdouble volume_step) {
    gdouble volume;

    g_object_get(playback->player, "volume", &volume, NULL);
    volume = round((volume + volume_step) * VOLUME_STEPS) / VOLUME_STEPS;
    volume = CLAMP (volume, 0.0, 10.0);

    g_object_set(playback->player, "volume", volume, NULL);

    g_print("Volume: %.0f%%                  \n", volume * 100);
}

static gchar *playback_uri_get_display_name(Playback *playback, const gchar *uri) {
    gchar *loc;

    if (gst_uri_has_protocol(uri, "file")) {
        loc = g_filename_from_uri(uri, NULL, NULL);
    } else if (gst_uri_has_protocol(uri, "pushfile")) {
        loc = g_filename_from_uri(uri + 4, NULL, NULL);
    } else {
        loc = g_strdup(uri);
    }

    /* Maybe additionally use glib's filename to display name function */
    return loc;
}

static void
play_uri(Playback *playback, const gchar *next_uri) {
    gchar *loc;

    playback_reset(playback);

    loc = playback_uri_get_display_name(playback, next_uri);
    g_print("Now playing %s\n", loc);
    g_free(loc);

    g_object_set(playback->player, "uri", next_uri, NULL);
    player_play(playback->player);
}

/* returns FALSE if we have reached the end of the playlist */
static gboolean
play_next(Playback *playback) {
    if ((playback->cur_idx + 1) >= playback->num_uris) {
        if (playback->repeat) {
            g_print("Looping playlist \n");
            playback->cur_idx = -1;
        } else
            return FALSE;
    }

    play_uri(playback, playback->uris[++playback->cur_idx]);
    return TRUE;
}

/* returns FALSE if we have reached the beginning of the playlist */
static gboolean
play_prev(Playback *playback) {
    if (playback->cur_idx == 0 || playback->num_uris <= 1)
        return FALSE;

    play_uri(playback, playback->uris[--playback->cur_idx]);
    return TRUE;
}

static void
do_play(Playback *playback) {
    gint i;

    /* dump playlist */
    for (i = 0; i < playback->num_uris; ++i)
        GST_INFO ("%4u : %s", i, playback->uris[i]);

    if (!play_next(playback))
        return;

    g_main_loop_run(playback->loop);
}

static void add_to_playlist(GPtrArray *playlist, const gchar *filename) {
    GDir *dir;
    gchar *uri;

    if (gst_uri_is_valid(filename)) {
        g_ptr_array_add(playlist, g_strdup(filename));
        return;
    }

    if ((dir = g_dir_open(filename, 0, NULL))) {
        const gchar *entry;

        /* FIXME: sort entries for each directory? */
        while ((entry = g_dir_read_name(dir))) {
            gchar *path;

            path = g_strconcat(filename, G_DIR_SEPARATOR_S, entry, NULL);
            add_to_playlist(playlist, path);
            g_free(path);
        }

        g_dir_close(dir);
        return;
    }

    uri = gst_filename_to_uri(filename, NULL);
    if (uri != NULL)
        g_ptr_array_add(playlist, uri);
    else
        g_warning ("Could not make URI out of filename '%s'", filename);
}

static void
shuffle_uris(gchar **uris, guint num) {
    gchar *tmp;
    guint i, j;

    if (num < 2)
        return;

    for (i = 0; i < num; i++) {
        /* gets equally distributed random number in 0..num-1 [0;num[ */
        j = g_random_int_range(0, num);
        tmp = uris[j];
        uris[j] = uris[i];
        uris[i] = tmp;
    }
}

static void
toggle_paused(Playback *playback) {
    if (playback->desired_state == GST_STATE_PLAYING) {
        playback->desired_state = GST_STATE_PAUSED;
        player_pause(playback->player);
    } else {
        playback->desired_state = GST_STATE_PLAYING;
        player_play(playback->player);
    }
}

static void
relative_seek(Playback *playback, gdouble percent) {
    gint64 dur = -1, pos = -1;

    g_return_if_fail (percent >= -1.0 && percent <= 1.0);

    g_object_get(playback->player, "position", &pos, "duration", &dur, NULL);

    if (dur <= 0) {
        g_print("\nCould not seek.\n");
        return;
    }

    pos = pos + dur * percent;
    if (pos < 0)
        pos = 0;
    player_seek(playback->player, pos);
}

static void
keyboard_cb(const gchar *key_input, gpointer user_data) {
    Playback *play = (Playback *) user_data;

    switch (g_ascii_tolower(key_input[0])) {
        case 'i': {
            PlayerMediaInfo *media_info = player_get_media_info(play->player);
            if (media_info) {
                print_media_info(media_info);
                g_object_unref(media_info);
                print_current_tracks(play);
            }
            break;
        }
        case ' ':
            toggle_paused(play);
            break;
        case 'q':
        case 'Q':
            g_main_loop_quit(play->loop);
            break;
        case '>':
            if (!play_next(play)) {
                g_print("\nReached end of play list.\n");
                g_main_loop_quit(play->loop);
            }
            break;
        case '<':
            play_prev(play);
            break;
        case 27:                   /* ESC */
            if (key_input[1] == '\0') {
                g_main_loop_quit(play->loop);
                break;
            }
            /* fall through */
        default:
            if (strcmp(key_input, "right") == 0) {
                relative_seek(play, +0.08);
            } else if (strcmp(key_input, "left") == 0) {
                relative_seek(play, -0.01);
            } else if (strcmp(key_input, "up") == 0) {
                playback_set_relative_volume(play, +1.0 / VOLUME_STEPS);
            } else if (strcmp(key_input, "down") == 0) {
                playback_set_relative_volume(play, -1.0 / VOLUME_STEPS);
            } else {
                GST_INFO ("keyboard input:");
                for (; *key_input != '\0'; ++key_input)
                    GST_INFO ("  code %3d", *key_input);
            }
            break;
    }
}

int
main(int argc, char **argv) {
    Playback *playback;
    GPtrArray *playlist;
    gboolean print_version = FALSE;
    gboolean interactive = TRUE; /* FIXME: maybe enable by default? */
    gboolean shuffle = FALSE;
    gboolean repeat = FALSE;
    gdouble volume = 1.0;
    gchar **filenames = NULL;
    gchar **uris;
    guint num, i;
    GError *err = NULL;
    GOptionContext *ctx;
    gchar *playlist_file = NULL;
    GOptionEntry options[] = {
            {"version",          0, 0, G_OPTION_ARG_NONE,           &print_version,
                                                                             "Print version information and exit",         NULL},
            {"shuffle",          0, 0, G_OPTION_ARG_NONE,           &shuffle,
                                                                             "Shuffle playlist",                           NULL},
            {"interactive",      0, 0, G_OPTION_ARG_NONE,           &interactive,
                                                                             "Interactive control via keyboard",           NULL},
            {"volume",           0, 0, G_OPTION_ARG_DOUBLE,         &volume,
                                                                             "Volume",                                     NULL},
            {"playlist",         0, 0, G_OPTION_ARG_FILENAME,       &playlist_file,
                                                                             "Playlist file containing input media files", NULL},
            {"loop",             0, 0, G_OPTION_ARG_NONE,           &repeat, "Repeat all",                                 NULL},
            {G_OPTION_REMAINING, 0, 0, G_OPTION_ARG_FILENAME_ARRAY, &filenames, NULL},
            {NULL}
    };

    g_set_prgname("gst-play");

    ctx = g_option_context_new("FILE1|URI1 [FILE2|URI2] [FILE3|URI3] ...");
    g_option_context_add_main_entries(ctx, options, NULL);
    g_option_context_add_group(ctx, gst_init_get_option_group());
    if (!g_option_context_parse(ctx, &argc, &argv, &err)) {
        g_print("Error initializing: %s\n", GST_STR_NULL (err->message));
        g_clear_error(&err);
        g_option_context_free(ctx);
        return 1;
    }
    g_option_context_free(ctx);

    GST_DEBUG_CATEGORY_INIT (play_debug, "play", 0, "gst-play");

    if (print_version) {
        gchar *version_str;

        version_str = gst_version_string();
        g_print("%s version %s\n", g_get_prgname(), "1.0");
        g_print("%s\n", version_str);
        g_free(version_str);

        g_free(playlist_file);

        return 0;
    }

    playlist = g_ptr_array_new();

    if (playlist_file != NULL) {
        gchar *playlist_contents = NULL;
        gchar **lines = NULL;

        if (g_file_get_contents(playlist_file, &playlist_contents, NULL, &err)) {
            lines = g_strsplit(playlist_contents, "\n", 0);
            num = g_strv_length(lines);

            for (i = 0; i < num; i++) {
                if (lines[i][0] != '\0') {
                    GST_LOG ("Playlist[%d]: %s", i + 1, lines[i]);
                    add_to_playlist(playlist, lines[i]);
                }
            }
            g_strfreev(lines);
            g_free(playlist_contents);
        } else {
            g_printerr("Could not read playlist: %s\n", err->message);
            g_clear_error(&err);
        }
        g_free(playlist_file);
        playlist_file = NULL;
    }

    if (playlist->len == 0 && (filenames == NULL || *filenames == NULL)) {
        g_printerr("Usage: %s FILE1|URI1 [FILE2|URI2] [FILE3|URI3] ...",
                   "gst-play");
        g_printerr("\n\n"),
                g_printerr("%s\n\n",
                           "You must provide at least one filename or URI to play.");
        /* No input provided. Free array */
        g_ptr_array_free(playlist, TRUE);

        return 1;
    }

    /* fill playlist */
    if (filenames != NULL && *filenames != NULL) {
        num = g_strv_length(filenames);
        for (i = 0; i < num; ++i) {
            GST_LOG ("command line argument: %s", filenames[i]);
            add_to_playlist(playlist, filenames[i]);
        }
        g_strfreev(filenames);
    }

    num = playlist->len;
    g_ptr_array_add(playlist, NULL);

    uris = (gchar **) g_ptr_array_free(playlist, FALSE);

    if (shuffle)
        shuffle_uris(uris, num);

    /* prepare */
    playback = playback_new(uris, volume);
    playback->repeat = repeat;

    /* play */
    do_play(playback);

    /* clean up */
    playback_free(playback);

    g_print("\n");
    gst_deinit();
    return 0;
}
