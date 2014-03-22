#include "AssetDescriptor.h"

AssetDescriptor::AssetDescriptor() {
  id     = 0;
  type   = Undef;
  length = 0;
};

std::string AssetDescriptor::Get(AssetDescriptor::Type type) {
  switch(type) {
    case AssetDescriptor::Type::Video:
      return ASSET_TYPE_VIDEO;
    case AssetDescriptor::Type::Audio:
      return ASSET_TYPE_AUDIO;
    case AssetDescriptor::Type::Image:
      return ASSET_TYPE_IMAGE;
  }
  return ASSET_TYPE_UNDEF;
};

AssetDescriptor::Type AssetDescriptor::Get(const std::string type) {
  if (type.compare(ASSET_TYPE_VIDEO) == 0)
    return AssetDescriptor::Type::Video;
  if (type.compare(ASSET_TYPE_AUDIO) == 0)
    return AssetDescriptor::Type::Audio;
  if (type.compare(ASSET_TYPE_IMAGE) == 0)
    return AssetDescriptor::Type::Image;
  return AssetDescriptor::Type::Undef;
};
