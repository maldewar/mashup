#ifndef MASHUP_PIPELINE_BASEACTOR_
#define MASHUP_PIPELINE_BASEACTOR_

#include <string>
#include <vector>
#include <sstream>

#include <gst/gst.h>
#include <ges/ges.h>

#include "../model/Actor.h"
#include "../model/AssetDescriptor.h"
#include "../model/AssetQualityDescriptor.h"

// Forward declaration.
class Pipeline;

/**
 * @brief Base class for actors in the pipeline.
 *
 * The class acts as a container for GStreamer Elements involved in the
 * reading and processing of images to be mixed in a Pipeline.
 * @author Waldemar Sánchez
 * @date March, 2014
 */
class BaseActor {
  public:
    /**
     * Class constructor.
     */
    BaseActor(Actor* actor,
               AssetDescriptor* asset_descriptor,
               AssetQualityDescriptor* asset_quality_descriptor);
    /**
     * Set true if the actor has been plugged into a pipeline.
     * @active True if the actor pads are connected within a pipeline.
     */
    void SetActive(bool active);
    /**
     * Tells if the actor is plugged into a running pipeline.
     * @return True if the actor pads are connected within a pipeline.
     */
    bool IsActive();
    /**
     * Prepares the element to be part of a pipeline.
     * @param pipeline  Pipeline object.
     * @param seek_time Point in the timeline length to play.
     * @return True if the actor is prepared.
     */
    virtual bool Prepare(Pipeline* pipeline, int seek_time = 0);
    /**
     * Plugs the actor in a pipeline.
     */
    virtual bool Plug(Pipeline* pipeline);
    /**
     * Unplugs the actor from its parent pipeline.
     */
    virtual bool Unplug();

  protected:
    std::string GetGstElementId(std::string base);
    void CheckGstElements(std::vector<std::pair<std::string, GstElement*>> elements);
    void ClearTimeouts();
    void SetTimelineProperties();
    virtual void SetGstElements() = 0;

  public:
    /**
     * Nanoseconds to wait for the clip to start in the pipeline.
     */
    unsigned long int enters_at_nano;
    /**
     * Nanoseconds to cut from the begining of the clip.
     */
    unsigned long int offset_nano;
    /**
     * Nanoseconds to play the clip.
     */
    unsigned long int duration_nano;
    /**
     * Resize Mode.
     */
    enum ResizeMode {
      /**
       * Black borders are added in order to keep aspect ratio.
       */
      AspectBorderBlack,
      /**
       * Transparent borders are added in order to keep aspect ratio.
       */
      AspectBorderTransparent,
      /**
       * The image is resizes to fit the requested dimensions.
       */
      AspectNone,
      /**
       * Aspect ratio is kept but cropping of the image is necessary.
       */
      AspectCrop
    };
    /**
     * Mashup Actor instance.
     */
    Actor* actor;
    /**
     * Mashupt AssetDescriptor instance.
     */
    AssetDescriptor* asset_descriptor;
    /**
     * Mashup AssetQualityDescriptor instance.
     */
    AssetQualityDescriptor* asset_quality_descriptor;
    /**
     * Pointer to the asset in the project.
     */
    GESAsset* asset;
    /**
     * Clip in the timeline.
     */
    GESClip* clip;
    

  protected:
    std::stringstream ss;
    /**
     * The value is true when the actor has been connected
     * to a playing pipeline.
     */
    bool active;
    /**
     * Parent pipeline if the actor is active in one.
     */
    Pipeline* parent_pipeline;
    /**
     * Identifiers for timeout events related to this actor.
     */
    std::vector<long int> timeout_ids;
};

#endif // MASHUP_PIPELINE_BASEACTOR_
