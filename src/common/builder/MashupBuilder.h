#ifndef MASHUP_COMMON_BUILDER_MASHUPBUILDER_
#define MASHUP_COMMON_BUILDER_MASHUPBUILDER_

#include <string>

#include <boost/property_tree/ptree.hpp>

#include "../../model/Mashup.h"
#include "../../model/AssetDescriptor.h"

/**
 * Everything was OK with building the object.
 */
const int BUILDER_STATUS_OK = 0;

/**
 * Error due the lack of a required property.
 */
const int BUILDER_ERROR_REQUIRED_PROPERTY = -1;

/**
 * @brief Populates Mashup objects with their properties.
 *
 * The builder uses a boost::property_tree object to
 * obtain the values to populate the Mashup object, it may
 * as well create objects related to that class.
 * @author Waldemar Sánchez
 * @date February, 2014
 */
class MashupBuilder {
  public:
    /**
     * Class constructor.
     */
    MashupBuilder();
    /**
     * Builds a mashup object with values taken from
     * a boost::property_tree.
     * @param mashup Mashup object.
     * @param p_tree Property tree object.
     * @return Status or error code about the building.
     */
    int Build(Mashup& mashup, const boost::property_tree::ptree& p_tree);
    /**
     * Appends AssetDescriptor objects to a given
     * Mashup object taking values from a boost::property_tree.
     * @param mashup Mashup object.
     * @param p_tree Property tree object.
     * @return Status or error code on the building of AssetDescriptor objects.
     */
    int AppendAssetDescriptors(Mashup& mashup,
                               const boost::property_tree::ptree& p_tree);
    /**
     * Appends Actor objects to a given Scene object, taking values
     * from a boost::property_tree object.
     * @param scene  Scene object.
     * @param p_tree Property tree object.
     * @return Status or error code on the building of Actor objects.
     */
    int AppendActors(Scene& scene, const boost::property_tree::ptree& p_tree);
    /**
     * Appeds Transition objects to a given Scene object, taking values
     * from a boost::property_tree object.
     * @param scene  Scene object.
     * @param p_tree Property tree object.
     * @return Status or error code on the building of Transition objects.
     */
    int AppendTransitions(Scene& scene, const boost::property_tree::ptree& p_tree);
    /**
     * Gets the asset type enum value given a string value.
     * @param type String value for type.
     * @return Asset Type enum value.
     */
    AssetDescriptor::Type GetAssetType(const std::string type);

  private:
    /**
     * Builds a AssetDescriptor object with values taken from
     * a boost::property_tree.
     * @param asset_desc AssetDescriptor object.
     * @param p_tree     Property tree object.
     * @return Status or error code about the building.
     */
    int BuildAssetDescriptor(AssetDescriptor& asset_desc,
                             const boost::property_tree::ptree& p_tree);
    /**
     * Builds a AssetQualityDescriptor object with values taken from
     * a boost::property_tree.
     * @param quality_desc AssetQualityDescriptor object.
     * @param p_tree       Property tree object.
     * @return Status or error code about the building.
     */
    int BuildAssetQualityDescriptor(AssetQualityDescriptor& quality_descriptor,
                             const boost::property_tree::ptree& p_tree);
    /**
     * Builds a Scene object with values taken from a boost::property_tree.
     * @param scene  Scene object.
     * @param p_tree Property tree object.
     * @return Status or error code about the building.
     */
    int BuildScene(Scene& scene, const boost::property_tree::ptree& p_tree);
    /**
     * Builds an Actor object with values taken from a boost::property_tree.
     * @param actor  Actor object.
     * @param p_tree Property tree object.
     * @return Status or error code about the building.
     */
    int BuildActor(Actor& actor, const boost::property_tree::ptree& p_tree);
    /**
     * Builds a Transition object with values taken from a boost::property_tree.
     * @param transition Transition object.
     * @param p_tree     Property tree object.
     * @return Status or error code about  the building.
     */
    int BuildTransition(Transition& transition,
                        const boost::property_tree::ptree& p_tree);
};

#endif // MASHUP_COMMON_BUILDER_MASHUPBUILDER_
