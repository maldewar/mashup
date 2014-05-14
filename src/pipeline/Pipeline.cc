#include "Pipeline.h"

//#include <gst/base/gstbasesink.h>

#include "GstConstants.h"
#include "../util/Math.h"
#include "../util/Log.h"

Pipeline::Pipeline() {
  position_nano = 0;
  duration_nano = -1;

  pipeline = gst_pipeline_new(id.c_str());
  project  = ges_project_new(id.c_str());
  filter   = gst_element_factory_make("capsfilter","filter");

  // TODO: Plugable sink.
  autovideosink = gst_element_factory_make("autovideosink", "autovideosink");
  autoaudiosink = gst_element_factory_make("autoaudiosink", "autoaudiosink");
  audioqueue    = gst_element_factory_make("queue", "audioqueue");
  videoqueue    = gst_element_factory_make("queue", "videoqueue");

  // Create GES Elements.
  timeline = GES_TIMELINE (ges_asset_extract (GES_ASSET (project), NULL));
  track_a  = GES_TRACK (ges_audio_track_new ());
  track_v  = GES_TRACK (ges_video_track_new ());
  layer = GES_LAYER (ges_layer_new ());
  g_object_set (layer, "priority", 1, NULL);

  if (!ges_timeline_add_layer (timeline, layer) ||
      !ges_timeline_add_track (timeline, track_a) ||
      !ges_timeline_add_track (timeline, track_v)) {
    LOG_ERROR("Can not add GES elements in Pipeline id: " << id, LOGGER_PIPELINE);
  } else {
    LOG_TRACE("GES elements added correctly to Pipeline id: " << id, LOGGER_PIPELINE);
  }

  // Link GStreamer elements.
  gst_bin_add_many(GST_BIN(pipeline),
                   GST_ELEMENT(timeline),
                   audioqueue,
                   autoaudiosink,
                   filter,
                   videoqueue,
                   autovideosink,
                   NULL);
  g_signal_connect (timeline,
                    "pad-added",
                    (GCallback) Pipeline::TimelinePadAddedCallback,
                    this);
  gst_element_link_many(filter, videoqueue, autovideosink, NULL);
  gst_element_link_many(audioqueue, autoaudiosink, NULL);
  bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
  bus_watch_id = gst_bus_add_watch(bus, Pipeline::BusCall, this);
  gst_object_unref (bus);
};

void Pipeline::asset_loaded_cb(GObject * source_object, GAsyncResult * res, Pipeline* pipeline) {
  LOG_TRACE("PIPELINE ASSET LOADED CALLBACK", LOGGER_PIPELINE);
  GError *error = NULL;

  GESUriClipAsset *mfs =
      GES_URI_CLIP_ASSET (ges_asset_request_finish (res, &error));

  if (error) {
    LOG_TRACE("error creating asset " << error->message, LOGGER_PIPELINE);

    return;
  }
/*
  GESUriClipAsset *mfs =
      GES_URI_CLIP_ASSET (ges_asset_request_finish (res, NULL));
*/
  GstDiscovererInfo *discoverer_info = NULL;
  discoverer_info = ges_uri_clip_asset_get_info (mfs);
  LOG_TRACE("RESULT IS " << gst_discoverer_info_get_result (discoverer_info) << ", INFOTYPE IS " << G_OBJECT_TYPE_NAME (mfs) << ", DURATION IS " << ges_uri_clip_asset_get_duration (mfs), LOGGER_PIPELINE);


  gst_object_unref (mfs);
  /*
  assetsLoaded++;
  if (assetsLoaded == assetsCount) {
    GstDiscovererInfo *info = ges_uri_clip_asset_get_info (mfs);
    GstEncodingProfile *profile = make_profile_from_info (info);
    ges_pipeline_set_render_settings (pipeline, output_uri, profile);
    if (!ges_pipeline_set_mode (pipeline, GES_PIPELINE_MODE_SMART_RENDER)) {
      g_main_loop_quit (mainloop);
      return;
    }
    gst_element_set_state (GST_ELEMENT (pipeline), GST_STATE_PLAYING);
  }

  gst_object_unref (mfs);
  */
};

void Pipeline::Prepare(int seek_time) {
  for (const auto& video_actor_pair : m_video_actors) {
    if (!video_actor_pair.second->plugged) {
      video_actor_pair.second->Plug(this);
    }
    video_actor_pair.second->Prepare(seek_time);
  }
  for (const auto& image_actor_pair : m_image_actors) {
  }
  /*
  position_nano = Math::MilliToNano(seek_time);
  bool seek_result = gst_element_seek(GST_ELEMENT(timeline),
                   1.0,
                   GST_FORMAT_TIME,
                   GST_SEEK_FLAG_FLUSH,
                   GST_SEEK_TYPE_SET,
                   position_nano,
                   GST_SEEK_TYPE_SET,
                   -1);
  if (seek_result) {
    LOG_TRACE("SEEK METHOD SUCCESSFULL.", LOGGER_PIPELINE);
  } else {
    LOG_TRACE("SEEK_METHOD FAILED.", LOGGER_PIPELINE);
  }
  */
};

void Pipeline::Play() {
  Prepare();
  g_timeout_add(1000/this->framerate, Pipeline::TimeoutBusCall, this);
  gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_PLAYING);
  loop = g_main_loop_new(NULL, false);
  g_main_loop_run (loop);
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

long int Pipeline::GetPosition() {
  gst_element_query_position(GST_ELEMENT(pipeline), GST_FORMAT_TIME, &position_nano);
  return position_nano;
};

long int Pipeline::GetDuration() {
  gst_element_query_duration(GST_ELEMENT(pipeline), GST_FORMAT_TIME, &duration_nano);
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
      /*
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
      }*/
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
  gst_element_get_state(GST_ELEMENT(pipeline->pipeline), &st, 0, 0);
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

void Pipeline::TimelinePadAddedCallback(GstElement* timeline,
                                        GstPad* pad,
                                        Pipeline* pipeline) {
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
    sink_pad = gst_element_get_static_pad(pipeline->filter, "sink");
    LOG_TRACE("Adding video pad for timeline in Pipeline id: " << pipeline->id,
              LOGGER_PIPELINE);
  }
  if (g_str_has_prefix(new_pad_type, "audio/x-raw")) {
    sink_pad = gst_element_get_static_pad(pipeline->audioqueue, "sink");
    LOG_TRACE("Adding audio pad for timeline in Pipeline id: " << pipeline->id,
              LOGGER_PIPELINE);
  }

  if (!gst_pad_is_linked(sink_pad)) {
    ret = gst_pad_link(pad, sink_pad);
    gst_element_sync_state_with_parent(timeline);
  }
  gst_caps_unref(new_pad_caps);
  gst_object_unref(sink_pad);
};
