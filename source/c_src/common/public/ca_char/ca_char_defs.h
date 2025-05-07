// ================================
// CodeAnalyzer - source/c_src/common/public/ca_char/ca_char_defs.h
//
// @file
// @brief Defines constants and types using in ca_char library.
// ================================

#ifndef CA_CHAR_DEFS_H
#define CA_CHAR_DEFS_H

// TODO please include this in math_core libray and delete the define there
#include <cstdint>
typedef uint8_t ca_uint8_t;
typedef uint16_t ca_uint16_t;
typedef uint32_t ca_uint32_t;
typedef int8_t ca_int8_t;
typedef int16_t ca_int16_t;
typedef int32_t ca_int32_t;

namespace ca {

typedef ca_uint8_t ca_char_t; /**< Represents a 1-byte character. */
typedef ca_uint16_t ca_char2_t; /** Represents a 2-byte character. */
typedef ca_uint32_t ca_char4_t; /** Represents a 4-byte character. */

namespace ca_char
{

/**
 * @enum ca_encoding_t
 * @brief Enum that defines the supported character encodings.
 */
enum class ca_encoding_t {
    ENCODING_ASCII,      /**< ASCII encoding */
    ENCODING_UTF8,       /**< UTF-8 encoding */
    ENCODING_UTF16,      /**< UTF-16 encoding */
    ENCODING_UTF32,      /**< UTF-32 encoding */
    ENCODING_GBK,        /**< GBK encoding */
    ENCODING_LATIN1,     /**< LATIN-1 encoding */
    ENCODING_UNKNOWN     /**< Unknown encoding */
};

typedef ca_char_t ca_ascii_t;   /**< Represents an ASCII character. */
typedef ca_char_t ca_utf8_t;    /**< Represents a UTF-8 character. */
typedef ca_char2_t ca_utf16_t;  /**< Represents a UTF-16 character. */
typedef ca_char4_t ca_utf32_t;  /**< Represents a UTF-32 character. */
typedef ca_char_t ca_gbk_t;     /**< Represents a GBK character. */
typedef ca_char_t ca_latin1_t;  /**< Represents a LATIN-1 character. */

/**
 * @brief Maps a character encoding enum to its corresponding character type.
 *
 * @tparam encoding The character encoding.
 */
template<ca_encoding_t encoding>
struct ca_char_type_map;

/**
 * @brief Retrieves the character type corresponding to a given encoding.
 *
 * @tparam encoding The encoding value from ca_encoding_t.
 */
template<ca_encoding_t encoding>
using ca_char_type_t = typename ca_char_type_map<encoding>::type;

} // namespace ca_char
} // namespace ca

#endif //CA_CHAR_DEFS_H
