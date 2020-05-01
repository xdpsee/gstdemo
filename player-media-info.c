#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "player-media-info.h"
#include "player-media-info-private.h"

/* Per-stream information */
G_DEFINE_ABSTRACT_TYPE (PlayerStreamInfo, player_stream_info,
                        G_TYPE_OBJECT);

static void
player_stream_info_init (PlayerStreamInfo * sinfo)
{
  sinfo->stream_index = -1;
}

static void
player_stream_info_finalize (GObject * object)
{
  PlayerStreamInfo *sinfo = GST_PLAYER_STREAM_INFO (object);

  //g_free (sinfo->codec);
  g_free (sinfo->stream_id);

  if (sinfo->caps)
    gst_caps_unref (sinfo->caps);

  if (sinfo->tags)
    gst_tag_list_unref (sinfo->tags);

  G_OBJECT_CLASS (player_stream_info_parent_class)->finalize (object);
}

static void
player_stream_info_class_init (PlayerStreamInfoClass * klass)
{
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
gint
player_stream_info_get_index (const PlayerStreamInfo * info)
{
  g_return_val_if_fail (GST_IS_PLAYER_STREAM_INFO (info), -1);

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
const gchar *
player_stream_info_get_stream_type (const PlayerStreamInfo * info)
{
  g_return_val_if_fail (GST_IS_PLAYER_STREAM_INFO (info), NULL);

  if (GST_IS_PLAYER_VIDEO_INFO (info))
    return "video";
  else if (GST_IS_PLAYER_AUDIO_INFO (info))
    return "audio";
  else
    return "subtitle";
}

/**
 * player_stream_info_get_tags:
 * @info: a #PlayerStreamInfo
 *
 * Returns: (transfer none): the tags contained in this stream.
 */
GstTagList *
player_stream_info_get_tags (const PlayerStreamInfo * info)
{
  g_return_val_if_fail (GST_IS_PLAYER_STREAM_INFO (info), NULL);

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
const gchar *
player_stream_info_get_codec (const PlayerStreamInfo * info)
{
  g_return_val_if_fail (GST_IS_PLAYER_STREAM_INFO (info), NULL);

  return NULL;//info->codec;
}

/**
 * player_stream_info_get_caps:
 * @info: a #PlayerStreamInfo
 *
 * Returns: (transfer none): the #GstCaps of the stream.
 */
GstCaps *
player_stream_info_get_caps (const PlayerStreamInfo * info)
{
  g_return_val_if_fail (GST_IS_PLAYER_STREAM_INFO (info), NULL);

  return info->caps;
}

/* Video information */
G_DEFINE_TYPE (PlayerVideoInfo, player_video_info,
               GST_TYPE_PLAYER_STREAM_INFO);

static void
player_video_info_init (PlayerVideoInfo * info)
{
  info->width = -1;
  info->height = -1;
  info->framerate_num = 0;
  info->framerate_denom = 1;
  info->par_num = 1;
  info->par_denom = 1;
}

static void
player_video_info_class_init (G_GNUC_UNUSED PlayerVideoInfoClass * klass)
{
  /* nothing to do here */
}

/**
 * player_video_info_get_width:
 * @info: a #PlayerVideoInfo
 *
 * Returns: the width of video in #PlayerVideoInfo.
 */
gint
player_video_info_get_width (const PlayerVideoInfo * info)
{
  g_return_val_if_fail (GST_IS_PLAYER_VIDEO_INFO (info), -1);

  return info->width;
}

/**
 * player_video_info_get_height:
 * @info: a #PlayerVideoInfo
 *
 * Returns: the height of video in #PlayerVideoInfo.
 */
gint
player_video_info_get_height (const PlayerVideoInfo * info)
{
  g_return_val_if_fail (GST_IS_PLAYER_VIDEO_INFO (info), -1);

  return info->height;
}

/**
 * player_video_info_get_framerate:
 * @info: a #PlayerVideoInfo
 * @fps_n: (out): Numerator of frame rate
 * @fps_d: (out): Denominator of frame rate
 *
 */
void
player_video_info_get_framerate (const PlayerVideoInfo * info,
    gint * fps_n, gint * fps_d)
{
  g_return_if_fail (GST_IS_PLAYER_VIDEO_INFO (info));

  *fps_n = info->framerate_num;
  *fps_d = info->framerate_denom;
}

/**
 * player_video_info_get_pixel_aspect_ratio:
 * @info: a #PlayerVideoInfo
 * @par_n: (out): numerator
 * @par_d: (out): denominator
 *
 * Returns the pixel aspect ratio in @par_n and @par_d
 *
 */
void
player_video_info_get_pixel_aspect_ratio (const PlayerVideoInfo * info,
    guint * par_n, guint * par_d)
{
  g_return_if_fail (GST_IS_PLAYER_VIDEO_INFO (info));

  *par_n = info->par_num;
  *par_d = info->par_denom;
}

/**
 * player_video_info_get_bitrate:
 * @info: a #PlayerVideoInfo
 *
 * Returns: the current bitrate of video in #PlayerVideoInfo.
 */
gint
player_video_info_get_bitrate (const PlayerVideoInfo * info)
{
  g_return_val_if_fail (GST_IS_PLAYER_VIDEO_INFO (info), -1);

  return info->bitrate;
}

/**
 * player_video_info_get_max_bitrate:
 * @info: a #PlayerVideoInfo
 *
 * Returns: the maximum bitrate of video in #PlayerVideoInfo.
 */
gint
player_video_info_get_max_bitrate (const PlayerVideoInfo * info)
{
  g_return_val_if_fail (GST_IS_PLAYER_VIDEO_INFO (info), -1);

  return info->max_bitrate;
}

/* Audio information */
G_DEFINE_TYPE (PlayerAudioInfo, player_audio_info,
               GST_TYPE_PLAYER_STREAM_INFO);

static void
player_audio_info_init (PlayerAudioInfo * info)
{
  info->channels = 0;
  info->sample_rate = 0;
  info->bitrate = -1;
  info->max_bitrate = -1;
}

static void
player_audio_info_finalize (GObject * object)
{
  PlayerAudioInfo *info = GST_PLAYER_AUDIO_INFO (object);

  g_free (info->language);

  G_OBJECT_CLASS (player_audio_info_parent_class)->finalize (object);
}

static void
player_audio_info_class_init (PlayerAudioInfoClass * klass)
{
  GObjectClass *gobject_class = (GObjectClass *) klass;

  gobject_class->finalize = player_audio_info_finalize;
}

/**
 * player_audio_info_get_language:
 * @info: a #PlayerAudioInfo
 *
 * Returns: the language of the stream, or NULL if unknown.
 */
const gchar *
player_audio_info_get_language (const PlayerAudioInfo * info)
{
  g_return_val_if_fail (GST_IS_PLAYER_AUDIO_INFO (info), NULL);

  return info->language;
}

/**
 * player_audio_info_get_channels:
 * @info: a #PlayerAudioInfo
 *
 * Returns: the number of audio channels in #PlayerAudioInfo.
 */
gint
player_audio_info_get_channels (const PlayerAudioInfo * info)
{
  g_return_val_if_fail (GST_IS_PLAYER_AUDIO_INFO (info), 0);

  return info->channels;
}

/**
 * player_audio_info_get_sample_rate:
 * @info: a #PlayerAudioInfo
 *
 * Returns: the audio sample rate in #PlayerAudioInfo.
 */
gint
player_audio_info_get_sample_rate (const PlayerAudioInfo * info)
{
  g_return_val_if_fail (GST_IS_PLAYER_AUDIO_INFO (info), 0);

  return info->sample_rate;
}

/**
 * player_audio_info_get_bitrate:
 * @info: a #PlayerAudioInfo
 *
 * Returns: the audio bitrate in #PlayerAudioInfo.
 */
gint
player_audio_info_get_bitrate (const PlayerAudioInfo * info)
{
  g_return_val_if_fail (GST_IS_PLAYER_AUDIO_INFO (info), -1);

  return info->bitrate;
}

/**
 * player_audio_info_get_max_bitrate:
 * @info: a #PlayerAudioInfo
 *
 * Returns: the audio maximum bitrate in #PlayerAudioInfo.
 */
gint
player_audio_info_get_max_bitrate (const PlayerAudioInfo * info)
{
  g_return_val_if_fail (GST_IS_PLAYER_AUDIO_INFO (info), -1);

  return info->max_bitrate;
}

/* Subtitle information */
G_DEFINE_TYPE (PlayerSubtitleInfo, player_subtitle_info,
               GST_TYPE_PLAYER_STREAM_INFO);

static void player_subtitle_info_init (G_GNUC_UNUSED PlayerSubtitleInfo * info)
{
  /* nothing to do */
}

static void
player_subtitle_info_finalize (GObject * object)
{
  PlayerSubtitleInfo *info = GST_PLAYER_SUBTITLE_INFO (object);

  g_free (info->language);

  G_OBJECT_CLASS (player_subtitle_info_parent_class)->finalize (object);
}

static void
player_subtitle_info_class_init (PlayerSubtitleInfoClass * klass)
{
  GObjectClass *gobject_class = (GObjectClass *) klass;

  gobject_class->finalize = player_subtitle_info_finalize;
}

/**
 * player_subtitle_info_get_language:
 * @info: a #PlayerSubtitleInfo
 *
 * Returns: the language of the stream, or NULL if unknown.
 */
const gchar *
player_subtitle_info_get_language (const PlayerSubtitleInfo * info)
{
  g_return_val_if_fail (GST_IS_PLAYER_SUBTITLE_INFO (info), NULL);

  return info->language;
}

/* Global media information */
G_DEFINE_TYPE (PlayerMediaInfo, player_media_info, G_TYPE_OBJECT);

static void
player_media_info_init (PlayerMediaInfo * info)
{
  info->duration = -1;
  info->is_live = FALSE;
  info->seekable = FALSE;
}

static void
player_media_info_finalize (GObject * object)
{
  PlayerMediaInfo *info = GST_PLAYER_MEDIA_INFO (object);

  g_free (info->uri);

  if (info->tags)
    gst_tag_list_unref (info->tags);

  g_free (info->title);

  g_free (info->container);

  if (info->image_sample)
    gst_sample_unref (info->image_sample);

  if (info->audio_stream_list)
    g_list_free (info->audio_stream_list);

  if (info->video_stream_list)
    g_list_free (info->video_stream_list);

  if (info->subtitle_stream_list)
    g_list_free (info->subtitle_stream_list);

  if (info->stream_list)
    g_list_free_full (info->stream_list, g_object_unref);

  G_OBJECT_CLASS (player_media_info_parent_class)->finalize (object);
}

static void
player_media_info_class_init (PlayerMediaInfoClass * klass)
{
  GObjectClass *oclass = (GObjectClass *) klass;

  oclass->finalize = player_media_info_finalize;
}

static PlayerVideoInfo *
player_video_info_new (void)
{
  return g_object_new (GST_TYPE_PLAYER_VIDEO_INFO, NULL);
}

static PlayerAudioInfo *
player_audio_info_new (void)
{
  return g_object_new (GST_TYPE_PLAYER_AUDIO_INFO, NULL);
}

static PlayerSubtitleInfo *
player_subtitle_info_new (void)
{
  return g_object_new (GST_TYPE_PLAYER_SUBTITLE_INFO, NULL);
}

static PlayerStreamInfo *
player_video_info_copy (PlayerVideoInfo * ref)
{
  PlayerVideoInfo *ret;

  ret = player_video_info_new();

  ret->width = ref->width;
  ret->height = ref->height;
  ret->framerate_num = ref->framerate_num;
  ret->framerate_denom = ref->framerate_denom;
  ret->par_num = ref->par_num;
  ret->par_denom = ref->par_denom;
  ret->bitrate = ref->bitrate;
  ret->max_bitrate = ref->max_bitrate;

  return (PlayerStreamInfo *) ret;
}

static PlayerStreamInfo *
player_audio_info_copy (PlayerAudioInfo * ref)
{
  PlayerAudioInfo *ret;

  ret = player_audio_info_new();

  ret->sample_rate = ref->sample_rate;
  ret->channels = ref->channels;
  ret->bitrate = ref->bitrate;
  ret->max_bitrate = ref->max_bitrate;

  if (ref->language)
    ret->language = g_strdup (ref->language);

  return (PlayerStreamInfo *) ret;
}

static PlayerStreamInfo *
player_subtitle_info_copy (PlayerSubtitleInfo * ref)
{
  PlayerSubtitleInfo *ret;

  ret = player_subtitle_info_new();
  if (ref->language)
    ret->language = g_strdup (ref->language);

  return (PlayerStreamInfo *) ret;
}

PlayerStreamInfo *
player_stream_info_copy (PlayerStreamInfo * ref)
{
  PlayerStreamInfo *info = NULL;

  if (!ref)
    return NULL;

  if (GST_IS_PLAYER_VIDEO_INFO (ref))
    info = player_video_info_copy((PlayerVideoInfo *) ref);
  else if (GST_IS_PLAYER_AUDIO_INFO (ref))
    info = player_audio_info_copy((PlayerAudioInfo *) ref);
  else
    info = player_subtitle_info_copy((PlayerSubtitleInfo *) ref);

  info->stream_index = ref->stream_index;
  if (ref->tags)
    info->tags = gst_tag_list_ref (ref->tags);
  if (ref->caps)
    info->caps = gst_caps_copy (ref->caps);
//  if (ref->codec)
//    info->codec = g_strdup (ref->codec);
  if (ref->stream_id)
    info->stream_id = g_strdup (ref->stream_id);

  return info;
}

PlayerMediaInfo *
player_media_info_copy (PlayerMediaInfo * ref)
{
  GList *l;
  PlayerMediaInfo *info;

  if (!ref)
    return NULL;

  info = player_media_info_new(ref->uri);
  info->duration = ref->duration;
  info->seekable = ref->seekable;
  info->is_live = ref->is_live;
  if (ref->tags)
    info->tags = gst_tag_list_ref (ref->tags);
  if (ref->title)
    info->title = g_strdup (ref->title);
  if (ref->container)
    info->container = g_strdup (ref->container);
  if (ref->image_sample)
    info->image_sample = gst_sample_ref (ref->image_sample);

  for (l = ref->stream_list; l != NULL; l = l->next) {
    PlayerStreamInfo *s;

    s = player_stream_info_copy((PlayerStreamInfo *) l->data);
    info->stream_list = g_list_append (info->stream_list, s);

    if (GST_IS_PLAYER_AUDIO_INFO (s))
      info->audio_stream_list = g_list_append (info->audio_stream_list, s);
    else if (GST_IS_PLAYER_VIDEO_INFO (s))
      info->video_stream_list = g_list_append (info->video_stream_list, s);
    else
      info->subtitle_stream_list =
          g_list_append (info->subtitle_stream_list, s);
  }

  return info;
}

PlayerStreamInfo *
player_stream_info_new (gint stream_index, GType type)
{
  PlayerStreamInfo *info = NULL;

  if (type == GST_TYPE_PLAYER_AUDIO_INFO)
    info = (PlayerStreamInfo *) player_audio_info_new();
  else if (type == GST_TYPE_PLAYER_VIDEO_INFO)
    info = (PlayerStreamInfo *) player_video_info_new();
  else
    info = (PlayerStreamInfo *) player_subtitle_info_new();

  info->stream_index = stream_index;

  return info;
}

PlayerMediaInfo *
player_media_info_new (const gchar * uri)
{
  PlayerMediaInfo *info;

  g_return_val_if_fail (uri != NULL, NULL);

  info = g_object_new (GST_TYPE_PLAYER_MEDIA_INFO, NULL);
  info->uri = g_strdup (uri);

  return info;
}

/**
 * player_media_info_get_uri:
 * @info: a #PlayerMediaInfo
 *
 * Returns: the URI associated with #PlayerMediaInfo.
 */
const gchar *
player_media_info_get_uri (const PlayerMediaInfo * info)
{
  g_return_val_if_fail (GST_IS_PLAYER_MEDIA_INFO (info), NULL);

  return info->uri;
}

/**
 * player_media_info_is_seekable:
 * @info: a #PlayerMediaInfo
 *
 * Returns: %TRUE if the media is seekable.
 */
gboolean
player_media_info_is_seekable (const PlayerMediaInfo * info)
{
  g_return_val_if_fail (GST_IS_PLAYER_MEDIA_INFO (info), FALSE);

  return info->seekable;
}

/**
 * player_media_info_is_live:
 * @info: a #PlayerMediaInfo
 *
 * Returns: %TRUE if the media is live.
 */
gboolean
player_media_info_is_live (const PlayerMediaInfo * info)
{
  g_return_val_if_fail (GST_IS_PLAYER_MEDIA_INFO (info), FALSE);

  return info->is_live;
}

/**
 * player_media_info_get_stream_list:
 * @info: a #PlayerMediaInfo
 *
 * Returns: (transfer none) (element-type PlayerStreamInfo): A #GList of
 * matching #PlayerStreamInfo.
 */
GList *
player_media_info_get_stream_list (const PlayerMediaInfo * info)
{
  g_return_val_if_fail (GST_IS_PLAYER_MEDIA_INFO (info), NULL);

  return info->stream_list;
}

/**
 * player_media_info_get_video_streams:
 * @info: a #PlayerMediaInfo
 *
 * Returns: (transfer none) (element-type PlayerVideoInfo): A #GList of
 * matching #PlayerVideoInfo.
 */
GList *
player_media_info_get_video_streams (const PlayerMediaInfo * info)
{
  g_return_val_if_fail (GST_IS_PLAYER_MEDIA_INFO (info), NULL);

  return info->video_stream_list;
}

/**
 * player_media_info_get_subtitle_streams:
 * @info: a #PlayerMediaInfo
 *
 * Returns: (transfer none) (element-type PlayerSubtitleInfo): A #GList of
 * matching #PlayerSubtitleInfo.
 */
GList *
player_media_info_get_subtitle_streams (const PlayerMediaInfo * info)
{
  g_return_val_if_fail (GST_IS_PLAYER_MEDIA_INFO (info), NULL);

  return info->subtitle_stream_list;
}

/**
 * player_media_info_get_audio_streams:
 * @info: a #PlayerMediaInfo
 *
 * Returns: (transfer none) (element-type PlayerAudioInfo): A #GList of
 * matching #PlayerAudioInfo.
 */
GList *
player_media_info_get_audio_streams (const PlayerMediaInfo * info)
{
  g_return_val_if_fail (GST_IS_PLAYER_MEDIA_INFO (info), NULL);

  return info->audio_stream_list;
}

/**
 * player_media_info_get_duration:
 * @info: a #PlayerMediaInfo
 *
 * Returns: duration of the media.
 */
GstClockTime
player_media_info_get_duration (const PlayerMediaInfo * info)
{
  g_return_val_if_fail (GST_IS_PLAYER_MEDIA_INFO (info), -1);

  return info->duration;
}

/**
 * player_media_info_get_tags:
 * @info: a #PlayerMediaInfo
 *
 * Returns: (transfer none): the tags contained in media info.
 */
GstTagList *
player_media_info_get_tags (const PlayerMediaInfo * info)
{
  g_return_val_if_fail (GST_IS_PLAYER_MEDIA_INFO (info), NULL);

  return info->tags;
}

/**
 * player_media_info_get_title:
 * @info: a #PlayerMediaInfo
 *
 * Returns: the media title.
 */
const gchar *
player_media_info_get_title (const PlayerMediaInfo * info)
{
  g_return_val_if_fail (GST_IS_PLAYER_MEDIA_INFO (info), NULL);

  return info->title;
}

/**
 * player_media_info_get_container_format:
 * @info: a #PlayerMediaInfo
 *
 * Returns: the container format.
 */
const gchar *
player_media_info_get_container_format (const PlayerMediaInfo * info)
{
  g_return_val_if_fail (GST_IS_PLAYER_MEDIA_INFO (info), NULL);

  return info->container;
}

/**
 * player_media_info_get_image_sample:
 * @info: a #PlayerMediaInfo
 *
 * Function to get the image (or preview-image) stored in taglist.
 * Application can use `gst_sample_*_()` API's to get caps, buffer etc.
 *
 * Returns: (transfer none): GstSample or NULL.
 */
GstSample *
player_media_info_get_image_sample (const PlayerMediaInfo * info)
{
  g_return_val_if_fail (GST_IS_PLAYER_MEDIA_INFO (info), NULL);

  return info->image_sample;
}

/**
 * player_media_info_get_number_of_streams:
 * @info: a #PlayerMediaInfo
 *
 * Returns: number of total streams.
 * Since: 1.12
 */
guint
player_media_info_get_number_of_streams (const PlayerMediaInfo * info)
{
  g_return_val_if_fail (GST_IS_PLAYER_MEDIA_INFO (info), 0);

  return g_list_length (info->stream_list);
}

/**
 * player_media_info_get_number_of_video_streams:
 * @info: a #PlayerMediaInfo
 *
 * Returns: number of video streams.
 * Since: 1.12
 */
guint
player_media_info_get_number_of_video_streams (const PlayerMediaInfo *
    info)
{
  g_return_val_if_fail (GST_IS_PLAYER_MEDIA_INFO (info), 0);

  return g_list_length (info->video_stream_list);
}

/**
 * player_media_info_get_number_of_audio_streams:
 * @info: a #PlayerMediaInfo
 *
 * Returns: number of audio streams.
 * Since: 1.12
 */
guint
player_media_info_get_number_of_audio_streams (const PlayerMediaInfo *
    info)
{
  g_return_val_if_fail (GST_IS_PLAYER_MEDIA_INFO (info), 0);

  return g_list_length (info->audio_stream_list);
}

/**
 * player_media_info_get_number_of_subtitle_streams:
 * @info: a #PlayerMediaInfo
 *
 * Returns: number of subtitle streams.
 * Since: 1.12
 */
guint player_media_info_get_number_of_subtitle_streams
    (const PlayerMediaInfo * info)
{
  g_return_val_if_fail (GST_IS_PLAYER_MEDIA_INFO (info), 0);

  return g_list_length (info->subtitle_stream_list);
}

/**
 * player_get_video_streams:
 * @info: a #PlayerMediaInfo
 *
 * Returns: (transfer none) (element-type PlayerVideoInfo): A #GList of
 * matching #PlayerVideoInfo.
 */
#ifndef GST_REMOVE_DEPRECATED
GList *
player_get_video_streams (const PlayerMediaInfo * info)
{
  return player_media_info_get_video_streams(info);
}
#endif

/**
 * player_get_audio_streams:
 * @info: a #PlayerMediaInfo
 *
 * Returns: (transfer none) (element-type PlayerAudioInfo): A #GList of
 * matching #PlayerAudioInfo.
 */
#ifndef GST_REMOVE_DEPRECATED
GList *
player_get_audio_streams (const PlayerMediaInfo * info)
{
  return player_media_info_get_audio_streams(info);
}
#endif

/**
 * player_get_subtitle_streams:
 * @info: a #PlayerMediaInfo
 *
 * Returns: (transfer none) (element-type PlayerSubtitleInfo): A #GList of
 * matching #PlayerSubtitleInfo.
 */
#ifndef GST_REMOVE_DEPRECATED
GList *
player_get_subtitle_streams (const PlayerMediaInfo * info)
{
  return player_media_info_get_subtitle_streams(info);
}
#endif
