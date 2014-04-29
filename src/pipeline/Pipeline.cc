#include "Pipeline.h"

#include "GstConstants.h"
#include "../util/Log.h"

Pipeline::Pipeline() {
  position     = 0;
  duration     = 0;
  pipeline     = gst_pipeline_new(id.c_str());
  loop         = g_main_loop_new(NULL, false);
  videomixer   = gst_element_factory_make("videomixer", "videomixer");
  videoconvert = gst_element_factory_make("videoconvert", "videoconvert");
  audiomixer   = gst_element_factory_make("adder", "audiomixer");
  filter       = gst_element_factory_make("capsfilter","filter");
};

void Pipeline::Prepare(int seek_time) {
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
    gint64 queued;
    guint in_width, in_height;
    gint fps_n;
    gint fps_d;
    gint xpos, ypos;
    guint zorder;
    gint blend_mode;
    gdouble alpha;
    */
    gst_object_unref(GST_OBJECT(video_src_pad));
    gst_object_unref(GST_OBJECT(audio_src_pad));
    video_actor->SetGstElements();
  }
  for (const auto& image_actor_pair : m_image_actors) {
    GstPad* video_src_pad;
    GstPad* video_sink_pad;
    ImageActor* image_actor = image_actor_pair.second;
    gst_bin_add(GST_BIN(pipeline), image_actor->bin);
    video_src_pad = gst_element_get_static_pad(image_actor->bin, M_GST_PAD_VIDEO_SRC.c_str());
    video_sink_pad = gst_element_get_request_pad(videomixer, M_GST_PAD_MULTI_SINK.c_str());
    gst_pad_link(video_src_pad, video_sink_pad);
    image_actor->videomixer_pad = video_sink_pad;
    gst_object_unref(GST_OBJECT(video_src_pad));
    image_actor->SetGstElements();
  }
};

void Pipeline::Play() {
  GstElement *e_videotestsrc;
  GstElement *e_audiotestsrc;
  GstElement *e_autovideosink;
  GstElement *e_autoaudiosink;
  GstBus* bus;
  guint bus_watch_id;
  e_autovideosink = gst_element_factory_make("autovideosink", "autovideosink");
  e_autoaudiosink = gst_element_factory_make("autoaudiosink", "autoaudiosink");
  gst_bin_add_many(GST_BIN(pipeline), videomixer, videoconvert, filter, e_autovideosink, NULL);
  gst_bin_add_many(GST_BIN(pipeline), audiomixer, e_autoaudiosink, NULL);
  gst_element_link_many(videomixer, videoconvert, filter, e_autovideosink, NULL);
  gst_element_link_many(audiomixer, e_autoaudiosink, NULL);
  Prepare();

  bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
  bus_watch_id = gst_bus_add_watch(bus, Pipeline::BusCall, this);
  g_timeout_add(1000/this->framerate, Pipeline::TimeoutBusCall, this);
  gst_object_unref (bus);

  gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_PLAYING);
  g_main_loop_run (loop);
  //gst_element_set_state (pipeline, GST_STATE_NULL);
  //gst_object_unref (GST_OBJECT (pipeline));
};

void Pipeline::SetResolution(const unsigned int width,
                             const unsigned int height,
                             const unsigned int framerate) {
  this->width     = width;
  this->height    = height;
  this->framerate = framerate;
  GstCaps* filter_caps;
  filter_caps = gst_caps_new_simple ("video/x-raw",
         GST_PROP_WIDTH.c_str(), G_TYPE_INT, width,
         GST_PROP_HEIGHT.c_str(), G_TYPE_INT, height,
         GST_PROP_FRAMERATE.c_str(), GST_TYPE_FRACTION, framerate, 1,
         NULL);
  g_object_set(G_OBJECT (filter), "caps", filter_caps, NULL);
  gst_caps_unref(filter_caps);
  LOG_TRACE("Setting framerate to:" << framerate, LOGGER_PIPELINE);
};

long int Pipeline::GetPosition() {
  gst_element_query_position(pipeline, GST_FORMAT_TIME, &position);
  return position;
};

long int Pipeline::GetDuration() {
  gst_element_query_duration(pipeline, GST_FORMAT_TIME, &duration);
  return duration;
};

int Pipeline::BusCall(GstBus* bus,
                      GstMessage* msg,
                      void* pipeline_obj)
{
  switch (GST_MESSAGE_TYPE (msg)) {

    case GST_MESSAGE_EOS:
      //g_print ("End of stream\n");
      //g_main_loop_quit (loop);
      LOG_TRACE("GST_MESSAGE_EOS", LOGGER_PIPELINE);
      break;

    case GST_MESSAGE_ERROR: {
      Pipeline* pipeline = static_cast<Pipeline*>(pipeline_obj);
      gchar  *debug;
      GError *error;

      gst_message_parse_error (msg, &error, &debug);
      g_free (debug);

      g_printerr ("Error: %s\n", error->message);
      g_error_free (error);

      g_main_loop_quit (pipeline->loop);
      break;
    }
    default:
      break;
  }

  return TRUE;
}

int Pipeline::TimeoutBusCall(void* pipeline_obj) {
  Pipeline* pipeline = static_cast<Pipeline*>(pipeline_obj);
  LOG_TRACE("Pipeline position: " << pipeline->GetPosition() << " duration: " << pipeline->GetDuration(), LOGGER_PIPELINE);
  return TRUE;
};
/*
gst-launch-1.0 -v uridecodebin uri=file:///home/waldemar/Projects/mashup/test/assets/hb/transparent.png ! imagefreeze ! videomixer name=m sink_0::zorder=0 ! videoconvert ! autovideosink videotestsrc ! videoscale ! video/x-raw, width=640, height=400 ! m.
*/
