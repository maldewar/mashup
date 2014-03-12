#include "TripleValue.h"

#include <sstream>

TripleValue::TripleValue(double value_a, double value_b, double value_c) : BaseValue(3) {
  value = std::make_tuple(value_a, value_b, value_c);;
};

void TripleValue::Set(double value_a, double value_b, double value_c) {
  std::get<0>(value) = value_a;
  std::get<1>(value) = value_b;
  std::get<2>(value) = value_c;
};

std::tuple<double, double, double> TripleValue::Get() {
  return value;
};

std::string TripleValue::ToString() {
  std::ostringstream stream;
  stream << "("<< std::get<0>(value)
         << ", " << std::get<1>(value)
         << ", " << std::get<2>(value) << ")";
  return stream.str();
};
