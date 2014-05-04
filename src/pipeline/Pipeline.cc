#include "Pipeline.h"

#include "GstConstants.h"
#include "../util/Math.h"
#include "../util/Log.h"

Pipeline::Pipeline() {
  position_nano = 0;
  duration_nano = -1;
  pipeline     = gst_pipeline_new(id.c_str());
  loop         = g_main_loop_new(NULL, false);
  videomixer   = gst_element_factory_make("videomixer", "videomixer");
  videoconvert = gst_element_factory_make("videoconvert", "videoconvert");
  audiomixer   = gst_element_factory_make("adder", "audiomixer");
  filter       = gst_element_factory_make("capsfilter","filter");
  fakesrc      = gst_element_factory_make("fakesrc", "fakesrc");
  g_object_set(G_OBJECT(fakesrc), "format", GST_FORMAT_TIME, NULL);
  g_object_set(G_OBJECT(fakesrc), "is-live", true, NULL);
  fakesink     = gst_element_factory_make("fakesink", "fakesink");
};

void Pipeline::Prepare(int seek_time) {
  position_nano = Math::MilliToNano(seek_time);
  //GstState state;
  //gst_element_get_state(pipeline, &state, NULL, GST_CLOCK_TIME_NONE);
  /*gst_element_set_state(pipeline, GST_STATE_PAUSED);
  bool seek_result = gst_element_seek(pipeline,
                   1.0,
                   GST_FORMAT_TIME,
                   GST_SEEK_FLAG_FLUSH,
                   GST_SEEK_TYPE_SET,
                   position_nano,
                   GST_SEEK_TYPE_SET,
                   duration_nano - position_nano);
  //gst_element_set_state(pipeline, state);
  if (seek_result) {
    LOG_TRACE("SEEK METHOD SUCCESSFULL.", LOGGER_PIPELINE);
  } else {
    LOG_TRACE("SEEK_METHOD FAILED.", LOGGER_PIPELINE);
  }
    //gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_PLAYING);
  */
  for (const auto& video_actor_pair : m_video_actors) {
    video_actor_pair.second->Prepare(this, seek_time);
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
    gst_element_set_state(image_actor->bin, GST_STATE_PLAYING);
    gst_element_set_state(pipeline, GST_STATE_PLAYING);
  }
};

void Pipeline::Play() {
  GstElement *e_videotestsrc;
  GstElement *e_audiotestsrc;
  GstElement *e_autovideosink;
  GstElement *e_autoaudiosink;
  guint bus_watch_id;
  e_autovideosink = gst_element_factory_make("autovideosink", "autovideosink");
  e_autoaudiosink = gst_element_factory_make("autoaudiosink", "autoaudiosink");
  gst_bin_add_many(GST_BIN(pipeline), fakesrc, fakesink, NULL);
  gst_bin_add_many(GST_BIN(pipeline), videomixer, videoconvert, filter, e_autovideosink, NULL);
  gst_bin_add_many(GST_BIN(pipeline), audiomixer, e_autoaudiosink, NULL);
  gst_element_link_many(fakesrc, fakesink, NULL);
  gst_element_link_many(videomixer, videoconvert, filter, e_autovideosink, NULL);
  gst_element_link_many(audiomixer, e_autoaudiosink, NULL);
  //gst_element_set_start_time(GST_ELEMENT(pipeline), GST_CLOCK_TIME_NONE);
  Prepare();

  bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
  bus_watch_id = gst_bus_add_watch(bus, Pipeline::BusCall, this);
  g_timeout_add(1000/this->framerate, Pipeline::TimeoutBusCall, this);
  gst_object_unref (bus);

  LOG_TRACE("SETTING PIPELINE TO PLAY.", LOGGER_PIPELINE);
  GstStateChangeReturn st =
  gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_PLAYING);
  LOG_TRACE("PIPELINE STATE SET: " << st, LOGGER_PIPELINE);
  g_main_loop_run (loop);
  LOG_TRACE("Running main Loop", LOGGER_PIPELINE);
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
         //GST_PROP_FRAMERATE.c_str(), GST_TYPE_FRACTION, framerate, 1, //TODO: research
         NULL);
  g_object_set(G_OBJECT (filter), "caps", filter_caps, NULL);
  gst_caps_unref(filter_caps);
  LOG_TRACE("Setting framerate to:" << framerate, LOGGER_PIPELINE);
};

