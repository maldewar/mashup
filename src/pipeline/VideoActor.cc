#include "VideoActor.h"

#include <gst/base/gstbasesrc.h>

#include "GstConstants.h"
#include "../util/Math.h"
#include "../util/Log.h"

VideoActor::VideoActor(Actor* actor,
                      AssetDescriptor* asset_descriptor,
                      AssetQualityDescriptor* asset_quality_descriptor) :
            BaseActor(actor, asset_descriptor, asset_quality_descriptor) {
  plugged = false;
};

bool VideoActor::Prepare(int seek_time) {
  ClearTimeouts();
  SetGstElements();
  return true;
};

bool VideoActor::Plug(Pipeline* pipeline) {
  parent_pipeline = pipeline;
  if (actor->id.compare("") != 0) {
    GError* error = NULL;
    gchar* uri = gst_filename_to_uri(asset_quality_descriptor->composed_path.c_str(),
                                     NULL);
    asset = GES_ASSET(ges_uri_clip_asset_request_sync(uri, &error));
    if (error) {
      LOG_ERROR("Cannot load asset for uri " << uri << " for VideoActor id: " << actor->id << ". " << error->message,
                LOGGER_PIPELINE);
    } else {
      LOG_TRACE("Asset loaded for uri " << uri << " for VideoActor id: " << actor->id,
                LOGGER_PIPELINE);
    }
    ges_project_add_asset(parent_pipeline->project, asset);
    clip = GES_CLIP(ges_asset_extract(asset, &error));
    if (error) {
      LOG_ERROR("Cannot create clip for uri " << uri << " for VideoActor id: " << actor->id << ". " << error->message,
                LOGGER_PIPELINE);
    } else {
      LOG_TRACE("Clip created for uri " << uri << " for VideoActor id: " << actor->id,
                LOGGER_PIPELINE);
    }
    ges_layer_add_clip(parent_pipeline->layer, clip);
    video_track_element = ges_clip_find_track_element(clip, parent_pipeline->track_v, G_TYPE_NONE);
    audio_track_element = ges_clip_find_track_element(clip, parent_pipeline->track_a, G_TYPE_NONE);
    SetTimelineProperties();
    /*
    g_object_set (clip,
                  "start", actor->enters_at * GST_MSECOND,
                  "in-point", actor->offset * GST_MSECOND, NULL);
    */
    g_free(uri);
    if (error) {
      g_error_free(error);
    }
    plugged = true;
  }
  return true;
};

bool VideoActor::Unplug() {
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
  area_width_px = Math::RelativeToPx(width, parent_pipeline->width);
  area_height_px = Math::RelativeToPx(height, parent_pipeline->height);
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
  ges_track_element_set_child_properties(video_track_element,
                                     GST_PROP_WIDTH.c_str(), width_px,
                                     GST_PROP_HEIGHT.c_str(), height_px, NULL);
};

void VideoActor::SetX(double x) {
  this->x = x;
  if (actor->resize_mode == Actor::ResizeMode::AspectBorderTransparent) {
    ges_track_element_set_child_properties(video_track_element,
                 GST_PROP_X.c_str(),
                 Math::RelativeToPx(x, parent_pipeline->width) + ((area_width_px - width_px) / 2),
                 NULL);
  } else {
    ges_track_element_set_child_properties(video_track_element,
                 GST_PROP_X.c_str(),
                 Math::RelativeToPx(x, parent_pipeline->width),
                 NULL);
  }
};

void VideoActor::SetY(double y) {
  this->y = y;
  if (actor->resize_mode == Actor::ResizeMode::AspectBorderTransparent) {
    ges_track_element_set_child_properties(video_track_element,
                 GST_PROP_Y.c_str(),
                 Math::RelativeToPx(y, parent_pipeline->height) + ((area_height_px - height_px) / 2),
                 NULL);
  } else {
    ges_track_element_set_child_properties(video_track_element,
                 GST_PROP_Y.c_str(),
                 Math::RelativeToPx(y, parent_pipeline->height),
                 NULL);
  }
};

void VideoActor::SetZ(int z) {
  /*
  this->z = z;
  g_object_set(G_OBJECT(videomixer_pad),
                 "zorder",
                 z,
                 NULL);
  */
};

void VideoActor::SetAlpha(double alpha) {
  /*
  this->alpha = alpha;
  g_object_set(G_OBJECT(videomixer_pad),
                 "alpha",
                 alpha,
                 NULL);
  */
};
