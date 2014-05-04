#include "VideoActor.h"

#include <gst/base/gstbasesrc.h>

#include "GstConstants.h"
#include "../util/Math.h"
#include "../util/Log.h"

VideoActor::VideoActor(Actor* actor,
                      AssetDescriptor* asset_descriptor,
                      AssetQualityDescriptor* asset_quality_descriptor) :
            BaseActor(actor, asset_descriptor, asset_quality_descriptor) {
  scene_width  = 0;
  scene_height = 0;
  if (actor->id.compare("") != 0) {
    bin          = gst_bin_new(GetGstElementId(GST_ELEM_BIN).c_str());
    filesrc      = gst_element_factory_make(GST_ELEM_FILESRC.c_str(),
                                            GetGstElementId(GST_ELEM_FILESRC).c_str());
    //gst_base_src_set_live((GstBaseSrc*)filesrc, true);
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
    bool seek_done = false;
    seek_done = gst_element_seek(decodebin,
                   1.0,
                   GST_FORMAT_TIME,
                   (GstSeekFlags)(GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_ACCURATE | GST_SEEK_FLAG_SEGMENT),
                   GST_SEEK_TYPE_SET,
                   0 * GST_SECOND,
                   GST_SEEK_TYPE_NONE,
                   GST_CLOCK_TIME_NONE);
    if (seek_done) {
      LOG_TRACE("SEEK EVENT SUCCESFUL ON INIT FOR " << actor->id, LOGGER_PIPELINE);
    } else {
      LOG_TRACE("SEEK EVENT FAILED ON INIT FOR " << actor->id, LOGGER_PIPELINE);
    }

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
    GstPad* last_video_src_pad = gst_element_get_static_pad (filter, M_GST_PAD_SRC.c_str());
    if (gst_element_add_pad(this->bin,
          gst_ghost_pad_new(M_GST_PAD_VIDEO_SRC.c_str(), last_video_src_pad))) {
      video_src_pad = gst_element_get_static_pad(this->bin, M_GST_PAD_VIDEO_SRC.c_str());
      gst_pad_set_active(video_src_pad, true);
    }
    gst_object_unref(GST_OBJECT (last_video_src_pad));

    // Audio Src to Bin
    GstPad* last_audio_src_pad = gst_element_get_static_pad (volume, M_GST_PAD_SRC.c_str());
    if (gst_element_add_pad(this->bin,
          gst_ghost_pad_new(M_GST_PAD_AUDIO_SRC.c_str(), last_audio_src_pad))) {
      audio_src_pad = gst_element_get_static_pad(this->bin, M_GST_PAD_AUDIO_SRC.c_str());
      gst_pad_set_active(audio_src_pad, true);
    }
    gst_object_unref(GST_OBJECT (last_audio_src_pad));

    LOG_TRACE("Setting to paused decodebin from actor: " << this->actor->id, LOGGER_PIPELINE);

    gst_element_set_state (this->decodebin, GST_STATE_PAUSED);
    gst_element_set_state (this->bin, GST_STATE_PAUSED);
  }
};

bool VideoActor::Prepare(Pipeline* pipeline, int seek_time) {
  ClearTimeouts();
  parent_pipeline = pipeline;
  if (actor->enters_at <= seek_time) {
    Plug();
  } else {
    gst_element_set_state (this->bin, GST_STATE_PAUSED);
    long int timeout_id = g_timeout_add(actor->enters_at - seek_time,
                                        VideoActor::PlugTimeout,
                                        this);
    /*
    g_timeout_add((actor->enters_at - seek_time)/2,
                                        VideoActor::SeekTimeout,
                                        this);
    */
  }
  return true;
};

