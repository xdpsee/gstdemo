#ifndef __GST_PLAYER_MEDIA_INFO_H__
#define __GST_PLAYER_MEDIA_INFO_H__

#include <gst/gst.h>
#include "PlayerPrelude.h"

G_BEGIN_DECLS

#define GST_TYPE_PLAYER_STREAM_INFO \
  (player_stream_info_get_type ())
#define GST_PLAYER_STREAM_INFO(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_PLAYER_STREAM_INFO,PlayerStreamInfo))
#define GST_PLAYER_STREAM_INFO_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_PLAYER_STREAM_INFO,PlayerStreamInfo))
#define GST_IS_PLAYER_STREAM_INFO(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_PLAYER_STREAM_INFO))
#define GST_IS_PLAYER_STREAM_INFO_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_PLAYER_STREAM_INFO))

/**
 * PlayerStreamInfo:
 *
 * Base structure for information concerning a media stream. Depending on
 * the stream type, one can find more media-specific information in
 * #PlayerVideoInfo, #PlayerAudioInfo, #PlayerSubtitleInfo.
 */
typedef struct _PlayerStreamInfo PlayerStreamInfo;
typedef struct _PlayerStreamInfoClass PlayerStreamInfoClass;

GST_PLAYER_API
GType         player_stream_info_get_type (void);

GST_PLAYER_API
gint          player_stream_info_get_index (const PlayerStreamInfo *info);

GST_PLAYER_API
const gchar*  player_stream_info_get_stream_type (const PlayerStreamInfo *info);

GST_PLAYER_API
GstTagList*   player_stream_info_get_tags  (const PlayerStreamInfo *info);

GST_PLAYER_API
GstCaps*      player_stream_info_get_caps  (const PlayerStreamInfo *info);

#define GST_TYPE_PLAYER_AUDIO_INFO \
  (player_audio_info_get_type ())
#define GST_PLAYER_AUDIO_INFO(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_PLAYER_AUDIO_INFO, PlayerAudioInfo))
#define GST_PLAYER_AUDIO_INFO_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_PLAYER_AUDIO_INFO, PlayerAudioInfoClass))
#define GST_IS_PLAYER_AUDIO_INFO(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_PLAYER_AUDIO_INFO))
#define GST_IS_PLAYER_AUDIO_INFO_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_PLAYER_AUDIO_INFO))

/**
 * PlayerAudioInfo:
 *
 * #PlayerStreamInfo specific to audio streams.
 */
typedef struct _PlayerAudioInfo PlayerAudioInfo;
typedef struct _PlayerAudioInfoClass PlayerAudioInfoClass;

GST_PLAYER_API
GType         player_audio_info_get_type (void);

GST_PLAYER_API
gint          player_audio_info_get_channels    (const PlayerAudioInfo* info);

GST_PLAYER_API
gint          player_audio_info_get_sample_rate (const PlayerAudioInfo* info);

GST_PLAYER_API
gint          player_audio_info_get_bitrate     (const PlayerAudioInfo* info);

GST_PLAYER_API
gint          player_audio_info_get_max_bitrate (const PlayerAudioInfo* info);

GST_PLAYER_API
const gchar*  player_audio_info_get_language    (const PlayerAudioInfo* info);

#define GST_TYPE_PLAYER_MEDIA_INFO \
  (player_media_info_get_type())
#define GST_PLAYER_MEDIA_INFO(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_PLAYER_MEDIA_INFO,PlayerMediaInfo))
#define GST_PLAYER_MEDIA_INFO_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_PLAYER_MEDIA_INFO,PlayerMediaInfoClass))
#define GST_IS_PLAYER_MEDIA_INFO(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_PLAYER_MEDIA_INFO))
#define GST_IS_PLAYER_MEDIA_INFO_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_PLAYER_MEDIA_INFO))

/**
 * PlayerMediaInfo:
 *
 * Structure containing the media information of a URI.
 */
typedef struct _PlayerMediaInfo PlayerMediaInfo;
typedef struct _PlayerMediaInfoClass PlayerMediaInfoClass;

GST_PLAYER_API
GType         player_media_info_get_type (void);

GST_PLAYER_API
const gchar * player_media_info_get_uri (const PlayerMediaInfo *info);

GST_PLAYER_API
gboolean      player_media_info_is_seekable (const PlayerMediaInfo *info);

GST_PLAYER_API
gboolean      player_media_info_is_live (const PlayerMediaInfo *info);

GST_PLAYER_API
GstClockTime  player_media_info_get_duration (const PlayerMediaInfo *info);

GST_PLAYER_API
GList*        player_media_info_get_stream_list (const PlayerMediaInfo *info);

GST_PLAYER_API
guint         player_media_info_get_number_of_streams (const PlayerMediaInfo *info);

GST_PLAYER_API
GList*        player_media_info_get_audio_streams (const PlayerMediaInfo *info);

GST_PLAYER_API
guint         player_media_info_get_number_of_audio_streams (const PlayerMediaInfo *info);

GST_PLAYER_API
const gchar*  player_media_info_get_title (const PlayerMediaInfo *info);

GST_PLAYER_API
const gchar*  player_media_info_get_container_format (const PlayerMediaInfo *info);

GST_PLAYER_DEPRECATED_FOR(player_media_info_get_audio_streams)
GList*        player_get_audio_streams    (const PlayerMediaInfo *info);

G_END_DECLS

#endif /* __GST_PLAYER_MEDIA_INFO_H */
