#include "ValueBuilder.h"

ValueBuilder::ValueBuilder() {};

EmptyValue* ValueBuilder::Get() {
  EmptyValue* value = new EmptyValue();
  return value;
};

BaseValue* ValueBuilder::Get(BaseProperty::Target target,
              std::initializer_list<double> values) {
  std::vector<double> v_values;
  for(double i : values)
  {
    v_values.push_back(i);
  }
  switch(target) {
    case BaseProperty::Target::Rotation:
    case BaseProperty::Target::Scale:
    case BaseProperty::Target::Opacity:
    case BaseProperty::Target::Volume:
      if (values.size() >= 1)
        return Get(v_values.at(0));
      break;
    case BaseProperty::Target::Position:
    case BaseProperty::Target::Dimension:
      if (values.size() >= 2)
        return Get(v_values.at(0), v_values.at(1));
      break;
    case BaseProperty::Target::Color:
      if (values.size() >= 3)
        return Get(v_values.at(0), v_values.at(1), v_values.at(2));
      break;
  }
  return Get();
};

SingleValue* ValueBuilder::Get(double value) {
  SingleValue* single_value = new SingleValue(value);
  return single_value;
};

DoubleValue* ValueBuilder::Get(double value_a, double value_b) {
  DoubleValue* double_value = new DoubleValue(value_a, value_b);
  return double_value;
};

TripleValue* ValueBuilder::Get(double value_a,
                               double value_b,
                               double value_c) {
  TripleValue* triple_value = new TripleValue(value_a, value_b, value_c);
  return triple_value;
};

int ValueBuilder::GetTupleSize(BaseProperty::Target target) {
  switch(target) {
    case BaseProperty::Target::Rotation:
    case BaseProperty::Target::Scale:
    case BaseProperty::Target::Opacity:
    case BaseProperty::Target::Volume:
      return 1;
    case BaseProperty::Target::Position:
    case BaseProperty::Target::Dimension:
      return 2;
    case BaseProperty::Target::Color:
      return 3;
  }
  return 0;
};
