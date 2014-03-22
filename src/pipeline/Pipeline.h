#ifndef MASHUP_PIPELINE_PIPELINE_
#define MASHUP_PIPELINE_PIPELINE_

#include <string>
#include <vector>
#include <utility>

#include <gst/gst.h>
#include <boost/unordered_map.hpp>

#include "../model/Mashup.h"
#include "VideoActor.h"

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
    int Play();
    static void OnDecodeDynamicPad(GstElement* decodebin,
                                   GstPad*     pad,
                                   gpointer*   data);

  public:
    std::string id;
    /**
     * Pipeline GStreamer Element.
     */
    GstElement* pipeline;
    /**
     * Mixer for video inputs.
     */
    GstElement* videomixer;
    /**
     * Mixer for audio inputs.
     */
    GstElement* audiomixer;
    /**
     * Keeps track of all the VideosActor instances involved in the pipeline.
     */
    boost::unordered_map<std::pair<long int, int>, VideoActor*> m_video_actors;
    //boost::unordered_map<std::pair<long int, int>, GstElement*> m_audio_actors;
    //boost::unordered_map<std::pair<long int, int>, GstElement*> m_image_actors;
};

#endif // MASHUP_MODEL_MASHUP_
