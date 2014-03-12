#ifndef MASHUP_COMMON_TRIPLE_VALUE_
#define MASHUP_COMMON_TRIPLE_VALUE_

#include "BaseValue.h"
#include <tuple>

/**
 * @brief Triple value container.
 *
 * Triplet value container class.
 * @author Waldemar Sánchez
 * @date February, 2014
 */
class TripleValue : virtual public BaseValue {
  public:
    /**
     * Class constructor.
     */
    TripleValue(double value_a, double value_b, double value_c);
    /**
     * Sets the value to the class.
     * @param value_a First value of the tuple.
     * @param value_b Second value of the tuple.
     * @param value_c Thrid value of the tuple.
     */
    void Set(double value_a, double value_b, double value_c);
    /**
     * Gets the value tuple set for this object.
     * @return Tuple for this value.
     */
    std::tuple<double, double, double> Get();
    /**
     * String representation for the contained value.
     * @return Print ready string.
     */
    std::string ToString();

  protected:
    /**
     * Container of the tuple value.
     */
    std::tuple<double,double, double> value;

};

#endif // MASHUP_COMMON_TRIPLE_VALUE_
