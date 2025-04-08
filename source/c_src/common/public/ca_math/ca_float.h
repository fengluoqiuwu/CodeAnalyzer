// ================================
// CodeAnalyzer - source/c_src/common/public/ca_math/ca_float.h
//
// @file
// @brief Defines float types for various bit widths.
// ================================

#ifndef CA_FLOAT_H
#define CA_FLOAT_H

#include <limits>

namespace ca {

// ============ Floating-Point Type Aliases ============

static_assert(sizeof(float) * 8 == 32, "float is not 32 bits");
static_assert(sizeof(double) * 8 == 64, "double is not 64 bits");

typedef float ca_float32_t;
typedef double ca_float64_t;

// ============ Floating-Point Limits ============

constexpr ca_float32_t CA_FLOAT32_MIN = std::numeric_limits<ca_float32_t>::lowest();
constexpr ca_float32_t CA_FLOAT32_MAX = std::numeric_limits<ca_float32_t>::max();
constexpr ca_float32_t CA_FLOAT32_EPSILON = std::numeric_limits<ca_float32_t>::epsilon();

constexpr ca_float64_t CA_FLOAT64_MIN = std::numeric_limits<ca_float64_t>::lowest();
constexpr ca_float64_t CA_FLOAT64_MAX = std::numeric_limits<ca_float64_t>::max();
constexpr ca_float64_t CA_FLOAT64_EPSILON = std::numeric_limits<ca_float64_t>::epsilon();

}

#endif //CA_FLOAT_H
