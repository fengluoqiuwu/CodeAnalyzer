// ================================
// CodeAnalyzer - source/c_src/common/public/ca_math/detail/ca_math_func.tpp
//
// @file
// @brief Implements core mathematical functions.
// ================================
#pragma once

#include <cassert>
#include <limits>
#include <type_traits>

namespace ca::ca_math {

template <typename T>
constexpr T ca_max(T x, T y) {
    static_assert(std::is_arithmetic_v<T>, "Py_MAX only works with arithmetic types.");
    return (x > y) ? x : y;
}

template <typename T>
constexpr T ca_min(T x, T y) {
    static_assert(std::is_arithmetic_v<T>, "Py_MIN only works with arithmetic types.");
    return (x < y) ? x : y;
}

template <typename Narrow, typename Wide>
constexpr Narrow ca_safe_downcast(Wide value) {
    static_assert(std::is_integral_v<Narrow>, "Narrow type must be an integer type.");
    static_assert(std::is_integral_v<Wide>, "Wide type must be an integer type.");

    assert(value >= static_cast<Wide>(std::numeric_limits<Narrow>::min()) &&
           value <= static_cast<Wide>(std::numeric_limits<Narrow>::max()));

    return static_cast<Narrow>(value);
}

}
