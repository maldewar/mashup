#ifndef MASHUP_MODEL_SCENE_
#define MASHUP_MODEL_SCENE_

#include <vector>

#include "Actor.h"
#include "Transition.h"

class Scene {
  public:
    /**
     * Class constructor.
     */
    Scene();

  public:
    int camera;
    int width;
    int height;
    int length;
    std::vector<Actor*> actors;
    std::vector<Transition*> transition;
};

#endif // MASHUP_MODEL_SCENE_
