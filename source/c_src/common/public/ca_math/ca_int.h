// ================================
// CodeAnalyzer - source/c_src/common/public/ca_math/ca_int.h
//
// @file
// @brief Defines signed and unsigned integer types for various
//        bit widths base on <cstdint>.
// ================================

#ifndef CA_INT_H
#define CA_INT_H

#include<cstdint>
#include <limits>

namespace ca{

typedef int8_t ca_int8_t;
typedef int16_t ca_int16_t;
typedef int32_t ca_int32_t;
typedef int64_t ca_int64_t;

typedef uint8_t ca_uint8_t;
typedef uint16_t ca_uint16_t;
typedef uint32_t ca_uint32_t;
typedef uint64_t ca_uint64_t;

typedef size_t ca_size_t;
typedef ssize_t ca_ssize_t;

constexpr ca_size_t CA_INT8_MIN = INT8_MIN;
constexpr ca_size_t CA_INT16_MIN = INT16_MIN;
constexpr ca_size_t CA_INT32_MIN = INT32_MIN;
constexpr ca_size_t CA_INT64_MIN = INT64_MIN;

constexpr ca_size_t CA_INT8_MAX = INT8_MAX;
constexpr ca_size_t CA_INT16_MAX = INT16_MAX;
constexpr ca_size_t CA_INT32_MAX = INT32_MAX;
constexpr ca_size_t CA_INT64_MAX = INT64_MAX;

constexpr ca_size_t CA_UINT8_MAX = UINT8_MAX;
constexpr ca_size_t CA_UINT16_MAX = UINT16_MAX;
constexpr ca_size_t CA_UINT32_MAX = UINT32_MAX;
constexpr ca_size_t CA_UINT64_MAX = UINT64_MAX;

constexpr ca_size_t CA_SIZE_T_MAX = std::numeric_limits<std::size_t>::max();
constexpr ca_size_t CA_SSIZE_T_MIN = std::numeric_limits<std::size_t>::min();
constexpr ca_size_t CA_SSIZE_T_MAX = std::numeric_limits<std::size_t>::max();

}

#endif //CA_INT_H
