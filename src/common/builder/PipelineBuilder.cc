#include "PipelineBuilder.h"

#include <sstream>
#include <gst/gst.h>

PipelineBuilder::PipelineBuilder() {};

Pipeline* PipelineBuilder::Build(const Mashup& mashup) {
  Pipeline* pipeline = new Pipeline();
  return pipeline;
};

int PipelineBuilder::BuildActors(const Mashup& mashup, Pipeline& pipeline) {
  if (mashup.scene != nullptr && mashup.assets.size() > 0) {
    std::stringstream ss;
    for(const auto& actor : mashup.scene->actors) {
      if (mashup.assets.count(actor->asset_id) > 0) {
        ss << actor->asset_id << "_" << actor->instance_id;
        actor->id = ss.str();
        AssetDescriptor* asset_descriptor = mashup.assets.at(actor->asset_id);
        AssetQualityDescriptor* asset_quality_descriptor = asset_descriptor->qualities[0];
        if (asset_descriptor->type == AssetDescriptor::Type::Video) {
          VideoActor* video_actor = new VideoActor(actor,
                                                   asset_descriptor,
                                                   asset_quality_descriptor);
          pipeline.m_video_actors[{actor->asset_id, actor->instance_id}] = video_actor;
        }
        ss.str(std::string());
      }
    }
  }
};
