#include "VideoActor.h"

#include "GstConstants.h"
#include "../util/Math.h"
#include "../util/Log.h"

VideoActor::VideoActor(Actor* actor,
                      AssetDescriptor* asset_descriptor,
                      AssetQualityDescriptor* asset_quality_descriptor) :
            BaseActor(actor, asset_descriptor, asset_quality_descriptor) {
  if (actor->id.compare("") != 0) {
    bin          = gst_bin_new(GetGstElementId(GST_ELEM_BIN).c_str());
    filesrc      = gst_element_factory_make(GST_ELEM_FILESRC.c_str(),
                                            GetGstElementId(GST_ELEM_FILESRC).c_str());
    g_object_set(G_OBJECT(filesrc),
                 GST_PROP_LOCATION.c_str(), 
                 asset_quality_descriptor->composed_path.c_str(),
                 NULL);
    queue        = gst_element_factory_make(GST_ELEM_QUEUE.c_str(),
                                            GetGstElementId(GST_ELEM_QUEUE).c_str());
    decodebin    = gst_element_factory_make(GST_ELEM_DECODEBIN.c_str(),
                                            GetGstElementId(GST_ELEM_DECODEBIN).c_str());
    g_signal_connect(decodebin, GST_EVENT_PAD_ADDED.c_str(),
                     G_CALLBACK(VideoActor::OnDecodebinPadAdded),
                     this);
    videoconvert = gst_element_factory_make(GST_ELEM_VIDEOCONVERT.c_str(),
                                            GetGstElementId(GST_ELEM_VIDEOCONVERT).c_str());
    audioconvert = gst_element_factory_make(GST_ELEM_AUDIOCONVERT.c_str(),
                                            GetGstElementId(GST_ELEM_AUDIOCONVERT).c_str());
    videoscale   = gst_element_factory_make(GST_ELEM_VIDEOSCALE.c_str(),
                                            GetGstElementId(GST_ELEM_VIDEOSCALE).c_str());
    if (actor->resize_mode == Actor::ResizeMode::AspectBorderTransparent ||
        actor->resize_mode == Actor::ResizeMode::AspectNone) {
      g_object_set(G_OBJECT(videoscale),
                   "add-borders",
                   false,
                   NULL);
    }
    filter       = gst_element_factory_make(GST_ELEM_CAPSFILTER.c_str(),
                                            GetGstElementId(GST_ELEM_CAPSFILTER).c_str());
    volume       = gst_element_factory_make(GST_ELEM_VOLUME.c_str(),
                                            GetGstElementId(GST_ELEM_VOLUME).c_str());

    CheckGstElements({{GST_ELEM_FILESRC, filesrc},
                      {GST_ELEM_QUEUE, queue},
                      {GST_ELEM_DECODEBIN, decodebin},
                      {GST_ELEM_VIDEOCONVERT, videoconvert},
                      {GST_ELEM_AUDIOCONVERT, audioconvert},
                      {GST_ELEM_CAPSFILTER, filter},
                      {GST_ELEM_VIDEOSCALE, videoscale},
                      {GST_ELEM_VOLUME, volume}});

    gst_bin_add_many(GST_BIN(bin),
                     filesrc,
                     queue,
                     decodebin,
                     videoconvert,
                     audioconvert,
                     videoscale,
                     filter,
                     volume, NULL);

    gst_element_link_many(filesrc, queue, decodebin, NULL);
    gst_element_link_many(videoconvert, videoscale, filter, NULL);
    gst_element_link_many(audioconvert, volume, NULL);

    // Video Src to Bin
    video_src_pad = gst_element_get_static_pad (filter, M_GST_PAD_SRC.c_str());
    gst_element_add_pad(bin, gst_ghost_pad_new(M_GST_PAD_VIDEO_SRC.c_str(), video_src_pad));
    gst_object_unref(GST_OBJECT (video_src_pad));

    // Audio Src to Bin
    audio_src_pad = gst_element_get_static_pad (volume, M_GST_PAD_SRC.c_str());
    gst_element_add_pad(bin, gst_ghost_pad_new(M_GST_PAD_AUDIO_SRC.c_str(), audio_src_pad));
    gst_object_unref(GST_OBJECT (audio_src_pad));
  }
};

