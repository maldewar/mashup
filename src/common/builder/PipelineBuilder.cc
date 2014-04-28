#include "PipelineBuilder.h"

#include <sstream>
#include <gst/gst.h>

PipelineBuilder::PipelineBuilder() {};

Pipeline* PipelineBuilder::Build(const Mashup& mashup) {
  Pipeline* pipeline = new Pipeline();
  if (mashup.scene != nullptr) {
    pipeline->SetResolution(mashup.scene->width,
                            mashup.scene->height,
                            mashup.scene->framerate);
    BuildActors(mashup, *pipeline);
  }
  return pipeline;
};

int PipelineBuilder::BuildActors(const Mashup& mashup, Pipeline& pipeline) {
  if (mashup.scene != nullptr && mashup.assets.size() > 0) {
    std::stringstream ss;
    for(const auto& actor : mashup.scene->actors) {
      if (mashup.assets.count(actor->asset_id) > 0) {
        AssetDescriptor* asset_descriptor = mashup.assets.at(actor->asset_id);
        if (asset_descriptor->type == AssetDescriptor::Type::Video) {
          AssetQualityDescriptor* asset_quality_descriptor = asset_descriptor->qualities[0];
          ss << "vid_" << actor->asset_id << "_" << actor->instance_id;
          actor->id = ss.str();
          VideoActor* video_actor = new VideoActor(actor,
                                                   asset_descriptor,
                                                   asset_quality_descriptor);
          pipeline.m_video_actors[{actor->asset_id, actor->instance_id}] = video_actor;
        }
        if (asset_descriptor->type == AssetDescriptor::Type::Image) {
          AssetQualityDescriptor* asset_quality_descriptor = asset_descriptor->qualities[0];
          ss << "img_" << actor->asset_id << "_" << actor->instance_id;
          actor->id = ss.str();
          ImageActor* image_actor = new ImageActor(actor,
                                                   asset_descriptor,
                                                   asset_quality_descriptor);
          pipeline.m_image_actors[{actor->asset_id, actor->instance_id}] = image_actor;
        }
        ss.str(std::string());
      }
    }
  }
};
