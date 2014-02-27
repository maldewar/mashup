#include "MashupBuilder.h"

#include "../../util/Log.h"

namespace pt = boost::property_tree;

MashupBuilder::MashupBuilder() {
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
      mashup.assets.push_back(asset_desc);
    else
      delete asset_desc;
  }
};

AssetDescriptor::Type MashupBuilder::GetAssetType(const std::string type) {
  if (type.compare("video") == 0)
    return AssetDescriptor::Type::Video;
  if (type.compare("audio") == 0)
    return AssetDescriptor::Type::Audio;
  if (type.compare("image") == 0)
    return AssetDescriptor::Type::Image;
  return AssetDescriptor::Type::Undef;
};

int MashupBuilder::BuildAssetDescriptor(AssetDescriptor& asset_desc,
                               const boost::property_tree::ptree& p_tree) {
  asset_desc.id     = p_tree.get<long int>("id", 0);
  asset_desc.type   = GetAssetType(p_tree.get<std::string>("type",""));
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
  asset_quality_desc.bitrate        = p_tree.get<int>("bitrate", 0);
  asset_quality_desc.natural_width  = p_tree.get<int>("naturalWidth", 0);
  asset_quality_desc.natural_height = p_tree.get<int>("naturalHeight", 0);
  LOG_TRACE("  AQDesc path: "<< asset_quality_desc.path, LOGGER_BUILDER);
  LOG_TRACE("  AQDesc bitrate: "<< asset_quality_desc.bitrate, LOGGER_BUILDER);
  LOG_TRACE("  AQDesc natural width: "<< asset_quality_desc.natural_width, LOGGER_BUILDER);
  LOG_TRACE("  AQDesc natural height: "<< asset_quality_desc.natural_height, LOGGER_BUILDER);
  return BUILDER_STATUS_OK;
};
