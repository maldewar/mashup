#ifndef MASHUP_MODEL_TRANSITION_
#define MASHUP_MODEL_TRANSITION_

#include "../common/SingleValue.h"

/**
 * @file
 * @brief Contains parameters to transition values on an asset.
 *
 * Contains parameters used to progress from an initial value to
 * a desired one.
 * @author Waldemar Sánchez
 * @date February, 2014
 */
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
    /**
     * Asset ID.
     */
    long int asset_id;
    /**
     * Instance ID for an asset in case it has more than one
     * actor representing it.
     */
    int instance_id;
    /**
     * Target value or property on the asset for the transition.
     */
    Target target;
    /**
     * Initial value for the transition, if NULL then the value
     * set on the asset is used instead.
     */
    SingleValue* start_value;
    /**
     * Target value for the transition, required.
     */
    SingleValue* end_value;
    /**
     * Time in milliseconds when the transition should being,
     * relative to the whole length of the mashup containing it.
     */
    int starts_at;
    /**
     * Time in milliseconds to go from start value to end value. 
     */
    int duration;
    /**
     * Function used to transition from start value to end value.
     */
    Easing easing;

};

#endif // MASHUP_MODEL_TRANSITION_
