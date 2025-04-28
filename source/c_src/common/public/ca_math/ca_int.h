// ================================
// CodeAnalyzer - source/c_src/common/public/ca_math/ca_int.h
//
// @file
// @brief Defines signed and unsigned integer types for various
//        bit widths base on <cstdint>.
// ================================

#ifndef CA_INT_H
#define CA_INT_H

#include <cstdint>
#include <limits>

#ifndef CA_OS_WINDOWS
#include <stddef.h>
#include <unistd.h>
#endif

#ifdef _MSC_VER
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#endif

namespace ca{

// Type aliases
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

// Common C-style aliases
typedef short ca_short_t;
typedef unsigned short ca_ushort_t;
typedef int ca_int_t;
typedef unsigned int ca_uint_t;
typedef long ca_long_t;
typedef unsigned long ca_ulong_t;
typedef long long ca_longlong_t;
typedef unsigned long long ca_ulonglong_t;

// Integer limits
constexpr ca_int8_t CA_INT8_MIN = INT8_MIN;
constexpr ca_int8_t CA_INT8_MAX = INT8_MAX;
constexpr ca_int16_t CA_INT16_MIN = INT16_MIN;
constexpr ca_int16_t CA_INT16_MAX = INT16_MAX;
constexpr ca_int32_t CA_INT32_MIN = INT32_MIN;
constexpr ca_int32_t CA_INT32_MAX = INT32_MAX;
constexpr ca_int64_t CA_INT64_MIN = INT64_MIN;
constexpr ca_int64_t CA_INT64_MAX = INT64_MAX;

constexpr ca_uint8_t CA_UINT8_MAX = UINT8_MAX;
constexpr ca_uint16_t CA_UINT16_MAX = UINT16_MAX;
constexpr ca_uint32_t CA_UINT32_MAX = UINT32_MAX;
constexpr ca_uint64_t CA_UINT64_MAX = UINT64_MAX;

// Min/max values for other types using std::numeric_limits
constexpr ca_short_t CA_SHORT_MIN = std::numeric_limits<ca_short_t>::min();
constexpr ca_short_t CA_SHORT_MAX = std::numeric_limits<ca_short_t>::max();
constexpr ca_ushort_t CA_USHORT_MAX = std::numeric_limits<ca_ushort_t>::max();

constexpr ca_int_t CA_INT_MIN = std::numeric_limits<ca_int_t>::min();
constexpr ca_int_t CA_INT_MAX = std::numeric_limits<ca_int_t>::max();
constexpr ca_uint_t CA_UINT_MAX = std::numeric_limits<ca_uint_t>::max();

constexpr ca_long_t CA_LONG_MIN = std::numeric_limits<ca_long_t>::min();
constexpr ca_long_t CA_LONG_MAX = std::numeric_limits<ca_long_t>::max();
constexpr ca_ulong_t CA_ULONG_MAX = std::numeric_limits<ca_ulong_t>::max();

constexpr ca_longlong_t CA_LONGLONG_MIN = std::numeric_limits<ca_longlong_t>::min();
constexpr ca_longlong_t CA_LONGLONG_MAX = std::numeric_limits<ca_longlong_t>::max();
constexpr ca_ulonglong_t CA_ULONGLONG_MAX = std::numeric_limits<ca_ulonglong_t>::max();

constexpr ca_size_t CA_SIZE_T_MAX = std::numeric_limits<ca_size_t>::max();
constexpr ca_ssize_t CA_SSIZE_T_MIN = std::numeric_limits<ca_ssize_t>::min();
constexpr ca_ssize_t CA_SSIZE_T_MAX = std::numeric_limits<ca_ssize_t>::max();

}

#endif //CA_INT_H
