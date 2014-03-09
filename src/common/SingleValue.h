#ifndef MASHUP_COMMON_SINGLE_VALUE_
#define MASHUP_COMMON_SINGLE_VALUE_

#include "BaseValue.h"

/**
 * @brief Single value container.
 *
 * Single value container class.
 * @author Waldemar Sánchez
 * @date February, 2014
 */
class SingleValue : public BaseValue {
  public:
    /**
     * Class constructor.
     * @param value Value contained by the object.
     */
    SingleValue(double value);
    /**
     * Sets the value contained in the instance.
     * @param value Value to set.
     */
    void Set(double value);
    /**
     * Gets the value contained in the instance.
     * @return Value contained.
     */
    double Get();

  protected:
    /**
     * Value for the instance.
     */
    double value;

};

#endif // MASHUP_COMMON_SINGLE_VALUE_
