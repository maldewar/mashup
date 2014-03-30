#include "ImageActor.h"

#include "GstConstants.h"
#include "../util/Math.h"
#include "../util/Log.h"

ImageActor::ImageActor(Actor* actor,
                      AssetDescriptor* asset_descriptor,
                      AssetQualityDescriptor* asset_quality_descriptor) :
            BaseActor(actor, asset_descriptor, asset_quality_descriptor) {
  if (actor->id.compare("") != 0) {
    bin          = gst_bin_new(GetGstElementId(GST_ELEM_BIN).c_str());
    filesrc      = gst_element_factory_make(GST_ELEM_FILESRC.c_str(),
                                            GetGstElementId(GST_ELEM_FILESRC).c_str());
    g_object_set(G_OBJECT(filesrc),
                 GST_PROP_LOCATION.c_str(), 
                 asset_quality_descriptor->composed_path.c_str(),
                 NULL);
    decodebin    = gst_element_factory_make(GST_ELEM_DECODEBIN.c_str(),
                                            GetGstElementId(GST_ELEM_DECODEBIN).c_str());
    g_signal_connect(decodebin, GST_EVENT_PAD_ADDED.c_str(),
                     G_CALLBACK(ImageActor::OnDecodebinPadAdded),
                     this);
    videoconvert = gst_element_factory_make(GST_ELEM_VIDEOCONVERT.c_str(),
                                            GetGstElementId(GST_ELEM_VIDEOCONVERT).c_str());
    imagefreeze  = gst_element_factory_make(GST_ELEM_IMAGEFREEZE.c_str(),
                                            GetGstElementId(GST_ELEM_IMAGEFREEZE).c_str());
    videoscale   = gst_element_factory_make(GST_ELEM_VIDEOSCALE.c_str(),
                                            GetGstElementId(GST_ELEM_VIDEOSCALE).c_str());
    if (actor->resize_mode == Actor::ResizeMode::AspectBorderTransparent ||
        actor->resize_mode == Actor::ResizeMode::AspectNone) {
      g_object_set(G_OBJECT(videoscale),
                   "add-borders",
                   false,
                   NULL);
    }
    filter       = gst_element_factory_make(GST_ELEM_CAPSFILTER.c_str(),
                                            GetGstElementId(GST_ELEM_CAPSFILTER).c_str());
    CheckGstElements({{GST_ELEM_FILESRC, filesrc},
                      {GST_ELEM_DECODEBIN, decodebin},
                      {GST_ELEM_VIDEOCONVERT, videoconvert},
                      {GST_ELEM_IMAGEFREEZE, imagefreeze},
                      {GST_ELEM_VIDEOSCALE, videoscale},
                      {GST_ELEM_CAPSFILTER, filter}});

    gst_bin_add_many(GST_BIN(bin),
                     filesrc,
                     decodebin,
                     videoconvert,
                     imagefreeze,
                     videoscale,
                     filter, NULL);

    gst_element_link_many(filesrc, decodebin, NULL);
    gst_element_link_many(videoconvert, imagefreeze, videoscale, filter, NULL);

    // Video Src to Bin
    video_src_pad = gst_element_get_static_pad (filter, M_GST_PAD_SRC.c_str());
    gst_element_add_pad(bin, gst_ghost_pad_new(M_GST_PAD_VIDEO_SRC.c_str(), video_src_pad));
    gst_object_unref(GST_OBJECT (video_src_pad));
  }
};

void ImageActor::SetGstElements() {
  SetDimensions(actor->width, actor->height);
  SetX(actor->x);
  SetY(actor->y);
  SetZ(actor->z);
};

void ImageActor::SetDimensions(double width, double height) {
  this->width = width;
  this->height = height;
  GstCaps* filter_caps;
  int scene_width = 640;
  int scene_height = 480;
  area_width_px = Math::RelativeToPx(width, scene_width);
  area_height_px = Math::RelativeToPx(height, scene_height);
  switch (actor->resize_mode) {
    case Actor::ResizeMode::AspectBorderTransparent:
      if (width > height) {
        height_px = area_height_px;
        width_px = height_px *
                   asset_quality_descriptor->natural_width /
                   asset_quality_descriptor->natural_height;
      } else {
        width_px = area_width_px;
        height_px = width_px *
                    asset_quality_descriptor->natural_height /
                    asset_quality_descriptor->natural_width;
      }
      break;
    case Actor::ResizeMode::AspectBorderBlack:
    case Actor::ResizeMode::AspectNone:
    case Actor::ResizeMode::AspectCrop:
      width_px = area_width_px;
      height_px = area_height_px;
      break;
  }
  filter_caps = gst_caps_new_simple ("video/x-raw",
         GST_PROP_WIDTH.c_str(), G_TYPE_INT, width_px,
         GST_PROP_HEIGHT.c_str(), G_TYPE_INT, height_px,
         NULL);
  g_object_set(G_OBJECT (filter), "caps", filter_caps, NULL);
  gst_caps_unref(filter_caps);
};

void ImageActor::SetX(double x) {
  this->x = x;
  if (actor->resize_mode == Actor::ResizeMode::AspectBorderTransparent) {
    g_object_set(G_OBJECT(videomixer_pad),
                 "xpos",
                 Math::RelativeToPx(x, 640) + ((area_width_px - width_px) / 2),
                 NULL);
  } else {
    g_object_set(G_OBJECT(videomixer_pad),
                 "xpos",
                 Math::RelativeToPx(x, 640),
                 NULL);
  }
};

void ImageActor::SetY(double y) {
  this->y = y;
  if (actor->resize_mode == Actor::ResizeMode::AspectBorderTransparent) {
    g_object_set(G_OBJECT(videomixer_pad),
                 "ypos",
                 Math::RelativeToPx(y, 480) + ((area_height_px - height_px) / 2),
                 NULL);
  } else {
    g_object_set(G_OBJECT(videomixer_pad),
                 "ypos",
                 Math::RelativeToPx(y, 480),
                 NULL);
  }
};

void ImageActor::SetZ(int z) {
  this->z = z;
  g_object_set(G_OBJECT(videomixer_pad),
                 "zorder",
                 z,
                 NULL);
};

void ImageActor::OnDecodebinPadAdded(GstElement* decodebin,
                                     GstPad* pad,
                                     ImageActor* image_actor) {
  GstCaps          *new_pad_caps = NULL;
  GstStructure     *new_pad_struct = NULL;
  const gchar      *new_pad_type = NULL;
  GstPad           *sink_pad = NULL;
  GstPadLinkReturn  ret;

  new_pad_caps   = gst_pad_query_caps(pad, NULL);
  new_pad_struct = gst_caps_get_structure(new_pad_caps, 0);
  new_pad_type   = gst_structure_get_name(new_pad_struct);

  std::string s_pad_type = std::string(new_pad_type);
  LOG_TRACE("Trying to link pads" << std::string(new_pad_type), LOGGER_PIPELINE);
  if (g_str_has_prefix(new_pad_type, "video/x-raw")) {
    sink_pad = gst_element_get_static_pad(image_actor->videoconvert, "sink");
  }
  if (!gst_pad_is_linked(sink_pad))
  {
    ret = gst_pad_link(pad, sink_pad);
    LOG_TRACE("Pads have been linked.", LOGGER_PIPELINE);
  }
  gst_caps_unref(new_pad_caps);
  gst_object_unref(sink_pad);
};
