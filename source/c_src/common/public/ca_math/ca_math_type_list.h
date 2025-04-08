// ================================
// CodeAnalyzer - source/c_src/common/public/ca_math/ca_math_type_list.h
//
// @file
// @brief Defines mathematical type lists for ca_math types.
// ================================

#ifndef CA_MATH_TYPE_LIST_H
#define CA_MATH_TYPE_LIST_H

#include "ca_int.h"
#include "ca_float.h"

#include <tuple>

namespace ca {

/**
 * @brief Tuple of all supported integer types.
 *
 * This includes signed and unsigned fixed-width integers, platform-specific types,
 * and standard C-style integer types.
 */
using ca_integer_types = std::tuple<
    ca_int8_t, ca_int16_t, ca_int32_t, ca_int64_t,
    ca_uint8_t, ca_uint16_t, ca_uint32_t, ca_uint64_t,
    ca_size_t, ca_ssize_t,
    ca_short_t, ca_int_t, ca_long_t, ca_longlong_t,
    ca_ushort_t, ca_uint_t, ca_ulong_t, ca_ulonglong_t
>;

/**
 * @brief Tuple of all supported floating-point types.
 *
 * Includes IEEE 754 float and double types.
 */
using ca_floating_types = std::tuple<
    ca_float32_t, ca_float64_t
>;

/**
 * @brief Tuple of all numeric real types (integer + floating-point).
 */
using ca_real_types = decltype(std::tuple_cat(
    ca_integer_types{}, ca_floating_types{}
));

}

#endif //CA_MATH_TYPE_LIST_H