void Pipeline::SetDuration(int duration) {
  duration_nano = Math::MilliToNano(duration);
};

long int Pipeline::GetPosition() {
  gst_element_query_position(pipeline, GST_FORMAT_TIME, &position_nano);
  return position_nano;
};

long int Pipeline::GetDuration() {
  gst_element_query_duration(pipeline, GST_FORMAT_TIME, &duration_nano);
  return duration_nano;
};

int Pipeline::BusCall(GstBus* bus,
                      GstMessage* msg,
                      void* pipeline_obj)
{
  switch (GST_MESSAGE_TYPE (msg)) {

    case GST_MESSAGE_EOS: {
      //g_print ("End of stream\n");
      //g_main_loop_quit (loop);
      LOG_TRACE("GST_MESSAGE_EOS", LOGGER_PIPELINE);
      break;
    }

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

    case GST_MESSAGE_APPLICATION: {
      if (gst_message_has_name(msg, "video_actor_plug")) {
        const GstStructure* structure = gst_message_get_structure(msg);
        VideoActor* video_actor;
        gst_structure_get(structure, "video_actor", G_TYPE_POINTER, &video_actor, NULL);
        gst_element_seek (video_actor->decodebin,
            1.0, GST_FORMAT_TIME,
            (GstSeekFlags)(GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_ACCURATE),
            GST_SEEK_TYPE_SET, 2 * GST_SECOND,
            GST_SEEK_TYPE_SET, 5 * GST_SECOND);
        //gst_element_set_state(video_actor->bin, GST_STATE_PLAYING);
        if (video_actor->IsActive()) {
          video_actor->Plug();
          LOG_TRACE("EVENT VIDEO ACTOR PLUG: " << video_actor->actor->id, LOGGER_PIPELINE);
        }
      }
      break;
    }

    case GST_MESSAGE_ASYNC_DONE: {
      LOG_TRACE("ASYNC DONE.", LOGGER_PIPELINE);
      //Pipeline* pipeline = static_cast<Pipeline*>(pipeline_obj);
      //gst_element_set_state(GST_ELEMENT(pipeline_obj), GST_STATE_PLAYING);
      break;
    }

    default:
      break;
  }

  return TRUE;
}

int Pipeline::TimeoutBusCall(void* pipeline_obj) {
  Pipeline* pipeline = static_cast<Pipeline*>(pipeline_obj);
  GstState st;
  gst_element_get_state(pipeline->pipeline, &st, 0, 0);
  std::string st_str;
  switch(st) {
    case GST_STATE_VOID_PENDING:
      st_str = "VOID PENDING";
      break;
    case GST_STATE_NULL:
      st_str = "NULL";
      break;
    case GST_STATE_READY:
      st_str = "READY";
      break;
    case GST_STATE_PAUSED:
      st_str = "PAUSED";
      break;
    case GST_STATE_PLAYING:
      st_str = "PLAYING";
      break;
  }
  LOG_TRACE("Pipeline position: " << pipeline->GetPosition() << " duration: " << pipeline->GetDuration() << " state: " << st_str << " state_num: " << st, LOGGER_PIPELINE);
  return TRUE;
};
/*
gst-launch-1.0 -v uridecodebin uri=file:///home/waldemar/Projects/mashup/test/assets/hb/transparent.png ! imagefreeze ! videomixer name=m sink_0::zorder=0 ! videoconvert ! autovideosink videotestsrc ! videoscale ! video/x-raw, width=640, height=400 ! m.
*/
