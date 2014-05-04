#ifndef MASHUP_PIPELINE_VIDEOACTOR_
#define MASHUP_PIPELINE_VIDEOACTOR_

#include "BaseActor.h"
#include "Pipeline.h"

/**
 * @brief Abstraction of elements required to creater a Scene actor.
 *
 * The class acts as a container for GStreamer Elements involved in the
 * reading and processing of videos to be mixed in a Pipeline.
 * @author Waldemar Sánchez
 * @date March, 2014
 */
class VideoActor : public BaseActor {
  public:
    /**
     * Class constructor.
     */
    VideoActor(Actor* actor,
               AssetDescriptor* asset_descriptor,
               AssetQualityDescriptor* asset_quality_descriptor);
    bool Prepare(Pipeline* pipeline, int seek_time = 0);
    bool Plug();
    bool Unplug();
    static int PlugTimeout(void* obj);
    static int UnplugTimeout(void* obj);
    static int SeekTimeout(void* obj);
    void SetGstElements();
    void SetDimensions(double width, double height);
    void SetX(double x);
    void SetY(double y);
    void SetZ(int z);
    void SetAlpha(double alpha);
    void DecBlockCount();
    static void OnDecodebinPadAdded(GstElement* decodebin,
                                    GstPad*     pad,
                                    VideoActor*   video_actor);
    static void OnNoMorePads(GstElement* decodebin,
                             GstPad*     pad,
                             VideoActor*   video_actor);
    static int OnBlockedPad(GstPad*          pad,
                            GstPadProbeInfo* info,
                            VideoActor*      video_actor);

  public:
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
     * Video output filter to set resolution.
     */
    GstElement* filter;
    /**
     * Modifies the volume of the audio channel.
     */
    GstElement* volume;
    int scene_width;
    int scene_height;
    int block_count;
    bool prerolled;
    

  private:
    double height;
    double width;
    int area_height_px;
    int area_width_px;
    int height_px;
    int width_px;
    double x;
    double y;
    int z;
    double alpha;
};

#endif // MASHUP_PIPELINE_VIDEOACTOR_
