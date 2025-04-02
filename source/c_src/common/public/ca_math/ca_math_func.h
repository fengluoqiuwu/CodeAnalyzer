//
// Created by Eden_ on 2025/3/31.
//

#ifndef CA_MATH_FUNC_H
#define CA_MATH_FUNC_H

#include "ca_int.h"

#include <cassert>
#include <limits>
#include <type_traits>

namespace ca::ca_math {

// Define Py_MAX as a template function
template <typename T>
constexpr T ca_max(T x, T y) {
    static_assert(std::is_arithmetic<T>::value, "Py_MAX only works with arithmetic types.");
    return (x > y) ? x : y;
}

// Define Py_MIN as a template function
template <typename T>
constexpr T ca_min(T x, T y) {
    static_assert(std::is_arithmetic<T>::value, "Py_MIN only works with arithmetic types.");
    return (x < y) ? x : y;
}

template <typename Narrow, typename Wide>
constexpr Narrow ca_safe_downcast(Wide value) {
    // Check if Narrow is an integer type
    static_assert(std::is_integral<Narrow>::value, "Narrow type must be an integer type.");
    static_assert(std::is_integral<Wide>::value, "Wide type must be an integer type.");

    // Check that the value is within the range of Narrow
    assert(value >= static_cast<Wide>(std::numeric_limits<Narrow>::min()) &&
           value <= static_cast<Wide>(std::numeric_limits<Narrow>::max()));

    // Perform the downcast
    return static_cast<Narrow>(value);
}// TODO

}

#endif //CA_MATH_FUNC_H
