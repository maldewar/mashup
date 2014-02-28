#ifndef MASHUP_MODEL_TRANSITION_
#define MASHUP_MODEL_TRANSITION_

#include "../common/SingleValue.h"

class Transition {
  public:
    /**
     * Class constructor.
     */
    Transition();

  public:
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
    enum Easing {
      Linear
    };
    long int asset_id;
    int instance_id;
    Target target;
    SingleValue* start_value;
    SingleValue* end_value;
    int start_at;
    int duration;
    Easing easing;

};

#endif // MASHUP_MODEL_TRANSITION_