bool VideoActor::Plug() {
  // GstPad* video_src_pad;
  // GstPad* audio_src_pad;
  GstPad* video_sink_pad;
  GstPad* audio_sink_pad;
  //gst_element_set_base_time (this->bin, 6 * GST_SECOND);
  //gst_element_set_start_time (this->bin, 6 * GST_SECOND);
  GstState bin_state;
  GstState bin_pend;
  gst_element_get_state(this->bin, &bin_state, &bin_pend, 0);
  LOG_TRACE("CURRENT VIDEO ACTOR " << actor->id << " BIN STATE IS " << bin_state << " PEND IS " << bin_pend, LOGGER_PIPELINE);
  bool seek_done = false;
  seek_done = gst_element_seek(this->bin,
                   1.0,
                   GST_FORMAT_TIME,
                   (GstSeekFlags)(GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_ACCURATE | GST_SEEK_FLAG_SEGMENT),
                   GST_SEEK_TYPE_SET,
                   0 * GST_SECOND,
                   GST_SEEK_TYPE_NONE,
                   GST_CLOCK_TIME_NONE);
  /*
  GstMessage *msg; 
  msg = gst_bus_timed_pop_filtered (GST_ELEMENT_BUS (this->bin), 
      GST_CLOCK_TIME_NONE, 
      (GstMessageType)(GST_MESSAGE_ERROR | GST_MESSAGE_ASYNC_DONE));
  gst_message_unref (msg);
  */
  if (seek_done) {
    LOG_TRACE("SEEK DONE ON VIDEO ACTOR " << actor->id, LOGGER_PIPELINE);
   } else {
    LOG_TRACE("SEEK FAILED ON VIDEO ACTOR " << actor->id, LOGGER_PIPELINE);
  }
  //gst_bin_add(GST_BIN(parent_pipeline->pipeline), this->bin);
  //gst_element_set_state(this->bin, GST_STATE_PLAYING);
  //video_src_pad  = gst_element_get_static_pad(this->bin, M_GST_PAD_VIDEO_SRC.c_str());
  //audio_src_pad  = gst_element_get_static_pad(this->bin, M_GST_PAD_AUDIO_SRC.c_str());

  video_sink_pad = gst_element_get_request_pad(parent_pipeline->videomixer,
                                              M_GST_PAD_MULTI_SINK.c_str());
  /*
  if (video_sink_pad) {
    LOG_TRACE("VIDEO SINK PAD SUCCESSFULLY REQUESTED.", LOGGER_PIPELINE);
  }
  */
  //LOG_TRACE("SETTING BLOCK COUNT TO 2 FOR " << this->actor->id, LOGGER_PIPELINE);
  //block_count = 2;
  /*gst_pad_add_probe(video_src_pad,
                    GST_PAD_PROBE_TYPE_BLOCK,
                    (GstPadProbeCallback)VideoActor::OnBlockedPad,
                    this, NULL);*/
  //LOG_TRACE("BLOCKING VIDEO PAD " << this->actor->id, LOGGER_PIPELINE);
  //gst_pad_add_probe(video_src_pad, GST_PAD_PROBE_TYPE_BLOCK_DOWNSTREAM, NULL, NULL, NULL);
  audio_sink_pad = gst_element_get_request_pad(parent_pipeline->audiomixer,
                                              M_GST_PAD_MULTI_SINK.c_str());
  /*
  if (prerolled) {
    gst_pad_set_offset (video_src_pad, 5 * GST_SECOND);
    gst_pad_set_offset (audio_src_pad, 5 * GST_SECOND);
  }*/
  /*
  if (prerolled) {
    gst_element_set_start_time(this->bin, -5 * GST_SECOND);
  }
  */
  /*gst_pad_add_probe(audio_src_pad,
                    GST_PAD_PROBE_TYPE_BLOCK,
                    (GstPadProbeCallback)VideoActor::OnBlockedPad,
                    this, NULL);*/
  //LOG_TRACE("BLOCKING AUDIO PAD " << this->actor->id, LOGGER_PIPELINE);
  //gst_element_set_state(this->bin, GST_STATE_PAUSED);
  /*gst_element_seek(this->bin,
                   1.0,
                   GST_FORMAT_TIME,
                   GST_SEEK_FLAG_FLUSH,
                   GST_SEEK_TYPE_SET,
                   0,
                   GST_SEEK_TYPE_NONE,
                   GST_CLOCK_TIME_NONE);*/
  //gst_element_set_state(this->bin, GST_STATE_PLAYING);
  //gst_pad_add_probe(audio_src_pad, GST_PAD_PROBE_TYPE_BLOCK_DOWNSTREAM, NULL, NULL, NULL);

  //GstStateChangeReturn st =
  //gst_element_set_state (this->bin, GST_STATE_PAUSED);
  //LOG_TRACE("ELEMENT STATE SET: " << st, LOGGER_PIPELINE);
  //gst_element_set_start_time(this->decodebin, 5 * GST_SECOND);
  gst_bin_add(GST_BIN(parent_pipeline->pipeline), this->bin);
  gst_element_sync_state_with_parent(this->bin);
  //gst_base_sink_set_ts_offset(static_cast<GstBaseSink*>(video_sink_pad), 6 * GST_SECOND);
  //gst_base_sink_set_ts_offset((GstBaseSink*)audio_sink_pad, 6 * GST_SECOND);
  //gst_base_sink_set_render_delay((GstBaseSink*)video_sink_pad, 6 * GST_SECOND);
  gst_pad_link(video_src_pad, video_sink_pad);
  gst_pad_link(audio_src_pad, audio_sink_pad);
  //gst_element_set_state (this->bin, GST_STATE_PLAYING);
  /*gst_element_seek(this->bin,
                   1.0,
                   GST_FORMAT_TIME,
                   GST_SEEK_FLAG_FLUSH,
                   GST_SEEK_TYPE_SET,
                   0 * GST_SECOND,
                   GST_SEEK_TYPE_NONE,
                   GST_CLOCK_TIME_NONE);*/
  //gst_element_sync_state_with_parent(this->bin);
  this->videomixer_pad = video_sink_pad;
  this->audiomixer_pad = audio_sink_pad;
  gst_object_unref(GST_OBJECT(video_src_pad));
  gst_object_unref(GST_OBJECT(audio_src_pad));
  SetGstElements();
  //gst_element_set_base_time (this->decodebin, 6 * GST_SECOND);
  //gst_element_set_start_time (this->decodebin, 6 * GST_SECOND);
  //gst_element_set_state(parent_pipeline->pipeline, GST_STATE_PLAYING);
  //GstStateChangeReturn st =
  //gst_element_set_state (this->bin, GST_STATE_PAUSED);
  //LOG_TRACE("ELEMENT STATE SET: " << st, LOGGER_PIPELINE);
  //if (gst_lement_sync_state_with_parent(this->bin))
  //  LOG_TRACE("ELEMENT SET TO PARENT STATE", LOGGER_PIPELINE);
  active = true;
  return true;
};

