#include "BaseActor.h"

#include "GstConstants.h"
#include "../util/Log.h"

BaseActor::BaseActor(Actor* actor,
                      AssetDescriptor* asset_descriptor,
                      AssetQualityDescriptor* asset_quality_descriptor) {
  this->actor = actor;
  this->asset_descriptor = asset_descriptor;
  this->asset_quality_descriptor = asset_quality_descriptor;
  video_src_pad = nullptr;
  audio_src_pad = nullptr;
  videomixer_pad = nullptr;
  audiomixer_pad = nullptr;
};

std::string BaseActor::GetGstElementId(std::string base) {
  ss.str(std::string());
  ss << base << "_" << actor->id;
  return ss.str().c_str();
};

void BaseActor::CheckGstElements(std::vector<std::pair<std::string, GstElement*>> elements) {
  for (const auto& element_pair : elements) {
    if (!GST_IS_ELEMENT(element_pair.second))
      LOG_WARN("Could not initiate GstElement " << element_pair.first << " for actor " << actor->id,
               LOGGER_PIPELINE);
  }
};
