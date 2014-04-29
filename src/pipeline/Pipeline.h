#ifndef MASHUP_PIPELINE_PIPELINE_
#define MASHUP_PIPELINE_PIPELINE_

#include <string>
#include <vector>
#include <utility>

#include <gst/gst.h>
#include <boost/unordered_map.hpp>

#include "../model/Mashup.h"
#include "VideoActor.h"
#include "ImageActor.h"

/**
 * @brief Abstraction of a pipeline element of GStreamer.
 *
 * The class acts as a container for a GstElement pipeline element
 * and provides methods to manage it as well as the elements
 * contained within it.
 * @author Waldemar Sánchez
 * @date March, 2014
 */
class Pipeline {
  public:
    /**
     * Class constructor.
     */
    Pipeline();
    /**
     * Prepares the pipeline, connecting actors and setting
     * initial values.
     * @param seek_time Point in the timeline to start playing.
     */
    void Prepare(int seek_time = 0);
    /**
     * Start playing the pipeline.
     */
    void Play();
    /**
     * Sets the resolution for the video in pixels.
     * @param width  Width in pixels.
     * @param height Height in pixels.
     */
    void SetResolution(const unsigned int width,
                       const unsigned int height,
                       const unsigned int framerate);
    /**
     * Get the running position for this pipeline.
     * @return Position for this pipeline in nanoseconds.
     */
    long int GetPosition();
    /**
     * Get the duration of this pipeline.
     * @return Duration for this pipeline in nanoseconds.
     */
    long int GetDuration();
    /**
     * Handle messager from the pipeline bus.
     * @param bus Bus object for the pipeline.
     * @param msg Message received from the pipeline.
     * @param pipeline_obj Void pointer to the Pipeline object.
     * @return True if the message should not cause pipeline
     *         termination.
     */
    static int BusCall(GstBus* bus,
                       GstMessage* msg,
                       void* pipeline_obj);
    static int TimeoutBusCall(void* pipeline_obj);

  public:
    std::string id;
    /**
     * Output resolution width.
     */
    int width;
    /**
     * Output resolution height.
     */
    int height;
    /**
     * Output framerate.
     */
    int framerate;
    /**
     * Position in nanoseconds for the pipeline.
     */
    long int position;
    /**
     * Duration in nanoseconds for the pipeline.
     */
    long int duration;
    /**
     * Pipeline GStreamer Element.
     */
    GstElement* pipeline;
    /**
     * Main loop for playing this pipeline.
     */
    GMainLoop* loop;
    /**
     * Mixer for video inputs.
     */
    GstElement* videomixer;
    /**
     * Mixer for audio inputs.
     */
    GstElement* audiomixer;
    /**
     * Video convert for output.
     */
    GstElement* videoconvert;
    /**
     * Video output filter to set resolution.
     */
    GstElement* filter;
    /**
     * Keeps track of all the VideosActor instances involved in the pipeline.
     */
    boost::unordered_map<std::pair<long int, int>, VideoActor*> m_video_actors;
    /**
     * Keeps track of all the ImageActor instances involved in the pipeline.
     */
    boost::unordered_map<std::pair<long int, int>, ImageActor*> m_image_actors;
};

#endif // MASHUP_PIPELINE_PIPELINE_
