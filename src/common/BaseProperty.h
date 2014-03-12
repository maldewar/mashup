#ifndef MASHUP_COMMON_BASE_PROPERTY_
#define MASHUP_COMMON_BASE_PROPERTY_

#include <string>

/**
 * Constant string to display unknown properties.
 */
const std::string PROPERTY_UNKNOWN = "<unknown>";

/**
 * String value for position.
 */
const std::string PROPERTY_POSITION = "position";

/**
 * String value for dimension.
 */
const std::string PROPERTY_DIMENSION = "dimension";

/**
 * String value for rotation.
 */
const std::string PROPERTY_ROTATION = "rotation";

/**
 * String value for scale.
 */
const std::string PROPERTY_SCALE = "scale";

/**
 * String value for opacity.
 */
const std::string PROPERTY_OPACITY = "opacity";
const std::string PROPERTY_VOLUME = "volume";
const std::string PROPERTY_COLOR = "color";

/**
 * @brief Basic properties for Assets.
 *
 * Basic properties for Assets.
 * @author Waldemar Sánchez
 * @date February, 2014
 */
class BaseProperty {
  public:
    /**
     * Target property on asset.
     */
    enum Target {
      Undef,
      Position,
      Dimension,
      Rotation,
      Scale,
      Opacity,
      Volume,
      Color
    };
    /**
     * Gets the target property enum value given a string value.
     * @param target String value for the property.
     * @return Target property enum value.
     */
    static Target Get(const std::string target);
    /**
     * Gets the string value for a property given its enum value.
     * @param target Enum value for the property.
     * @return String value for the property.
     */
    static std::string Get(const BaseProperty::Target target);

  private:
    BaseProperty();

};

#endif // MASHUP_COMMON_BASE_PROPERTY_
