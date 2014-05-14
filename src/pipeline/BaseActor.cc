#include "BaseActor.h"

#include "GstConstants.h"
#include "../util/Log.h"

BaseActor::BaseActor(Actor* actor,
                      AssetDescriptor* asset_descriptor,
                      AssetQualityDescriptor* asset_quality_descriptor) {
  this->active = false;
  this->actor = actor;
  this->asset_descriptor = asset_descriptor;
  this->asset_quality_descriptor = asset_quality_descriptor;
  parent_pipeline = nullptr;
  enters_at_nano  = 0;
  offset_nano     = 0;
  duration_nano   = 0;
};

void BaseActor::SetActive(bool active) {
  this->active = active;
};

bool BaseActor::IsActive() {
  return this->active;
};

bool BaseActor::Prepare(Pipeline* pipeline, int seek_time) {
  // pure virtual.
};

bool BaseActor::Plug(Pipeline* pipeline) {
  // pure virtual.
};

bool BaseActor::Unplug() {
  // pure virtual.
};

std::string BaseActor::GetGstElementId(std::string base) {
  ss.str(std::string());
  ss << base << "_" << actor->id;
  return ss.str();
};

void BaseActor::CheckGstElements(std::vector<std::pair<std::string, GstElement*>> elements) {
  for (const auto& element_pair : elements) {
    if (!GST_IS_ELEMENT(element_pair.second))
      LOG_WARN("Could not initiate GstElement " << element_pair.first << " for actor " << actor->id,
               LOGGER_PIPELINE);
  }
};

void BaseActor::ClearTimeouts() {
  for(auto& id : timeout_ids) {
    g_source_remove(id);
  }
  timeout_ids.clear();
};

void BaseActor::SetTimelineProperties() {
  if (actor->enters_at >= 0) {
    enters_at_nano = actor->enters_at * GST_MSECOND;
  } else {
    enters_at_nano = 0;
  }
  // Get the original run for the whole clip.
  g_object_get (clip, "duration", &duration_nano, NULL);
  if (actor->offset > 0 && (actor->offset * GST_MSECOND) < duration_nano) {
    offset_nano = actor->offset * GST_MSECOND;
  } else {
    offset_nano = 0;
  }
  if (actor->duration >= 0 && (actor->duration * GST_MSECOND + offset_nano) <= duration_nano) {
    duration_nano = actor->duration * GST_MSECOND;
  } else if (actor->exits_at >= 0 &&
        ((actor->exits_at - actor->enters_at) * GST_MSECOND + offset_nano) <= duration_nano) {
    duration_nano = (actor->exits_at - actor->enters_at) * GST_MSECOND;
  }
  g_object_set(clip,
              "start", enters_at_nano,
              "in-point", offset_nano,
              "duration", duration_nano, NULL);
  LOG_TRACE("Actor " << actor->id << " enters at " << enters_at_nano, LOGGER_PIPELINE);
  LOG_TRACE("Actor " << actor->id << " offset " << offset_nano, LOGGER_PIPELINE);
  LOG_TRACE("Actor " << actor->id << " duration " << duration_nano, LOGGER_PIPELINE);
};
