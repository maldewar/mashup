#include "SingleValue.h"

SingleValue::SingleValue(double value) : BaseValue(1) {
  this->value = value;
};

void SingleValue::Set(double value) {
  this->value = value;
};

double SingleValue::Get() {
  return value;
};
