#ifndef MASHUP_UTIL_MATH_
#define MASHUP_UTIL_MATH_

#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

#include "Log.h"

/**
 * @brief Utility to get.
 *
 * The methods provide convenient manipulation
 * for strings.
 * @author Waldemar Sánchez
 * @date March, 2014
 */
class Math {
  private:
    Math();

  public:
    /**
     * Get the pixel coordinates given a relative value.
     * @param relative Relative value.
     * @param px_limit Number of pixel for relative value of 1.
     */
    static inline int RelativeToPx(double relative, int px_limit) {
      return (int)(px_limit * relative);
    };
    /**
     * Get the nano equivalent of a milli scale value.
     * @param milli_value Value in milli scale.
     * @return Value in nano scale.
     */
    template<typename Type> static inline long int MilliToNano(Type milli_value) {
      return (milli_value * 1000000);
    };
    /**
     * Get the milli equivalent of a nano scale value.
     * @param nano_value Value in nano scale.
     * @return Value in milli scale.
     */
    template<typename Type> static inline int NanoToMilli(Type nano_value) {
      return (nano_value / 1000000);
    };

};

#endif // MASHUP_UTIL_MATH_
