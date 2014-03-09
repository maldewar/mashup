#include "DoubleValue.h"

DoubleValue::DoubleValue(double value_a, double value_b) : BaseValue(2) {
  value = std::make_tuple(value_a, value_b);;
};

void DoubleValue::Set(double value_a, double value_b) {
  std::get<0>(value) = value_a;
  std::get<1>(value) = value_b;
};

std::tuple<double, double> DoubleValue::Get() {
  return value;
};