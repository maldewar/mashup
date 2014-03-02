#ifndef MASHUP_MODEL_ASSET_DESCRIPTOR_
#define MASHUP_MODEL_ASSET_DESCRIPTOR_

#include <string>
#include <vector>

#include "AssetQualityDescriptor.h"

/**
 * @brief Describes an asset.
 *
 * The properties are not concern with the file(s) an asset
 * might have.
 * @author Waldemar Sánchez
 * @date February, 2014
 */
class AssetDescriptor {
  public:
    /**
     * Class constructor.
     */
    AssetDescriptor();

  public:
    /**
     * Type of media.
     */
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
    std::vector<AssetQualityDescriptor*> qualities;
};

#endif // MASHUP_MODEL_ASSET_DESCRIPTOR_
