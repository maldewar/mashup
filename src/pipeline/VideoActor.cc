#include "VideoActor.h"

#include "GstConstants.h"
#include "../util/Log.h"

VideoActor::VideoActor(Actor* actor,
                      AssetDescriptor* asset_descriptor,
                      AssetQualityDescriptor* asset_quality_descriptor) {
  this->actor = actor;
  this->asset_descriptor = asset_descriptor;
  this->asset_quality_descriptor = asset_quality_descriptor;
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
    volume       = gst_element_factory_make(GST_ELEM_VOLUME.c_str(),
                                            GetGstElementId(GST_ELEM_VOLUME).c_str());

    gst_bin_add_many(GST_BIN(bin),
                     filesrc,
                     queue,
                     decodebin,
                     videoconvert,
                     audioconvert,
                     videoscale,
                     volume, NULL);

    gst_element_link_many(filesrc, queue, decodebin, NULL);
    gst_element_link_many(videoconvert, videoscale, NULL);
    gst_element_link_many(audioconvert, volume, NULL);

    // Video Src to Bin
    video_src_pad = gst_element_get_static_pad (videoscale, M_GST_PAD_SRC.c_str());
    gst_element_add_pad(bin, gst_ghost_pad_new(M_GST_PAD_VIDEO_SRC.c_str(), video_src_pad));
    gst_object_unref(GST_OBJECT (video_src_pad));

    // Audio Src to Bin
    audio_src_pad = gst_element_get_static_pad (volume, M_GST_PAD_SRC.c_str());
    gst_element_add_pad(bin, gst_ghost_pad_new(M_GST_PAD_AUDIO_SRC.c_str(), audio_src_pad));
    gst_object_unref(GST_OBJECT (audio_src_pad));

    videomixer_pad = NULL;
    audiomixer_pad = NULL;
  }
};

std::string VideoActor::GetGstElementId(std::string base) {
  ss.str(std::string());
  ss << base << "_" << actor->id;
  return ss.str().c_str();
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
