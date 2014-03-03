#ifndef MASHUP_UTIL_PRINT_
#define MASHUP_UTIL_PRINT_

#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

/**
 * @brief Utility to format strings.
 *
 * The methods provide convenient manipulation
 * for strings.
 * @author Waldemar Sánchez
 * @date February, 2014
 */
class Print {
  public:
    /**
     * Character used to tabulate columns.
     */
    static char separator;
    /**
     * String to concatenate a field-value duple.
     */
    static std::string field_separator;
    /**
     * Number of spaces a tabulates column occupies.
     */
    static int field_width;

  public:
    /**
     * Print a single value using tab spaces, there is no new line
     * after.
     * @param t       Value to print.
     * @param sstream String stream to append the string.
     * @param indent  Identation spaces.
     */
    template<typename T>
    static void Simple(T t, std::stringstream& sstream, int indent = 0) {
      sstream << std::string(indent, ' ') << std::left 
              << std::setw(field_width) << std::setfill(' ') << t;
    };
    /**
     * Print a field-value duple using tabular column spaces, there's a
     * new line generated after.
     * @param t       Field name to print.
     * @param u       Value to print.
     * @param sstream String stream to append the string.
     * @param indent  Identation spaces.
     */
    template<typename T, typename U>
    static void Duple(T t, U u, std::stringstream& sstream, int indent = 0) {
      sstream << std::string(indent, ' ') << std::left << std::setw(field_width)
              << std::setfill(separator) << t << field_separator << " "
              << std::left << std::setw(field_width)
              << std::setfill(separator) << u << std::endl;
    };

  private:
    /**
     * Private constructor for static class.
     */
    Print();

};

char Print::separator = ' ';
std::string Print::field_separator = "";
int Print::field_width = 10;

#endif // MASHUP_UTIL_PRINT_
