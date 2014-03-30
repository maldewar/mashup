#include "Actor.h"

Actor::Actor() {
  asset_id = 0;
  instance_id = 0;
  type = AssetDescriptor::Type::Undef;
  x = 0;
  y = 0;
  z = 0;
  width = 0;
  height = 0;
  enters_at = 0;
  offset = 0;
  exits_at = 0;
  resize_mode = Actor::ResizeMode::AspectBorderTransparent;
};

std::string Actor::Get(Actor::ResizeMode resize_mode) {
  switch(resize_mode) {
    case Actor::ResizeMode::AspectBorderBlack:
      return RESIZE_ASPECT_BORDER_BLACK;
    case Actor::ResizeMode::AspectBorderTransparent:
      return RESIZE_ASPECT_BORDER_TRANSPARENT;
    case Actor::ResizeMode::AspectNone:
      return RESIZE_ASPECT_NONE;
    case Actor::ResizeMode::AspectCrop:
      return RESIZE_ASPECT_CROP;
  }
  return RESIZE_ASPECT_BORDER_TRANSPARENT;
};

Actor::ResizeMode Actor::Get(const std::string resize_mode) {
  if (resize_mode.compare(RESIZE_ASPECT_BORDER_TRANSPARENT) == 0)
    return Actor::ResizeMode::AspectBorderTransparent;
  if (resize_mode.compare(RESIZE_ASPECT_BORDER_BLACK) == 0)
    return Actor::ResizeMode::AspectBorderBlack;
  if (resize_mode.compare(RESIZE_ASPECT_NONE) == 0)
    return Actor::ResizeMode::AspectNone;
  if (resize_mode.compare(RESIZE_ASPECT_CROP) == 0)
    return Actor::ResizeMode::AspectCrop;
  return Actor::ResizeMode::AspectBorderTransparent;
};
