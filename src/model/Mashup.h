#ifndef MASHUP_MODEL_MASHUP_
#define MASHUP_MODEL_MASHUP_

#include <string>
#include <unordered_map>

#include "AssetDescriptor.h"
#include "Scene.h"

/**
 * @brief Container for instructions for building a mashup.
 *
 * The full instructions must contain descriptors for all the
 * required assets as a full populated Scene object.
 * @author Waldemar Sánchez
 * @date February, 2014
 */
class Mashup {
  public:
    /**
     * Class constructor.
     */
    Mashup();

  public:
    /**
     * Version for the script formatting.
     */
    std::string version;
    /**
     * Id of the mashup if any.
     */
    long int id;
    /**
     * Title for the mashup.
     */
    std::string title;
    /**
     * Id of the creator of this mashup.
     */
    long int creator_id;
    /**
     * Descriptors for the assets participating on this mashup.
     */
    std::unordered_map<long int, AssetDescriptor*> assets;
    /**
     * Scene instructions for this mashup.
     */
    Scene* scene;
};

#endif // MASHUP_MODEL_MASHUP_
