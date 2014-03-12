#ifndef MASHUP_COMMON_EMPTY_VALUE_
#define MASHUP_COMMON_EMPTY_VALUE_

#include "BaseValue.h"

/**
 * @brief Base class for all values.
 *
 * Value-container class should extend this class.
 * @author Waldemar Sánchez
 * @date February, 2014
 */
class EmptyValue : virtual public BaseValue {
  public:
    /**
     * Class constructor.
     */
    EmptyValue();
    std::string ToString();
    /*friend std::ostream& operator<< (std::ostream& stream,
                                     const EmptyValue& empty_value) {
      return stream << "<empty>";
    };*/

};

#endif // MASHUP_COMMON_EMPTY_VALUE_
