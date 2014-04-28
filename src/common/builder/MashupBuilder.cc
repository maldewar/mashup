#include "MashupBuilder.h"

#include <utility>

#include "../../util/Log.h"
#include "../BaseProperty.h"
#include "ValueBuilder.h"
#include "PathBuilder.h"

namespace pt = boost::property_tree;

MashupBuilder::MashupBuilder() {
  assets_path = "";
};

int MashupBuilder::Build(Mashup& mashup,
                         const boost::property_tree::ptree& p_tree) {
  mashup.id = p_tree.get<long int>("id", 0);
  mashup.title = p_tree.get<std::string>("title", "");
  mashup.version = p_tree.get<std::string>("version","");
  mashup.creator_id = p_tree.get<long int>("creatorId");
  LOG_TRACE("ID: " << mashup.id, LOGGER_BUILDER);
  LOG_TRACE("Title: " << mashup.title, LOGGER_BUILDER);
  LOG_TRACE("Version: " << mashup.version, LOGGER_BUILDER);
  LOG_TRACE("Creator ID: " << mashup.creator_id, LOGGER_BUILDER);
  if (boost::optional<const pt::ptree&> p_assets = 
      p_tree.get_child_optional("assets")) {
    AppendAssetDescriptors(mashup, p_assets.get());
  }
  if (boost::optional<const pt::ptree&> p_scene =
      p_tree.get_child_optional("scene")) {
    int result;
    Scene* scene = new Scene();
    result = BuildScene(*scene, p_scene.get());
    if (result == BUILDER_STATUS_OK)
      mashup.scene = scene;
    else {
      delete scene;
      return result;
    }
  } else {
    LOG_ERROR("No scene descriptor found for mashup " << mashup.id,
              LOGGER_BUILDER);
    return BUILDER_ERROR_REQUIRED_PROPERTY;
  }
  return BUILDER_STATUS_OK;
};

int MashupBuilder::AppendAssetDescriptors(Mashup& mashup,
                    const::boost::property_tree::ptree& p_tree) {
  int result;
  for (const auto& value_type_asset_desc : p_tree) {
    //value_type_asset_desc is ptree::value_type
    const pt::ptree& p_desc = value_type_asset_desc.second;
    AssetDescriptor* asset_desc = new AssetDescriptor();
    result = BuildAssetDescriptor(*asset_desc, p_desc);
    if (result == BUILDER_STATUS_OK)
      mashup.assets.insert(std::make_pair(asset_desc->id, asset_desc));
    else
      delete asset_desc;
  }
  return BUILDER_STATUS_OK;
};

int MashupBuilder::AppendActors(Scene& scene,
          const boost::property_tree::ptree& p_tree) {
  int result;
  for (const auto& value_type_actor: p_tree) {
    //value_type_actor is ptree::value_type
    const pt::ptree& p_actor = value_type_actor.second;
    Actor* actor = new Actor();
    result = BuildActor(*actor, p_actor);
    if (result == BUILDER_STATUS_OK)
      scene.actors.push_back(actor);
    else
      delete actor;
  }
  return BUILDER_STATUS_OK;
};

int MashupBuilder::AppendTransitions(Scene& scene,
          const boost::property_tree::ptree& p_tree) {
  int result;
  for (const auto& value_type_transition: p_tree) {
    //value_type_transition is ptree::value_type
    const pt::ptree& p_transition = value_type_transition.second;
    Transition* transition = new Transition();
    result = BuildTransition(*transition, p_transition);
    if (result == BUILDER_STATUS_OK)
      scene.transitions.push_back(transition);
    else
      delete transition;
  }
  return BUILDER_STATUS_OK;
};

