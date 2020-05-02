#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "MediaInfo.h"
#include "MediaInfoPrivate.h"

/* Per-stream information */
G_DEFINE_ABSTRACT_TYPE (PlayerStreamInfo, player_stream_info,
                        G_TYPE_OBJECT);

static void player_stream_info_init(PlayerStreamInfo *sinfo) {
    sinfo->stream_index = -1;
}

static void player_stream_info_finalize(GObject *object) {
    PlayerStreamInfo *stream_info = GST_PLAYER_STREAM_INFO (object);

    //g_free (stream_info->codec);
    g_free(stream_info->stream_id);

    if (stream_info->caps)
        gst_caps_unref(stream_info->caps);

    if (stream_info->tags)
        gst_tag_list_unref(stream_info->tags);

    G_OBJECT_CLASS (player_stream_info_parent_class)->finalize(object);
}

static void player_stream_info_class_init(PlayerStreamInfoClass *klass) {
    GObjectClass *gobject_class = (GObjectClass *) klass;

    gobject_class->finalize = player_stream_info_finalize;
}

/**
 * player_stream_info_get_index:
 * @info: a #PlayerStreamInfo
 *
 * Function to get stream index from #PlayerStreamInfo instance.
 *
 * Returns: the stream index of this stream.
 */
gint player_stream_info_get_index(const PlayerStreamInfo *info) {
    g_return_val_if_fail (GST_IS_PLAYER_STREAM_INFO(info), -1);

    return info->stream_index;
}

/**
 * player_stream_info_get_stream_type:
 * @info: a #PlayerStreamInfo
 *
 * Function to return human readable name for the stream type
 * of the given @info (ex: "audio", "video", "subtitle")
 *
 * Returns: a human readable name
 */
const gchar *player_stream_info_get_stream_type(const PlayerStreamInfo *info) {
    g_return_val_if_fail (GST_IS_PLAYER_STREAM_INFO(info), NULL);

    if (GST_IS_PLAYER_AUDIO_INFO (info))
        return "audio";


    return "unknown";
}

/**
 * player_stream_info_get_tags:
 * @info: a #PlayerStreamInfo
 *
 * Returns: (transfer none): the tags contained in this stream.
 */
GstTagList *player_stream_info_get_tags(const PlayerStreamInfo *info) {
    g_return_val_if_fail (GST_IS_PLAYER_STREAM_INFO(info), NULL);

    return info->tags;
}

/**
 * player_stream_info_get_codec:
 * @info: a #PlayerStreamInfo
 *
 * A string describing codec used in #PlayerStreamInfo.
 *
 * Returns: codec string or NULL on unknown.
 */
const gchar *player_stream_info_get_codec(const PlayerStreamInfo *info) {
    g_return_val_if_fail (GST_IS_PLAYER_STREAM_INFO(info), NULL);

    return NULL;//info->codec;
}

/**
 * player_stream_info_get_caps:
 * @info: a #PlayerStreamInfo
 *
 * Returns: (transfer none): the #GstCaps of the stream.
 */
GstCaps *player_stream_info_get_caps(const PlayerStreamInfo *info) {
    g_return_val_if_fail (GST_IS_PLAYER_STREAM_INFO(info), NULL);

    return info->caps;
}


/* Audio information */
G_DEFINE_TYPE (PlayerAudioInfo, player_audio_info,
               GST_TYPE_PLAYER_STREAM_INFO);

static void player_audio_info_init(PlayerAudioInfo *info) {
    info->channels = 0;
    info->sample_rate = 0;
    info->bitrate = -1;
    info->max_bitrate = -1;
}

static void player_audio_info_finalize(GObject *object) {
    PlayerAudioInfo *info = GST_PLAYER_AUDIO_INFO (object);

    g_free(info->language);

    G_OBJECT_CLASS (player_audio_info_parent_class)->finalize(object);
}

static void player_audio_info_class_init(PlayerAudioInfoClass *klass) {
    GObjectClass *gobject_class = (GObjectClass *) klass;

    gobject_class->finalize = player_audio_info_finalize;
}

/**
 * player_audio_info_get_language:
 * @info: a #PlayerAudioInfo
 *
 * Returns: the language of the stream, or NULL if unknown.
 */
const gchar *player_audio_info_get_language(const PlayerAudioInfo *info) {
    g_return_val_if_fail (GST_IS_PLAYER_AUDIO_INFO(info), NULL);

    return info->language;
}

/**
 * player_audio_info_get_channels:
 * @info: a #PlayerAudioInfo
 *
 * Returns: the number of audio channels in #PlayerAudioInfo.
 */
gint player_audio_info_get_channels(const PlayerAudioInfo *info) {
    g_return_val_if_fail (GST_IS_PLAYER_AUDIO_INFO(info), 0);

    return info->channels;
}

