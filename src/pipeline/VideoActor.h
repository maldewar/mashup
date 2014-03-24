#ifndef MASHUP_PIPELINE_VIDEOACTOR_
#define MASHUP_PIPELINE_VIDEOACTOR_

#include <string>
#include <sstream>

#include <gst/gst.h>

#include "../model/Actor.h"
#include "../model/AssetDescriptor.h"
#include "../model/AssetQualityDescriptor.h"

/**
 * @brief Abstraction of elements required to creater a Scene actor.
 *
 * The class acts as a container for GStreamer Elements involved in the
 * reading and processing of videos to be mixed in a Pipeline.
 * @author Waldemar Sánchez
 * @date March, 2014
 */
class VideoActor {
  public:
    /**
     * Class constructor.
     */
    VideoActor(Actor* actor,
               AssetDescriptor* asset_descriptor,
               AssetQualityDescriptor* asset_quality_descriptor);
    static void OnDecodebinPadAdded(GstElement* decodebin,
                                    GstPad*     pad,
                                    VideoActor*   video_actor);
  private:
    std::string GetGstElementId(std::string base);

  public:
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
     * Reads the data stream from a file.
     */
    GstElement* filesrc;
    /**
     * Creates to buffer in memory to contain the data read
     * from the file.
     */
    GstElement* queue;
    /**
     * Decodes the data stream and produces video/audio output.
     */
    GstElement* decodebin;
    /**
     * Converts video to video/x-raw format.
     */
    GstElement* videoconvert;
    /**
     * Converts audio to audio/x-raw format.
     */
    GstElement* audioconvert;
    /**
     * Scale the proportions of the video channel.
     */
    GstElement* videoscale;
    /**
     * Modifies the volume of the audio channel.
     */
    GstElement* volume;
    /**
     * Pad to be used as video output for the bin.
     */
    GstPad* video_src_pad;
    /**
     * Pad to be user as audio output for the bin.
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

  private:
    std::stringstream ss;
};

#endif // MASHUP_PIPELINE_VIDEOACTOR_
