#ifndef MASHUP_MODEL_ASSET_QUALITY_DESCRIPTOR_
#define MASHUP_MODEL_ASSET_QUALITY_DESCRIPTOR_

#include <string>
#include <vector>

#include "AssetQualityDescriptor.h"

class AssetQualityDescriptor {
  public:
    /**
     * Class constructor.
     */
    AssetQualityDescriptor();

  public:
    /**
     * Path to the asset.
     */
    std::string path;
    /**
     * Bitrate in kb/s.
     */
    int bitrate;
    /**
     * Width in pixels.
     */
    int naturalWidth;
    /**
     * Height in pixels.
     */
    int naturalHeight;
};

#endif // MASHUP_MODEL_ASSET_QUALITY_DESCRIPTOR_
