#ifndef MASHUP_MODEL_ASSET_QUALITY_DESCRIPTOR_
#define MASHUP_MODEL_ASSET_QUALITY_DESCRIPTOR_

#include <string>
#include <vector>

#include "AssetQualityDescriptor.h"

/**
 * @file
 * @brief Provides quality-related details on an asset's file.
 *
 * The properties belong to one file, as an asset might have
 * many files.
 * @author Waldemar Sánchez
 * @date February, 2014
 */
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
    int natural_width;
    /**
     * Height in pixels.
     */
    int natural_height;
};

#endif // MASHUP_MODEL_ASSET_QUALITY_DESCRIPTOR_
