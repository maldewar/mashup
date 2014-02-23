#ifndef MASHUP_MODEL_ASSET_DESCRIPTOR_
#define MASHUP_MODEL_ASSET_DESCRIPTOR_

#include <string>
#include <vector>

#include "AssetQualityDescriptor.h"

class AssetDescriptor {
  public:
    /**
     * Class constructor.
     */
    AssetDescriptor();

  public:
    enum Type
    {
        Undef,
        Image,
        Audio,
        Video
    };
    /**
     * Id of the asset.
     */
    long int id;
    /**
     * Type of asset.
     */
    Type type;
    /**
     * Duration of the asset in milliseconds.
     */
    int length;
    /**
     * Qualities available for the asset.
     */
    std::vector<AssetQualityDescriptor*>;
};

#endif // MASHUP_MODEL_ASSET_DESCRIPTOR_
