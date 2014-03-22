#include "VideoActor.h"

#include "../util/Log.h"

VideoActor::VideoActor(Actor* actor,
                      AssetDescriptor* asset_descriptor,
                      AssetQualityDescriptor* asset_quality_descriptor) {
/**
  if (actor->id.compare("") != 0) {
    bin          = gst_bin_new(actor->id.c_str());
    filesrc      = gst_element_factory_make("filesrc", actor->id.c_str());
    g_object_set(G_OBJECT(filesrc),
                 "location", 
                 asset_quality_descriptor->composed_path.c_str(),
                 NULL);
    queue        = gst_element_factory_make("queue", actor->id.c_str());
    decodebin    = gst_element_factory_make("decodebin", actor->id.c_str());
    g_signal_connect(decodebin, "pad-added",
                     G_CALLBACK(VideoActor::OnDecodebinPadAdded),
                     this);
    videoconvert = gst_element_factory_make("videoconvert", actor->id.c_str());
    audioconvert = gst_element_factory_make("audioconvert", actor->id.c_str());
    videoscale   = gst_element_factory_make("videoscale", actor->id.c_str());
    volume       = gst_element_factory_make("volume", actor->id.c_str());
    gst_bin_add_many(GST_BIN(bin),
                     filesrc,
                     queue,
                     decodebin,
                     videoconvert,
                     audioconvert,
                     videoscale,
                     volume);
    gst_element_link(filesrc, queue, decodebin);
    gst_element_link(videoconvert, videoscale);
    gst_element_link(audioconvert, volume);
  }
  */
};

/*
void VideoActor::OnDecodebinPadAdded(GstElement* decodebin,
                                     GstPad* pad,
                                     gpointer* data) {
  VideoActor* video_actor = reinterpret_cast<VideoActor*>(data);
};
*/
