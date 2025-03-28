// ================================
// CodeAnalyzer - source/c_src/common/public/ca_string/ca_char_types.h
//
// @file
// @brief Defines core character types.
// ================================

#ifndef CA_CHAR_TYPES_H
#define CA_CHAR_TYPES_H

#include "ca_math.h"

namespace ca::ca_string {
/**
 * @typedef ca_char_t
 * @brief Represents a raw byte-based character unit.
 *        This type is used for holding individual characters as bytes before decoding.
 *        Use decoding functions to extract actual Unicode code points.
 */
typedef ca_uint8_t ca_char_t; /**< Raw character unit (single byte) */

/**
 * @typedef ca_char4_t
 * @brief Represents a 4-byte character unit (a Unicode code point).
 *        This type is used for storing decoded characters as full Unicode code points.
 */
typedef ca_uint32_t ca_char4_t;

/**
 * @constant CA_ERROR_VALUE
 * @brief Error value used to indicate unsupported encodings.
 *        This value is returned when an unsupported encoding type is encountered.
 */
constexpr ca_char4_t CA_ERROR_VALUE = 0xFFFFFFFF;  ///< 32-bit error value
}

#endif //CA_CHAR_TYPES_H