int MashupBuilder::BuildAssetDescriptor(AssetDescriptor& asset_desc,
                               const boost::property_tree::ptree& p_tree) {
  asset_desc.id     = p_tree.get<long int>("id", 0);
  asset_desc.type   = AssetDescriptor::Get(p_tree.get<std::string>("type",""));
  asset_desc.length = p_tree.get<int>("length", 0);
  //TODO: parser for length being an int or a string.
  LOG_TRACE(" ADesc ID: " << asset_desc.id, LOGGER_BUILDER);
  LOG_TRACE(" ADesc type: " << asset_desc.type, LOGGER_BUILDER);
  LOG_TRACE(" ADesc length: " << asset_desc.length, LOGGER_BUILDER);
  if (boost::optional<const pt::ptree&> p_qualities =
      p_tree.get_child_optional("quality")) {
    int result;
    for (const auto& value_type_asset_quality : p_qualities.get()) {
      //value_type_asset_quality is ptree::value_type
      const pt::ptree& p_quality = value_type_asset_quality.second;
      AssetQualityDescriptor* asset_quality_desc =
        new AssetQualityDescriptor();
      result = BuildAssetQualityDescriptor(*asset_quality_desc, p_quality);
      if (result == BUILDER_STATUS_OK)
        asset_desc.qualities.push_back(asset_quality_desc);
      else
        delete asset_quality_desc;
    }
  } else {
    LOG_WARN("Asset with id: " << asset_desc.id
              << " has no 'quality' node.", LOGGER_BUILDER);
    return BUILDER_ERROR_REQUIRED_PROPERTY;
  }
  return BUILDER_STATUS_OK;
};

int MashupBuilder::BuildAssetQualityDescriptor(AssetQualityDescriptor& asset_quality_desc,
    const boost::property_tree::ptree& p_tree) {
  asset_quality_desc.path           = p_tree.get<std::string>("path", "");
  asset_quality_desc.composed_path  = PathBuilder::GetAbsolute(asset_quality_desc.path,
                                        assets_path);
  asset_quality_desc.bitrate        = p_tree.get<int>("bitrate", 0);
  asset_quality_desc.natural_width  = p_tree.get<int>("naturalWidth", 0);
  asset_quality_desc.natural_height = p_tree.get<int>("naturalHeight", 0);
  LOG_TRACE("  AQDesc path: " << asset_quality_desc.path, LOGGER_BUILDER);
  LOG_TRACE("  AQDesc comp. path: " << asset_quality_desc.composed_path, LOGGER_BUILDER);
  LOG_TRACE("  AQDesc bitrate: "<< asset_quality_desc.bitrate, LOGGER_BUILDER);
  LOG_TRACE("  AQDesc natural width: "<< asset_quality_desc.natural_width, LOGGER_BUILDER);
  LOG_TRACE("  AQDesc natural height: "<< asset_quality_desc.natural_height, LOGGER_BUILDER);
  return BUILDER_STATUS_OK;
};

int MashupBuilder::BuildScene(Scene& scene, const boost::property_tree::ptree& p_tree) {
  scene.width     = p_tree.get<int>("width", 0);
  scene.height    = p_tree.get<int>("height", 0);
  scene.length    = p_tree.get<int>("length", 0);
  scene.framerate = p_tree.get<int>("framerate", 25);
  LOG_TRACE(" Scene width: " << scene.width, LOGGER_BUILDER);
  LOG_TRACE(" Scene height: " << scene.height, LOGGER_BUILDER);
  LOG_TRACE(" Scene length: " << scene.length, LOGGER_BUILDER);
  LOG_TRACE(" Scene framerate: " << scene.framerate, LOGGER_BUILDER);
  if (boost::optional<const pt::ptree&> p_actors =
      p_tree.get_child_optional("actors")) {
    AppendActors(scene, p_actors.get());
  }
  if (boost::optional<const pt::ptree&> p_transitions =
      p_tree.get_child_optional("transitions")) {
    AppendTransitions(scene, p_transitions.get());
  }
  return BUILDER_STATUS_OK;
};

int MashupBuilder::BuildActor(Actor& actor, const boost::property_tree::ptree& p_tree) {
  actor.asset_id    = p_tree.get<long int>("assetId", 0);
  actor.instance_id = p_tree.get<int>("instanceId", 0);
  actor.type        = AssetDescriptor::Get(p_tree.get<std::string>("type",""));
  actor.x           = p_tree.get<double>("x", 0);
  actor.y           = p_tree.get<double>("y", 0);
  actor.z           = p_tree.get<int>("z", 0);
  actor.width       = p_tree.get<double>("width", 0);
  actor.height      = p_tree.get<double>("height", 0);
  actor.rotation    = p_tree.get<double>("rotation", 0);
  actor.enters_at   = p_tree.get<int>("entersAt", 0);
  actor.offset      = p_tree.get<int>("offset", 0);
  actor.exits_at    = p_tree.get<int>("exitsAt", 0);
  actor.resize_mode = Actor::Get(p_tree.get<std::string>("resize_mode",""));
  LOG_TRACE(" Actor asset ID: " << actor.asset_id, LOGGER_BUILDER);
  LOG_TRACE(" Actor instance ID: " << actor.instance_id, LOGGER_BUILDER);
  LOG_TRACE(" Actor x: " << actor.x, LOGGER_BUILDER);
  LOG_TRACE(" Actor y: " << actor.y, LOGGER_BUILDER);
  LOG_TRACE(" Actor z: " << actor.z, LOGGER_BUILDER);
  LOG_TRACE(" Actor width: " << actor.width, LOGGER_BUILDER);
  LOG_TRACE(" Actor height: " << actor.height, LOGGER_BUILDER);
  LOG_TRACE(" Actor rotation: " << actor.rotation, LOGGER_BUILDER);
  LOG_TRACE(" Actor enters at: " << actor.enters_at, LOGGER_BUILDER);
  LOG_TRACE(" Actor offset: " << actor.offset, LOGGER_BUILDER);
  LOG_TRACE(" Actor exists_at: " << actor.exits_at, LOGGER_BUILDER);
  return BUILDER_STATUS_OK;
};