bool VideoActor::Unplug() {
  if (active) {
    active = false;
    // TODO: send EOS to bin and unlink from parent pipeline.
    return true;
  }
  return false;
};

int VideoActor::PlugTimeout(void* obj) {
  VideoActor* video_actor = static_cast<VideoActor*>(obj);
  //gst_pad_set_offset(video_actor->video_src_pad, 5 * GST_SECOND);
  //gst_pad_set_offset(video_actor->audio_src_pad, 5 * GST_SECOND);
  /*
  gst_element_seek(video_actor->bin,
                   1.0,
                   GST_FORMAT_TIME,
                   GST_SEEK_FLAG_FLUSH,
                   GST_SEEK_TYPE_SET,
                   0,
                   GST_SEEK_TYPE_NONE,
                   GST_CLOCK_TIME_NONE);*/
  video_actor->prerolled = true;
  video_actor->Plug();
  LOG_TRACE("Plugging VideoActor with ID: " << video_actor->actor->id, LOGGER_PIPELINE);
  return false;
};

int VideoActor::SeekTimeout(void* obj) {
  VideoActor* video_actor = static_cast<VideoActor*>(obj);
  bool seek_done = gst_element_seek(video_actor->decodebin,
                   1.0,
                   GST_FORMAT_TIME,
                   GST_SEEK_FLAG_FLUSH,
                   GST_SEEK_TYPE_SET,
                   0,
                   GST_SEEK_TYPE_NONE,
                   GST_CLOCK_TIME_NONE);
  //video_actor->Plug();
  LOG_TRACE("PERFORMING SEEK ON " << video_actor->actor->id, LOGGER_PIPELINE);
  if (seek_done) {
    LOG_TRACE("  SEEK SUCCESSFUL.", LOGGER_PIPELINE);
  } else {
    LOG_TRACE("  SEEK FAILED.", LOGGER_PIPELINE);
  }
  return false;
};

int VideoActor::UnplugTimeout(void* obj) {
  VideoActor* video_actor = static_cast<VideoActor*>(obj);
  LOG_TRACE("Unplugging VideoActor with ID: " << video_actor->actor->id, LOGGER_PIPELINE);
  return false;
};

