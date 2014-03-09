#ifndef MASHUP_COMMON_DOUBLE_VALUE_
#define MASHUP_COMMON_DOUBLE_VALUE_

#include "BaseValue.h"
#include <tuple>

/**
 * @brief Double value container.
 *
 * Double value container class.
 * @author Waldemar Sánchez
 * @date February, 2014
 */
class DoubleValue : public BaseValue {
  public:
    /**
     * Class constructor.
     */
    DoubleValue(double value_a, double value_b);
    /**
     * Sets the two values contained in the tuple.
     * @param value_a First value of the tuple.
     * @param value_b Second value of the tuple.
     */
    void Set(double value_a, double value_b);
    /**
     * Gets the tuple contained by this object.
     * @return Tuple value.
     */
    std::tuple<double, double> Get();

  protected:
    /**
     * Container of the tuple value.
     */
    std::tuple<double,double> value;

};

#endif // MASHUP_COMMON_DOUBLE_VALUE_
