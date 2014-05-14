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
    bool Plug(Pipeline* pipeline);
    bool Unplug();
    bool Prepare(int seek_time = 0);
    void SetGstElements();
    void SetDimensions(double width, double height);
    void SetX(double x);
    void SetY(double y);
    void SetZ(int z);
    void SetAlpha(double alpha);

  public:
    bool plugged;
    GESTrackElement* audio_track_element;
    GESTrackElement* video_track_element;
    
    

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
