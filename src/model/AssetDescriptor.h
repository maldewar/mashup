#ifndef MASHUP_MODEL_ASSET_DESCRIPTOR_
#define MASHUP_MODEL_ASSET_DESCRIPTOR_

#include <string>
#include <vector>

#include "AssetQualityDescriptor.h"

const std::string ASSET_TYPE_UNDEF = "undef";

const std::string ASSET_TYPE_VIDEO = "video";

const std::string ASSET_TYPE_AUDIO = "audio";

const std::string ASSET_TYPE_IMAGE = "image";

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

  public:
     /**
     * Gets the string value for an asset type given its enum value.
     * @param type Enum value for the asset type.
     * @return String value for the asset type.
     */
    static std::string Get(const AssetDescriptor::Type type);
    /**
     * Gets the enum value for an asset type given its string value.
     * @param type String value for the type.
     * @return Enum value for the asset type.
     */
    static AssetDescriptor::Type Get(const std::string type);
};

#endif // MASHUP_MODEL_ASSET_DESCRIPTOR_
