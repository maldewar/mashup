#ifndef MASHUP_PIPELINE_GSTCONSTANTS_
#define MASHUP_PIPELINE_GSTCONSTANTS_

#include <string>

const std::string GST_ELEM_BIN          = "bin";
const std::string GST_ELEM_FILESRC      = "filesrc";
const std::string GST_ELEM_QUEUE        = "queue";
const std::string GST_ELEM_DECODEBIN    = "decodebin";
const std::string GST_ELEM_VIDEOCONVERT = "videoconvert";
const std::string GST_ELEM_AUDIOCONVERT = "audioconvert";
const std::string GST_ELEM_VIDEOSCALE   = "videoscale";
const std::string GST_ELEM_VOLUME       = "volume";
const std::string GST_ELEM_IMAGEFREEZE  = "imagefreeze";
const std::string GST_ELEM_CAPSFILTER   = "capsfilter";

const std::string GST_PROP_LOCATION  = "location";
const std::string GST_PROP_WIDTH     = "width";
const std::string GST_PROP_HEIGHT    = "height";
const std::string GST_PROP_FRAMERATE = "framerate";

const std::string M_GST_PAD_SINK       = "sink";
const std::string M_GST_PAD_SRC        = "src";
const std::string M_GST_PAD_MULTI_SINK = "sink_%u";
const std::string M_GST_PAD_MULTI_SRC  = "src_%u";
const std::string M_GST_PAD_VIDEO_SRC  = "video_src";
const std::string M_GST_PAD_AUDIO_SRC  = "audio_src";

const std::string GST_EVENT_PAD_ADDED    = "pad-added";
const std::string GST_EVENT_NO_MORE_PADS = "no-more-pads";

#endif // MASHUP_PIPELINE_GSTCONSTANTS_
