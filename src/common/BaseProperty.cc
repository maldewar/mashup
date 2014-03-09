#include "BaseProperty.h"

BaseProperty::BaseProperty() {};

BaseProperty::Target BaseProperty::Get(const std::string target) {
  if (target.compare("") == 0)
    return BaseProperty::Target::Undef;
  if (target.compare(PROPERTY_POSITION) == 0)
    return BaseProperty::Target::Position;
  if (target.compare(PROPERTY_DIMENSION) == 0)
    return BaseProperty::Target::Dimension;
  if (target.compare(PROPERTY_ROTATION) == 0)
    return BaseProperty::Target::Rotation;
  if (target.compare(PROPERTY_SCALE) == 0)
    return BaseProperty::Target::Scale;
  if (target.compare(PROPERTY_OPACITY) == 0)
    return BaseProperty::Target::Opacity;
  if (target.compare(PROPERTY_VOLUME) == 0)
    return BaseProperty::Target::Volume;
  if (target.compare(PROPERTY_COLOR) == 0)
    return BaseProperty::Target::Color;
  return BaseProperty::Target::Undef;
};

std::string BaseProperty::Get(const BaseProperty::Target target) {
  switch(target) {
    case BaseProperty::Target::Position:
      return PROPERTY_POSITION;
    case BaseProperty::Target::Dimension:
      return PROPERTY_DIMENSION;
    case BaseProperty::Target::Rotation:
      return PROPERTY_ROTATION;
    case BaseProperty::Target::Scale:
      return PROPERTY_SCALE;
    case BaseProperty::Target::Opacity:
      return PROPERTY_OPACITY;
    case BaseProperty::Target::Volume:
      return PROPERTY_VOLUME;
    case BaseProperty::Target::Color:
      return PROPERTY_COLOR;
  }
  return PROPERTY_UNKNOWN;
};