/**
 * player_audio_info_get_sample_rate:
 * @info: a #PlayerAudioInfo
 *
 * Returns: the audio sample rate in #PlayerAudioInfo.
 */
gint player_audio_info_get_sample_rate(const PlayerAudioInfo *info) {
    g_return_val_if_fail (GST_IS_PLAYER_AUDIO_INFO(info), 0);

    return info->sample_rate;
}

/**
 * player_audio_info_get_bitrate:
 * @info: a #PlayerAudioInfo
 *
 * Returns: the audio bitrate in #PlayerAudioInfo.
 */
gint player_audio_info_get_bitrate(const PlayerAudioInfo *info) {
    g_return_val_if_fail (GST_IS_PLAYER_AUDIO_INFO(info), -1);

    return info->bitrate;
}

/**
 * player_audio_info_get_max_bitrate:
 * @info: a #PlayerAudioInfo
 *
 * Returns: the audio maximum bitrate in #PlayerAudioInfo.
 */
gint player_audio_info_get_max_bitrate(const PlayerAudioInfo *info) {
    g_return_val_if_fail (GST_IS_PLAYER_AUDIO_INFO(info), -1);

    return info->max_bitrate;
}

/* Global media information */
G_DEFINE_TYPE (PlayerMediaInfo, player_media_info, G_TYPE_OBJECT);

static void player_media_info_init(PlayerMediaInfo *info) {
    info->duration = -1;
    info->is_live = FALSE;
    info->seekable = FALSE;
}

static void player_media_info_finalize(GObject *object) {
    PlayerMediaInfo *info = GST_PLAYER_MEDIA_INFO (object);

    g_free(info->uri);

    g_free(info->title);

    g_free(info->container);

    if (info->audio_stream_list)
        g_list_free(info->audio_stream_list);

    if (info->stream_list)
        g_list_free_full(info->stream_list, g_object_unref);

    G_OBJECT_CLASS (player_media_info_parent_class)->finalize(object);
}

static void player_media_info_class_init(PlayerMediaInfoClass *klass) {
    GObjectClass *oclass = (GObjectClass *) klass;

    oclass->finalize = player_media_info_finalize;
}

static PlayerAudioInfo *player_audio_info_new(void) {
    return g_object_new(GST_TYPE_PLAYER_AUDIO_INFO, NULL);
}

static PlayerStreamInfo *player_audio_info_copy(PlayerAudioInfo *ref) {
    PlayerAudioInfo *ret;

    ret = player_audio_info_new();

    ret->sample_rate = ref->sample_rate;
    ret->channels = ref->channels;
    ret->bitrate = ref->bitrate;
    ret->max_bitrate = ref->max_bitrate;

    if (ref->language)
        ret->language = g_strdup(ref->language);

    return (PlayerStreamInfo *) ret;
}

PlayerStreamInfo *player_stream_info_copy(PlayerStreamInfo *ref) {
    PlayerStreamInfo *info = NULL;

    if (!ref)
        return NULL;

    if (GST_IS_PLAYER_AUDIO_INFO (ref)) {
        info = player_audio_info_copy((PlayerAudioInfo *) ref);
    }

    if (info) {
        info->stream_index = ref->stream_index;
    }
    if (ref->tags)
        info->tags = gst_tag_list_ref(ref->tags);
    if (ref->caps)
        info->caps = gst_caps_copy (ref->caps);
//  if (ref->codec)
//    info->codec = g_strdup (ref->codec);
    if (ref->stream_id)
        info->stream_id = g_strdup(ref->stream_id);

    return info;
}

PlayerMediaInfo *player_media_info_copy(PlayerMediaInfo *ref) {
    GList *l;
    PlayerMediaInfo *info;

    if (!ref)
        return NULL;

    info = player_media_info_new(ref->uri);
    info->duration = ref->duration;
    info->seekable = ref->seekable;
    info->is_live = ref->is_live;
    if (ref->title)
        info->title = g_strdup(ref->title);
    if (ref->container)
        info->container = g_strdup(ref->container);

    for (l = ref->stream_list; l != NULL; l = l->next) {
        PlayerStreamInfo *s;

        s = player_stream_info_copy((PlayerStreamInfo *) l->data);
        info->stream_list = g_list_append(info->stream_list, s);

        if (GST_IS_PLAYER_AUDIO_INFO (s)){
            info->audio_stream_list = g_list_append(info->audio_stream_list, s);
        }
    }

    return info;
}

PlayerStreamInfo *player_stream_info_new(gint stream_index, GType type) {
    PlayerStreamInfo *info = NULL;

    if (type == GST_TYPE_PLAYER_AUDIO_INFO) {
        info = (PlayerStreamInfo *) player_audio_info_new();
    }

    info->stream_index = stream_index;

    return info;
}