void VideoActor::SetGstElements() {
  SetDimensions(actor->width, actor->height);
  SetX(actor->x);
  SetY(actor->y);
  SetZ(actor->z);
};

void VideoActor::SetDimensions(double width, double height) {
  this->width = width;
  this->height = height;
  GstCaps* filter_caps;
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
                 Math::RelativeToPx(x, scene_width) + ((area_width_px - width_px) / 2),
                 NULL);
  } else {
    g_object_set(G_OBJECT(videomixer_pad),
                 "xpos",
                 Math::RelativeToPx(x, scene_width),
                 NULL);
  }
};

void VideoActor::SetY(double y) {
  this->y = y;
  if (actor->resize_mode == Actor::ResizeMode::AspectBorderTransparent) {
    g_object_set(G_OBJECT(videomixer_pad),
                 "ypos",
                 Math::RelativeToPx(y, scene_height) + ((area_height_px - height_px) / 2),
                 NULL);
  } else {
    g_object_set(G_OBJECT(videomixer_pad),
                 "ypos",
                 Math::RelativeToPx(y, scene_height),
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
    LOG_TRACE("Adding Video Decodebin Pad. " << video_actor->actor->id, LOGGER_PIPELINE);
  }
  if (g_str_has_prefix(new_pad_type, "audio/x-raw")) {
    sink_pad = gst_element_get_static_pad(video_actor->audioconvert, "sink");
    LOG_TRACE("Adding Audio Decodebin Pad. " << video_actor->actor->id, LOGGER_PIPELINE);
  }
  if (!gst_pad_is_linked(sink_pad)) {
    ret = gst_pad_link(pad, sink_pad);
    gst_element_sync_state_with_parent(decodebin);
  }/* else {
    gst_element_sync_state_with_parent(decodebin);
    //gst_element_set_state(video_actor->bin, GST_STATE_READY);
  }*/
  gst_caps_unref(new_pad_caps);
  gst_object_unref(sink_pad);
};

int VideoActor::OnBlockedPad(GstPad*          pad,
                            GstPadProbeInfo* info,
                            VideoActor*      video_actor) {
  LOG_TRACE("INFO->TYPE: " << info->type << " block: " << GST_PAD_PROBE_TYPE_BLOCK, LOGGER_PIPELINE);
  //if (info->type == GST_PAD_PROBE_TYPE_BLOCK) {
    LOG_TRACE("PAD IS BLOCKED FOR " << video_actor->actor->id, LOGGER_PIPELINE);
    video_actor->block_count--;
    LOG_TRACE(" COUNT FOR BLOCK IS: " << video_actor->block_count, LOGGER_PIPELINE);
    if (video_actor->block_count == 0) {
      gst_bin_add(GST_BIN(video_actor->parent_pipeline->pipeline), video_actor->bin);
      GstPad* video_src_pad;
      GstPad* audio_src_pad;
      GstPad* video_sink_pad;
      GstPad* audio_sink_pad;
      video_src_pad  = gst_element_get_static_pad(video_actor->bin, M_GST_PAD_VIDEO_SRC.c_str());
      audio_src_pad  = gst_element_get_static_pad(video_actor->bin, M_GST_PAD_AUDIO_SRC.c_str());
      video_sink_pad = gst_element_get_request_pad(video_actor->parent_pipeline->videomixer,
                                                M_GST_PAD_MULTI_SINK.c_str());
      audio_sink_pad = gst_element_get_request_pad(video_actor->parent_pipeline->audiomixer,
                                                M_GST_PAD_MULTI_SINK.c_str());
      video_actor->videomixer_pad = video_sink_pad;
      video_actor->audiomixer_pad = audio_sink_pad;
      gst_pad_link(video_src_pad, video_sink_pad);
      gst_pad_link(audio_src_pad, audio_sink_pad);
      video_actor->SetGstElements();
      //gst_element_set_state (video_actor->parent_pipeline->pipeline, GST_STATE_PLAYING);
      gst_element_sync_state_with_parent(video_actor->bin);
      LOG_TRACE("SETTING VIDEO ACTOR BIN TO PLAY.", LOGGER_PIPELINE);
    }
  //}
  return GST_PAD_PROBE_REMOVE;
};
