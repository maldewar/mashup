#include "SingleValue.h"

#include <sstream>

SingleValue::SingleValue(double value) : BaseValue(1) {
  this->value = value;
};

void SingleValue::Set(double value) {
  this->value = value;
};

double SingleValue::Get() {
  return value;
};

std::string SingleValue::ToString() {
  std::ostringstream stream;
  stream << value;
  return stream.str();
};
