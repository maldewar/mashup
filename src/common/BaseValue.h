#ifndef MASHUP_COMMON_BASE_VALUE_
#define MASHUP_COMMON_BASE_VALUE_

/**
 * @brief Base class for all values.
 *
 * Value-container class should extend this class.
 * @author Waldemar Sánchez
 * @date February, 2014
 */
class BaseValue {
  public:
    /**
     * Class constructor.
     */
    BaseValue(int size);

  protected:
    /**
     * Indicates the number of values
     * this implentation holds.
     */
    int tuple_size;

};

#endif // MASHUP_COMMON_BASE_VALUE_
