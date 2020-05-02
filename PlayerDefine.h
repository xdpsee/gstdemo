#ifndef __GST_PLAYER_H__
#define __GST_PLAYER_H__

#include <gst/gst.h>
#include "PlayerPrelude.h"
#include "PlayerTypes.h"
#include "MediaInfo.h"
#include "PlayerSignalDispatcher.h"

G_BEGIN_DECLS

GType player_state_get_type(void);

#define      GST_TYPE_PLAYER_STATE                    (player_state_get_type ())

/**
 * PlayerState:
 * @PLAYER_STATE_STOPPED: the player is stopped.
 * @PLAYER_STATE_BUFFERING: the player is buffering.
 * @PLAYER_STATE_PAUSED: the player is paused.
 * @PLAYER_STATE_PLAYING: the player is currently playing a
 * stream.
 */
typedef enum {
    PLAYER_STATE_STOPPED,
    PLAYER_STATE_BUFFERING,
    PLAYER_STATE_PAUSED,
    PLAYER_STATE_PLAYING
} PlayerState;

const gchar *player_state_get_name(PlayerState state);

GQuark player_error_quark(void);

GType player_error_get_type(void);

#define      PLAYER_ERROR                         (player_error_quark ())
#define      TYPE_PLAYER_ERROR                    (player_error_get_type ())

/**
 * PlayerError:
 * @GST_PLAYER_ERROR_FAILED: generic error.
 */
typedef enum {
    PLAYER_ERROR_FAILED = 0
} PlayerError;

const gchar *player_error_get_name(PlayerError error);

#define GST_TYPE_PLAYER             (player_get_type ())
#define GST_IS_PLAYER(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GST_TYPE_PLAYER))
#define GST_IS_PLAYER_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GST_TYPE_PLAYER))
#define GST_PLAYER_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GST_TYPE_PLAYER, PlayerClass))
#define GST_PLAYER(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GST_TYPE_PLAYER, Player))
#define GST_PLAYER_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GST_TYPE_PLAYER, PlayerClass))
#define GST_PLAYER_CAST(obj)        ((GstPlayer*)(obj))


GType player_get_type(void);

Player *player_new(PlayerSignalDispatcher *signal_dispatcher);

void player_play(Player *player);

void player_pause(Player *player);

void player_stop(Player *player);

void player_seek(Player *player,
                 GstClockTime position);

void player_set_rate(Player *player,
                     gdouble rate);

gdouble player_get_rate(Player *player);

gchar *player_get_uri(Player *player);


void player_set_uri(Player *player,
                    const gchar *uri);


GstClockTime player_get_position(Player *player);


GstClockTime player_get_duration(Player *player);


gdouble player_get_volume(Player *player);


void player_set_volume(Player *player,
                       gdouble val);


gboolean player_get_mute(Player *player);


void player_set_mute(Player *player,
                     gboolean val);


GstElement *player_get_pipeline(Player *player);


void player_set_video_track_enabled(Player *player,
                                    gboolean enabled);


void player_set_audio_track_enabled(Player *player,
                                    gboolean enabled);


void player_set_subtitle_track_enabled(Player *player,
                                       gboolean enabled);


gboolean player_set_audio_track(Player *player,
                                gint stream_index);

PlayerMediaInfo *player_get_media_info(Player *player);


PlayerAudioInfo *player_get_current_audio_track(Player *player);

gboolean player_set_config(Player *player,
                           GstStructure *config);


GstStructure *player_get_config(Player *player);

/* helpers for configuring the config structure */


void player_config_set_user_agent(GstStructure *config,
                                  const gchar *agent);


gchar *player_config_get_user_agent(const GstStructure *config);


void player_config_set_position_update_interval(GstStructure *config,
                                                guint interval);


guint player_config_get_position_update_interval(const GstStructure *config);


void player_config_set_seek_accurate(GstStructure *config, gboolean accurate);


gboolean player_config_get_seek_accurate(const GstStructure *config);

G_END_DECLS

#endif /* __GST_PLAYER_H__ */
