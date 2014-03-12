#ifndef MASHUP_COMMON_BUILDER_VALUEBUILDER_
#define MASHUP_COMMON_BUILDER_VALUEBUILDER_

#include <initializer_list>
#include <vector>

#include "ValueBuilder.h"
#include "../EmptyValue.h"
#include "../SingleValue.h"
#include "../DoubleValue.h"
#include "../TripleValue.h"
#include "../BaseProperty.h"

/**
 * @brief Creates Value instances from BaseValue.
 *
 * The buider is a helper in getting the appropiate values
 * according to the property these represent.
 * @author Waldemar Sánchez
 * @date February, 2014
 */
class ValueBuilder {
  public:
    /**
     * Gets an empty value instance.
     * @return Empty value instance.
     */
    static EmptyValue* Get();
    /**
     * Gets a BaseValue derived class according to the 
     * property represented.
     * @param target Property enum.
     * @param values List of values to assign to the instance.
     * @return Instance containing a tuple of values according
     *         to the property, if no property is matched an
     *         empty value instance is returned instead.
     */
    static BaseValue* Get(BaseProperty::Target target,
                          std::initializer_list<double> values);
    /**
     * Builds a single value instance.
     * @param value Init value.
     * @return Single value instance.
     */
    static SingleValue* Get(double value);
    /**
     * Builds a double value instance.
     * @param value_a First element of the tuple.
     * @param value_b Second element of the tuple.
     * @return Double value instance.
     */
    static DoubleValue* Get(double value_a, double value_b);
    /**
     * Builds a triple value instance.
     * @param value_a First element of the tuple.
     * @param value_b Second element of the tuple.
     * @param value_c Third element of the tuple.
     * @return Triple value instance.
     */
    static TripleValue* Get(double value_a, double value_b, double value_c);
    /**
     * Returns the number of expected values for a tuple
     * given the property represented.
     * @param target Property enum.
     * @return Number of elements for the tuple.
     */
    static int GetTupleSize(BaseProperty::Target target);

  private:
    /**
     * Private constructor.
     */
    ValueBuilder();
};

#endif // MASHUP_COMMON_BUILDER_VALUEBUILDER_