int MashupBuilder::BuildTransition(Transition& transition,
                      const boost::property_tree::ptree& p_tree) {
  transition.asset_id    = p_tree.get<long int>("assetId", 0);
  transition.instance_id = p_tree.get<int>("instanceId", 0);
  transition.type        = AssetDescriptor::Get(p_tree.get<std::string>("type",""));
  transition.target = BaseProperty::Get(p_tree.get<std::string>("target",""));
  if (transition.target == BaseProperty::Target::Undef) {
    LOG_WARN("Transition for asset " << transition.asset_id << ":"
            << transition.instance_id << " has no valid target specified.",
            LOGGER_BUILDER);
    return BUILDER_ERROR_REQUIRED_PROPERTY;
  }
  transition.starts_at = p_tree.get<int>("startsAt", 0);
  transition.duration = p_tree.get<int>("duration", 0);
  int tuple_size = ValueBuilder::GetTupleSize(transition.target);
  boost::optional<const pt::ptree&> p_start_values = p_tree.get_child_optional("startValue");
  boost::optional<const pt::ptree&> p_end_values = p_tree.get_child_optional("endValue");
  if (tuple_size > 0 && p_end_values) { // End value parsing.
    if (tuple_size > 1) {
      std::vector<double> v_values;
      for (const auto& pt_end_val : p_end_values.get()) {
        v_values.push_back(pt_end_val.second.get<double>(""));
      }
      if (tuple_size == 2) {
        transition.end_value = ValueBuilder::Get(v_values[0], v_values[1]);
      } else {
        transition.end_value = ValueBuilder::Get(v_values[0], v_values[1] , v_values[2]);
      }
      std::vector<double>().swap(v_values);
    } else {
      transition.end_value = ValueBuilder::Get(p_tree.get<double>("endValue", 0));
    }
  } else {
    LOG_WARN("Transition for asset " << transition.asset_id << ":"
            << transition.instance_id << " has no valid endValue.", LOGGER_BUILDER);
    return BUILDER_ERROR_REQUIRED_PROPERTY;
  }
  if (tuple_size > 0 && p_start_values) { // Start value parsing.
    if (tuple_size > 1) {
      std::vector<double> v_values;
      for (const auto& pt_start_val : p_start_values.get()) {
        v_values.push_back(pt_start_val.second.get<double>(""));
      }
      if (tuple_size == 2) {
        transition.start_value = ValueBuilder::Get(v_values[0], v_values[1]);
      } else {
        transition.start_value = ValueBuilder::Get(v_values[0], v_values[1] , v_values[2]);
      }
      std::vector<double>().swap(v_values);
    } else {
      transition.start_value = ValueBuilder::Get(p_tree.get<double>("startValue", 0));
    }
  } else {
    transition.start_value = ValueBuilder::Get();
  }
  LOG_TRACE(" Transition asset ID: " << transition.asset_id, LOGGER_BUILDER);
  LOG_TRACE(" Transition instance ID: " << transition.instance_id, LOGGER_BUILDER);
  LOG_TRACE(" Transition target: " << transition.target, LOGGER_BUILDER);
  LOG_TRACE(" Transition starts at: " << transition.starts_at, LOGGER_BUILDER);
  LOG_TRACE(" Transition duration: " << transition.duration, LOGGER_BUILDER);
  LOG_TRACE(" Transition start value: " << transition.start_value->ToString(), LOGGER_BUILDER);
  LOG_TRACE(" Transition end value: " << transition.end_value->ToString(), LOGGER_BUILDER)
  return BUILDER_STATUS_OK;
};
