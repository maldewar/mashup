#include "EmptyValue.h"

EmptyValue::EmptyValue() : BaseValue(0) {
};

std::string EmptyValue::ToString() {
  return "<empty>";
};
