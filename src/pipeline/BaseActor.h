#ifndef MASHUP_PIPELINE_BASEACTOR_
#define MASHUP_PIPELINE_BASEACTOR_

#include <string>
#include <vector>
#include <sstream>

#include <gst/gst.h>

#include "../model/Actor.h"
#include "../model/AssetDescriptor.h"
#include "../model/AssetQualityDescriptor.h"

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
};

#endif // MASHUP_PIPELINE_BASEACTOR_
