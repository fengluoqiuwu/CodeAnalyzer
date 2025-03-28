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
}

#endif //CA_INT_H
