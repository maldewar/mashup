#include "Transition.h"

Transition::Transition() {
  asset_id = 0;
  instance_id = 0;
  target = Transition::Target::Undef;
  start_value = nullptr;
  end_value = nullptr;
  starts_at = 0;
  duration = 0;
  easing = Transition::Easing::Linear;
};