void VideoActor::SetGstElements() {
  LOG_TRACE("actor->width:" << actor->width ,LOGGER_PIPELINE);
  SetDimensions(actor->width, actor->height);
  SetX(actor->x);
  SetY(actor->y);
  SetZ(actor->z);
  LOG_TRACE("x:" << actor->x <<  " y:" << actor->y << " z:" << actor->z, LOGGER_PIPELINE);
};

void VideoActor::SetDimensions(double width, double height) {
  this->width = width;
  this->height = height;
  GstCaps* filter_caps;
  int scene_width = 640;
  int scene_height = 480;
  area_width_px = Math::RelativeToPx(width, scene_width);
  area_height_px = Math::RelativeToPx(height, scene_height);
  switch (actor->resize_mode) {
    case Actor::ResizeMode::AspectBorderTransparent:
      if (width > height) {
        height_px = area_height_px;
        width_px = height_px *
                   asset_quality_descriptor->natural_width /
                   asset_quality_descriptor->natural_height;
      } else {
        width_px = area_width_px;
        height_px = width_px *
                    asset_quality_descriptor->natural_height /
                    asset_quality_descriptor->natural_width;
      }
      break;
    case Actor::ResizeMode::AspectBorderBlack:
    case Actor::ResizeMode::AspectNone:
    case Actor::ResizeMode::AspectCrop:
      width_px = area_width_px;
      height_px = area_height_px;
      break;
  }
  filter_caps = gst_caps_new_simple ("video/x-raw",
         GST_PROP_WIDTH.c_str(), G_TYPE_INT, width_px,
         GST_PROP_HEIGHT.c_str(), G_TYPE_INT, height_px,
         NULL);
  g_object_set(G_OBJECT (filter), "caps", filter_caps, NULL);
  gst_caps_unref(filter_caps);
};

void VideoActor::SetX(double x) {
  this->x = x;
  if (actor->resize_mode == Actor::ResizeMode::AspectBorderTransparent) {
    g_object_set(G_OBJECT(videomixer_pad),
                 "xpos",
                 Math::RelativeToPx(x, 640) + ((area_width_px - width_px) / 2),
                 NULL);
  } else {
    g_object_set(G_OBJECT(videomixer_pad),
                 "xpos",
                 Math::RelativeToPx(x, 640),
                 NULL);
  }
};

void VideoActor::SetY(double y) {
  this->y = y;
  if (actor->resize_mode == Actor::ResizeMode::AspectBorderTransparent) {
    g_object_set(G_OBJECT(videomixer_pad),
                 "ypos",
                 Math::RelativeToPx(y, 480) + ((area_height_px - height_px) / 2),
                 NULL);
  } else {
    g_object_set(G_OBJECT(videomixer_pad),
                 "ypos",
                 Math::RelativeToPx(y, 480),
                 NULL);
  }
};

void VideoActor::SetZ(int z) {
  this->z = z;
  g_object_set(G_OBJECT(videomixer_pad),
                 "zorder",
                 z,
                 NULL);
};

void VideoActor::SetAlpha(double alpha) {
  this->alpha = alpha;
  g_object_set(G_OBJECT(videomixer_pad),
                 "alpha",
                 alpha,
                 NULL);
};

void VideoActor::OnDecodebinPadAdded(GstElement* decodebin,
                                     GstPad* pad,
                                     VideoActor* video_actor) {
  GstCaps          *new_pad_caps = NULL;
  GstStructure     *new_pad_struct = NULL;
  const gchar      *new_pad_type = NULL;
  GstPad           *sink_pad = NULL;
  GstPadLinkReturn  ret;

  new_pad_caps   = gst_pad_query_caps(pad, NULL);
  new_pad_struct = gst_caps_get_structure(new_pad_caps, 0);
  new_pad_type   = gst_structure_get_name(new_pad_struct);

  std::string s_pad_type = std::string(new_pad_type);
  if (g_str_has_prefix(new_pad_type, "video/x-raw")) {
    sink_pad = gst_element_get_static_pad(video_actor->videoconvert, "sink");
  }
  if (g_str_has_prefix(new_pad_type, "audio/x-raw")) {
    sink_pad = gst_element_get_static_pad(video_actor->audioconvert, "sink");
  }
  if (!gst_pad_is_linked(sink_pad))
  {
    ret = gst_pad_link(pad, sink_pad);
  }
  gst_caps_unref(new_pad_caps);
  gst_object_unref(sink_pad);
};
