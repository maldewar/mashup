#ifndef MASHUP_MODEL_ACTOR_
#define MASHUP_MODEL_ACTOR_

#include <string>

#include "AssetDescriptor.h"

/**
 * @brief Contains initialization values for assets entering a scene.
 *
 * The initialization values depend on the type of asset the actor
 * represents.
 * @author Waldemar Sánchez
 * @date February, 2014
 */
class Actor {
  public:
    /**
     * Class constructor.
     */
    Actor();

  public:
    /**
     * Unique identifier for this instance.
     */
    std::string id;
    /**
     * ID for the assets representing this actor.
     */
    long int asset_id;
    /**
     * Type of asset.
     */
    AssetDescriptor::Type type;
    /**
     * Unique id relative to the mashup in case there are several
     * actors representing the same asset.
     */
    int instance_id;
    /**
     * Relative x position for the actor in relation to the scene's
     * width. 0 means left and 1 right of the scene rect.
     */
    double x;
    /**
     * Relative y position for the actor in relation to the scene's
     * height. O means bottom and 1 top of the scene rect.
     */
    double y;
    /**
     * Z index for ordering elements position from back to front.
     */
    int z;
    /**
     * Relative width value for the element in relation to the scene's
     * width. 1 means the whole width of the scene's rect.
     */
    double width;
    /**
     * Relative height value for the element in relation the scene's
     * height. 1 means the whole height of the scene's rect.
     */
    double height;
    /**
     * Counterclockwise rotation for the element in radians.
     */
    double rotation;
    /**
     * Time in milliseconds for the element to first appear on the scene,
     * relative to the total length of the mashup.
     */
    int enters_at;
    /**
     * Time in millisecond to cut off from the begining of the media.
     */
    int offset;
    /**
     * Time in milliseconds for the element to leave the scene, relative
     * to the total length of the mashup.
     */
    int exits_at;

};

#endif // MASHUP_MODEL_ACTOR_
