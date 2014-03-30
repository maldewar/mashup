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
    static inline int RelativeToPx(double relative, int px_limit) {
      return (int)(px_limit * relative);
    };

};

#endif // MASHUP_UTIL_MATH_
