#include "Pipeline.h"

#include "../util/Log.h"

Pipeline::Pipeline() {
  id = "dummy";
  pipeline = gst_pipeline_new(id.c_str());
  videomixer = gst_element_factory_make("videomixer", id.c_str());
  audiomixer = gst_element_factory_make("adder", id.c_str());
};

int Pipeline::Play() {
  GMainLoop *loop;
  loop = g_main_loop_new(NULL, false);
  GstElement *e_videotestsrc;
  GstElement *e_autovideosink;
  e_videotestsrc = gst_element_factory_make("videotestsrc", "videotestsrc");
  e_autovideosink = gst_element_factory_make("autovideosink", "autovideosink");
  gst_bin_add_many(GST_BIN(pipeline), e_videotestsrc, e_autovideosink, NULL);
  gst_element_link_many(e_videotestsrc, e_autovideosink, NULL);
  gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_PLAYING);
  g_main_loop_run (loop);
};

void Pipeline::OnDecodeDynamicPad(GstElement* decodebin,
                                      GstPad* pad,
                                    gpointer* data) {
  Pipeline* pipeline = reinterpret_cast<Pipeline*>(data);
};
