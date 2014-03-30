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
     */
    void Prepare();
    /**
     * Start playing the pipeline.
     */
    void Play();
    /**
     * Sets the resolution for the video in pixels.
     */
    void SetResolution(const int width, const int height);
    static int BusCall(GstBus* bus,
                       GstMessage* msg,
                       void* pipeline_obj);

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
