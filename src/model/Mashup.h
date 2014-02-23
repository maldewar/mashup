#ifndef MASHUP_MODEL_MASHUP_
#define MASHUP_MODEL_MASHUP_

#include <string>
#include <vector>

#include "AssetDescriptor.h"

class Mashup {
  public:
    /**
     * Class constructor.
     */
    Mashup();
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
    long int creatorId;
    /**
     * Descriptors for the assets participating on this mashup.
     */
    std::vector<AssetDescriptor*>;
    /**
     * Scene instructions for this mashup.
     */
    Scene* scene;
};

#endif // MASHUP_MODEL_MASHUP_
