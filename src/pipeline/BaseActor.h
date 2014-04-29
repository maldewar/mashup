#ifndef MASHUP_PIPELINE_BASEACTOR_
#define MASHUP_PIPELINE_BASEACTOR_

#include <string>
#include <vector>
#include <sstream>

#include <gst/gst.h>

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
     * @param pipeline Pipeline to plug the actor in. The pipeline
     *                 becomes the parent pipeline.
     */
    virtual bool Plug(Pipeline* pipeline);
    /**
     * Unplugs the actor from its parent pipeline.
     */
    virtual bool Unplug();

  protected:
    std::string GetGstElementId(std::string base);
    void CheckGstElements(std::vector<std::pair<std::string, GstElement*>> elements);
    virtual void SetGstElements() = 0;

  public:
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
     * Bin to contain all the elements created for this actor.
     */
    GstElement* bin;
    /**
     * Pad to be used as video output for the bin.
     */
    GstPad* video_src_pad;
    /**
     * Pad to be used as audio output for the bin.
     */
    GstPad* audio_src_pad;
    /**
     * Pad to the mixing element for video this bin is linked to.
     */
    GstPad* videomixer_pad;
    /**
     * Pad to the mixing element for audio this bin is linked to.
     */
    GstPad* audiomixer_pad;

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
};

#endif // MASHUP_PIPELINE_BASEACTOR_
