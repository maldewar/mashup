#include "Pipeline.h"

#include "GstConstants.h"
#include "../util/Log.h"

Pipeline::Pipeline() {
  pipeline = gst_pipeline_new(id.c_str());
  videomixer = gst_element_factory_make("videomixer", "videomixer");
  audiomixer = gst_element_factory_make("adder", "audiomixer");
  filter     = gst_element_factory_make("capsfilter","filter");
};

void Pipeline::Prepare() {
  for (const auto& video_actor_pair : m_video_actors) {
    GstPad* video_src_pad;
    GstPad* audio_src_pad;
    GstPad* video_sink_pad;
    GstPad* audio_sink_pad;
    VideoActor* video_actor = video_actor_pair.second;
    gst_bin_add(GST_BIN(pipeline), video_actor->bin);
    video_src_pad = gst_element_get_static_pad(video_actor->bin, M_GST_PAD_VIDEO_SRC.c_str());
    audio_src_pad = gst_element_get_static_pad(video_actor->bin, M_GST_PAD_AUDIO_SRC.c_str());
    video_sink_pad = gst_element_get_request_pad(videomixer, M_GST_PAD_MULTI_SINK.c_str());
    audio_sink_pad = gst_element_get_request_pad(audiomixer, M_GST_PAD_MULTI_SINK.c_str());
    gst_pad_link(video_src_pad, video_sink_pad);
    gst_pad_link(audio_src_pad, audio_sink_pad);
    video_actor->videomixer_pad = video_sink_pad;
    video_actor->audiomixer_pad = audio_sink_pad;
    /*
    g_object_set(G_OBJECT(video_sink_pad),
                 "xpos", 
                 100,
                 NULL);
    g_object_set(G_OBJECT(video_sink_pad),
                 "ypos",                   
                 50,
                 NULL);
    g_object_set(G_OBJECT(video_sink_pad),
                 "alpha",                   
                 0.7f,
                 NULL);
    */
    gst_object_unref(GST_OBJECT(video_src_pad));
    gst_object_unref(GST_OBJECT(audio_src_pad));
  };
};

void Pipeline::Play() {
  GMainLoop *loop;
  loop = g_main_loop_new(NULL, false);
  GstElement *e_videotestsrc;
  GstElement *e_audiotestsrc;
  GstElement *e_autovideosink;
  GstElement *e_autoaudiosink;
  e_autovideosink = gst_element_factory_make("autovideosink", "autovideosink");
  e_autoaudiosink = gst_element_factory_make("autoaudiosink", "autoaudiosink");
  gst_bin_add_many(GST_BIN(pipeline), videomixer, filter, e_autovideosink, NULL);
  gst_bin_add_many(GST_BIN(pipeline), audiomixer, e_autoaudiosink, NULL);
  gst_element_link_many(videomixer, filter, e_autovideosink, NULL);
  gst_element_link_many(audiomixer, e_autoaudiosink, NULL);
  Prepare();
  gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_PLAYING);
  g_main_loop_run (loop);
  //gst_element_set_state (pipeline, GST_STATE_NULL);
  //gst_object_unref (GST_OBJECT (pipeline));
};

void Pipeline::SetResolution(const int width, const int height) {
  GstCaps* filter_caps;
  filter_caps = gst_caps_new_simple ("video/x-raw",
         "width", G_TYPE_INT, width,
         "height", G_TYPE_INT, height,
         NULL);
  g_object_set(G_OBJECT (filter), "caps", filter_caps, NULL);
  gst_caps_unref(filter_caps);
};