PlayerMediaInfo *player_media_info_new(const gchar *uri) {
    PlayerMediaInfo *info;

    g_return_val_if_fail (uri != NULL, NULL);

    info = g_object_new(GST_TYPE_PLAYER_MEDIA_INFO, NULL);
    info->uri = g_strdup(uri);

    return info;
}

/**
 * player_media_info_get_uri:
 * @info: a #PlayerMediaInfo
 *
 * Returns: the URI associated with #PlayerMediaInfo.
 */
const gchar *player_media_info_get_uri(const PlayerMediaInfo *info) {
    g_return_val_if_fail (GST_IS_PLAYER_MEDIA_INFO(info), NULL);

    return info->uri;
}

/**
 * player_media_info_is_seekable:
 * @info: a #PlayerMediaInfo
 *
 * Returns: %TRUE if the media is seekable.
 */
gboolean player_media_info_is_seekable(const PlayerMediaInfo *info) {
    g_return_val_if_fail (GST_IS_PLAYER_MEDIA_INFO(info), FALSE);

    return info->seekable;
}

/**
 * player_media_info_is_live:
 * @info: a #PlayerMediaInfo
 *
 * Returns: %TRUE if the media is live.
 */
gboolean player_media_info_is_live(const PlayerMediaInfo *info) {
    g_return_val_if_fail (GST_IS_PLAYER_MEDIA_INFO(info), FALSE);

    return info->is_live;
}

/**
 * player_media_info_get_stream_list:
 * @info: a #PlayerMediaInfo
 *
 * Returns: (transfer none) (element-type PlayerStreamInfo): A #GList of
 * matching #PlayerStreamInfo.
 */
GList *player_media_info_get_stream_list(const PlayerMediaInfo *info) {
    g_return_val_if_fail (GST_IS_PLAYER_MEDIA_INFO(info), NULL);

    return info->stream_list;
}

/**
 * player_media_info_get_audio_streams:
 * @info: a #PlayerMediaInfo
 *
 * Returns: (transfer none) (element-type PlayerAudioInfo): A #GList of
 * matching #PlayerAudioInfo.
 */
GList *player_media_info_get_audio_streams(const PlayerMediaInfo *info) {
    g_return_val_if_fail (GST_IS_PLAYER_MEDIA_INFO(info), NULL);

    return info->audio_stream_list;
}

/**
 * player_media_info_get_duration:
 * @info: a #PlayerMediaInfo
 *
 * Returns: duration of the media.
 */
GstClockTime player_media_info_get_duration(const PlayerMediaInfo *info) {
    g_return_val_if_fail (GST_IS_PLAYER_MEDIA_INFO(info), -1);

    return info->duration;
}

/**
 * player_media_info_get_title:
 * @info: a #PlayerMediaInfo
 *
 * Returns: the media title.
 */
const gchar *player_media_info_get_title(const PlayerMediaInfo *info) {
    g_return_val_if_fail (GST_IS_PLAYER_MEDIA_INFO(info), NULL);

    return info->title;
}

/**
 * player_media_info_get_container_format:
 * @info: a #PlayerMediaInfo
 *
 * Returns: the container format.
 */
const gchar *player_media_info_get_container_format(const PlayerMediaInfo *info) {
    g_return_val_if_fail (GST_IS_PLAYER_MEDIA_INFO(info), NULL);

    return info->container;
}

/**
 * player_media_info_get_number_of_streams:
 * @info: a #PlayerMediaInfo
 *
 * Returns: number of total streams.
 * Since: 1.12
 */
guint player_media_info_get_number_of_streams(const PlayerMediaInfo *info) {
    g_return_val_if_fail (GST_IS_PLAYER_MEDIA_INFO(info), 0);

    return g_list_length(info->stream_list);
}


/**
 * player_media_info_get_number_of_audio_streams:
 * @info: a #PlayerMediaInfo
 *
 * Returns: number of audio streams.
 * Since: 1.12
 */
guint player_media_info_get_number_of_audio_streams(const PlayerMediaInfo *info) {
    g_return_val_if_fail (GST_IS_PLAYER_MEDIA_INFO(info), 0);
    return g_list_length(info->audio_stream_list);
}

/**
 * player_get_audio_streams:
 * @info: a #PlayerMediaInfo
 *
 * Returns: (transfer none) (element-type PlayerAudioInfo): A #GList of
 * matching #PlayerAudioInfo.
 */
#ifndef GST_REMOVE_DEPRECATED

GList *player_get_audio_streams(const PlayerMediaInfo *info) {
    return player_media_info_get_audio_streams(info);
}

#endif


