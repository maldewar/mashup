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
};
