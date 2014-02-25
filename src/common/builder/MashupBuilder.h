#ifndef MASHUP_COMMON_BUILDER_MASHUPBUILDER_
#define MASHUP_COMMON_BUILDER_MASHUPBUILDER_

#include <string>

#include <boost/property_tree/ptree.hpp>

#include "MashupBuilder.h"
#include "../../model/Mashup.h"

const int BUILDER_STATUS_OK               = 0;
const int BUILDER_ERROR_REQUIRED_PROPERTY = -1;

/**
 * @file
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
};

#endif // MASHUP_COMMON_BUILDER_MASHUPBUILDER_
