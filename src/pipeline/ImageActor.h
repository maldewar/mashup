#ifndef MASHUP_PIPELINE_IMAGEACTOR_
#define MASHUP_PIPELINE_IMAGEACTOR_

#include "BaseActor.h"

/**
 * @brief Abstraction of elements required to creater an Image actor.
 *
 * The class acts as a container for GStreamer Elements involved in the
 * reading and processing of images to be mixed in a Pipeline.
 * @author Waldemar Sánchez
 * @date March, 2014
 */
class ImageActor : public BaseActor {
  public:
    /**
     * Class constructor.
     */
    ImageActor(Actor* actor,
               AssetDescriptor* asset_descriptor,
               AssetQualityDescriptor* asset_quality_descriptor);
    void SetDimensions(double width, double height);
    void SetX(double x);
    void SetY(double y);
    void SetZ(int z);
    void SetGstElements();
    static void OnDecodebinPadAdded(GstElement* decodebin,
                                    GstPad*     pad,
                                    ImageActor* image_actor);

  public:
    /**
     * Reads the data stream from a file.
     */
    GstElement* filesrc;
    /**
     * Decodes the data stream and produces video/audio output.
     */
    GstElement* decodebin;
    /**
     * Converts video to video/x-raw format.
     */
    GstElement* videoconvert;
    /**
     * Create the frames with the base image.
     */
    GstElement* imagefreeze;
    /**
     * Scale the proportions of the video channel.
     */
    GstElement* videoscale;
    /**
     * Video output filter to set resolution.
     */
    GstElement* filter;
    int scene_width;
    int scene_height;

  private:
    double height;
    double width;
    int area_width_px;
    int area_height_px;
    int width_px;
    int height_px;
    double x;
    double y;
    int z;
};

#endif // MASHUP_PIPELINE_IMAGEACTOR_
