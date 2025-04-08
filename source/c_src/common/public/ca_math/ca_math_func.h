// ================================
// CodeAnalyzer - source/c_src/common/public/ca_math/ca_math_func.h
//
// @file
// @brief Defines core mathematical functions.
// ================================

#ifndef CA_MATH_FUNC_H
#define CA_MATH_FUNC_H

namespace ca::ca_math {

/**
 * @brief Returns the maximum of two arithmetic values.
 *
 * @tparam T An arithmetic type (e.g., int, float, double).
 * @param x First value.
 * @param y Second value.
 * @return The greater of x and y.
 */
template <typename T>
constexpr T ca_max(T x, T y);

/**
 * @brief Returns the minimum of two arithmetic values.
 *
 * @tparam T An arithmetic type (e.g., int, float, double).
 * @param x First value.
 * @param y Second value.
 * @return The lesser of x and y.
 */
template <typename T>
constexpr T ca_min(T x, T y);

/**
 * @brief Safely downcasts a wide integer type to a narrower integer type.
 *
 * Ensures at runtime (via assert) that the value fits in the target type.
 *
 * @tparam Narrow Target (narrower) integer type.
 * @tparam Wide Source (wider) integer type.
 * @param value The value to downcast.
 * @return The downcast value as type Narrow.
 *
 * @note This function uses assert, which is only active in debug builds.
 */
template <typename Narrow, typename Wide>
constexpr Narrow ca_safe_downcast(Wide value);

}

// Include the implementation file
#include "../../private/ca_math/ca_math_func.tpp"

#endif //CA_MATH_FUNC_H
