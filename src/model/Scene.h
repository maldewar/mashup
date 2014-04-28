#ifndef MASHUP_MODEL_SCENE_
#define MASHUP_MODEL_SCENE_

#include <vector>

#include "Actor.h"
#include "Transition.h"

/**
 * @brief Instruction for actors and transitions.
 *
 * The object may include cameras, specification on the display area,
 * asset init values and their transitions..
 * @author Waldemar Sánchez
 * @date February, 2014
 */
class Scene {
  public:
    /**
     * Class constructor.
     */
    Scene();

  public:
    /**
     * Defines a rect to be applied to a viewport.
     */
    int camera;
    /**
     * Width in pixels for the viewport or document.
     */
    int width;
    /**
     * Height in pixels for the viewport or document.
     */
    int height;
    /**
     * Length in millisecond for the whole mashup.
     */
    int length;
    /**
     * Number of frames per second.
     */
    int framerate;
    /**
     * Listing of actor to be included in the mashup.
     */
    std::vector<Actor*> actors;
    /**
     * List of transition instructions for the actors
     * in the mashup.
     */
    std::vector<Transition*> transitions;
};

#endif // MASHUP_MODEL_SCENE_
