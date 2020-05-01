#include "player-media-info.h"

#ifndef __GST_PLAYER_MEDIA_INFO_PRIVATE_H__
#define __GST_PLAYER_MEDIA_INFO_PRIVATE_H__

struct _PlayerStreamInfo
{
  GObject parent;

  //gchar *codec;

  GstCaps *caps;
  gint stream_index;
  GstTagList  *tags;
  gchar *stream_id;
};

struct _PlayerStreamInfoClass
{
  GObjectClass parent_class;
};

struct _PlayerSubtitleInfo
{
  PlayerStreamInfo  parent;

  gchar *language;
};

struct _PlayerSubtitleInfoClass
{
  PlayerStreamInfoClass parent_class;
};

struct _PlayerAudioInfo
{
  PlayerStreamInfo  parent;

  gint channels;
  gint sample_rate;

  guint bitrate;
  guint max_bitrate;

  gchar *language;
};

struct _PlayerAudioInfoClass
{
  PlayerStreamInfoClass parent_class;
};

struct _PlayerVideoInfo
{
  PlayerStreamInfo  parent;

  gint width;
  gint height;
  gint framerate_num;
  gint framerate_denom;
  gint par_num;
  gint par_denom;

  guint bitrate;
  guint max_bitrate;
};

struct _PlayerVideoInfoClass
{
  PlayerStreamInfoClass parent_class;
};

struct _PlayerMediaInfo
{
  GObject parent;

  gchar *uri;
  gchar *title;
  gchar *container;
  gboolean seekable, is_live;
  GstTagList *tags;
  GstSample *image_sample;

  GList *stream_list;
  GList *audio_stream_list;
  GList *video_stream_list;
  GList *subtitle_stream_list;

  GstClockTime  duration;
};

struct _PlayerMediaInfoClass
{
  GObjectClass parent_class;
};

G_GNUC_INTERNAL PlayerMediaInfo*   player_media_info_new
                                      (const gchar *uri);
G_GNUC_INTERNAL PlayerMediaInfo*   player_media_info_copy
                                      (PlayerMediaInfo *ref);
G_GNUC_INTERNAL PlayerStreamInfo*  player_stream_info_new
                                      (gint stream_index, GType type);
G_GNUC_INTERNAL PlayerStreamInfo*  player_stream_info_copy
                                      (PlayerStreamInfo *ref);

#endif /* __GST_PLAYER_MEDIA_INFO_PRIVATE_H__ */
