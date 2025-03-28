// ================================
// CodeAnalyzer - source/c_src/common/public/ca_string/ca_char.h
//
// @file
// @brief Defines core character types and utility functions for encoding-aware
//        character handling.
// ================================

#ifndef CA_CHAR_H
#define CA_CHAR_H

#include "ca_char_types.h"

namespace ca::ca_string {
/**
 * @enum ca_encoding_t
 * @brief Enum that defines the supported character encodings.
 */
enum class ca_encoding_t {
    CA_ENCODING_ASCII,      /**< ASCII encoding */
    CA_ENCODING_UTF8,       /**< UTF-8 encoding */
    CA_ENCODING_UTF32       /**< UTF-32 encoding */
};

/**
 * @brief Template function to retrieve the size (in bytes) of a character based on its encoding.
 *
 * @param c Pointer to the character whose byte size is to be determined.
 *
 * @return The size of the character in bytes based on the specified encoding.
 *         Returns `-1` if the encoding is unsupported.
 * @tparam encoding The character encoding type to calculate the byte size.
 *
 * @note This function assumes that the character `c` is properly encoded according to the specified
 *       `encoding`. If the encoding is unsupported or the character is invalid, the function will
 *       return `-1`.
 */
template <ca_encoding_t encoding>
inline int
ca_get_bytes(const ca_char_t *c);

/**
 * @brief Template function to retrieve the next character from a byte stream.
 *
 * @param c Pointer to the byte stream representing the character.
 * @param size Pointer to an integer that will hold the size of the character in bytes.
 *
 * @return The decoded character as a 4-byte Unicode code point.
 * @tparam encoding The character encoding type to decode.
 *
 * @note For unsupported encoding types, this function will return `CA_ERROR_VALUE`.
 */
template <ca_encoding_t encoding>
inline ca_char4_t
ca_getchar(const ca_char_t *c, int *size);

/**
 * @enum ca_char_check_types
 * @brief Enumeration for character check types.
 */
enum class ca_char_check_types {
    CA_ISALPHA,   /**< Check if the character is an alphabetic letter. */
    CA_ISDIGIT,   /**< Check if the character is a digit (0-9). */
    CA_ISSPACE,   /**< Check if the character is a whitespace. */
    CA_ISALNUM,   /**< Check if the character is alphanumeric (letter or digit). */
    CA_ISLOWER,   /**< Check if the character is a lowercase letter. */
    CA_ISUPPER,   /**< Check if the character is an uppercase letter. */
    CA_ISTITLE,   /**< Check if the character is a titlecase letter. */
    CA_ISNUMERIC, /**< Check if the character is numeric. */
    CA_ISDECIMAL, /**< Check if the character is a decimal digit. */
};

/**
 * @brief Template function to check if a character is alphabetic (A-Z, a-z).
 *
 * @param c The character (Unicode code point) to check.
 *
 * @return `true` if the character is alphabetic (i.e., within the ranges A-Z or a-z),
 *         `false` otherwise. The result depends on the encoding type.
 * @tparam encoding The character encoding type for proper classification.
 *
 * @note For unsupported encoding types, this function will return `false`.
 */
template <ca_encoding_t encoding>
inline bool
ca_isalpha(ca_char4_t c);

/**
 * @brief Template function to check if a character is a digit.
 *
 * @param c The character (Unicode code point) to check.
 *
 * @return `true` if the character is a digit (i.e., within the range 0-9),
 *         `false` otherwise. This function checks only for decimal digits.
 *         It does not account for other numeric characters like fractions or Roman numerals.
 * @tparam encoding The character encoding type for proper classification.
 *
 * @note For unsupported encoding types, this function will return `false`.
 */
template <ca_encoding_t encoding>
inline bool
ca_isdigit(ca_char4_t c);

/**
 * @brief Template function to check if a character is a whitespace character.
 *        Whitespace characters include spaces, tabs, newlines, and others.
 *
 * @param c The character (Unicode code point) to check.
 *
 * @return `true` if the character is a whitespace character (e.g., space, tab, newline),
 *         `false` otherwise. The result depends on the encoding type.
 * @tparam encoding The character encoding type for proper classification.
 *
 * @note For unsupported encoding types, this function will return `false`.
 */
template <ca_encoding_t encoding>
inline bool
ca_isspace(ca_char4_t c);

/**
 * @brief Template function to check if a character is alphanumeric (A-Z, a-z, 0-9).
 *
 * @param c The character (Unicode code point) to check.
 *
 * @return `true` if the character is alphanumeric (i.e., a letter or a digit),
 *         `false` otherwise. The result depends on the encoding type.
 * @tparam encoding The character encoding type for proper classification.
 *
 * @note For unsupported encoding types, this function will return `false`.
 */
template <ca_encoding_t encoding>
inline bool
ca_isalnum(ca_char4_t c);

/**
 * @brief Template function to check if a character is lowercase (a-z).
 *
 * @param c The character (Unicode code point) to check.
 *
 * @return `true` if the character is lowercase (i.e., within the range a-z),
 *         `false` otherwise. The result depends on the encoding type.
 * @tparam encoding The character encoding type for proper classification.
 *
 * @note For unsupported encoding types, this function will return `false`.
 */
template <ca_encoding_t encoding>
inline bool
ca_islower(ca_char4_t c);

/**
 * @brief Template function to check if a character is uppercase (A-Z).
 *
 * @param c The character (Unicode code point) to check.
 *
 * @return `true` if the character is uppercase (i.e., within the range A-Z),
 *         `false` otherwise. The result depends on the encoding type.
 * @tparam encoding The character encoding type for proper classification.
 *
 * @note For unsupported encoding types, this function will return `false`.
 */
template <ca_encoding_t encoding>
inline bool
ca_isupper(ca_char4_t c);

/**
 * @brief Template function to check if a character is in title case (uppercase first letter).
 *
 * @param c The character (Unicode code point) to check.
 *
 * @return `true` if the character is in title case (typically the first letter of a word, uppercase),
 *         `false` otherwise. The result depends on the encoding type.
 * @tparam encoding The character encoding type for proper classification.
 *
 * @note For unsupported encoding types, this function will return `false`.
 */
template <ca_encoding_t encoding>
inline bool
ca_istitle(ca_char4_t c);

/**
 * @brief Template function to check if a character is numeric.
 *        This includes digits (0-9), as well as other characters that are
 *        considered numeric in various scripts (e.g., Roman numerals, fractions, etc.).
 *
 * @param c The character (Unicode code point) to check.
 *
 * @return `true` if the character is numeric (i.e., a digit or other numeric characters such as fractions or Roman numerals),
 *         `false` otherwise. This function checks for all types of numeric characters, not just decimal digits.
 * @tparam encoding The character encoding type for proper classification.
 *
 * @note For unsupported encoding types, this function will return `false`.
 */
template <ca_encoding_t encoding>
inline bool
ca_isnumeric(ca_char4_t c);

/**
 * @brief Template function to check if a character is a decimal digit.
 *        A decimal digit is a digit character in the range 0-9.
 *
 * @param c The character (Unicode code point) to check.
 *
 * @return `true` if the character is a decimal digit (i.e., within the range 0-9),
 *         `false` otherwise. The result depends on the encoding type.
 * @tparam encoding The character encoding type for proper classification.
 *
 * @note For unsupported encoding types, this function will return `false`.
 */
template <ca_encoding_t encoding>
inline bool
ca_isdecimal(ca_char4_t c);

/**
 * @brief Template function to check a character based on the specified encoding and check type.
 *
 * This function checks a character (`c`) against the specified condition (`check_type`) for a given encoding (`encoding`).
 * The function is specialized for various character checks such as whether the character is alphanumeric,
 * alphabetic, digit, whitespace, and so on, depending on the `check_type` passed as a template parameter.
 *
 * @param c The character to check.
 *
 * @tparam encoding The character encoding type to use for the check (e.g., UTF-8, ASCII).
 * @tparam check_type The type of check to perform, such as:
 *        - `CA_ISALPHA`: Check if the character is alphabetic.
 *        - `CA_ISDIGIT`: Check if the character is a digit.
 *        - `CA_ISSPACE`: Check if the character is a whitespace.
 *        - `CA_ISALNUM`: Check if the character is alphanumeric.
 *        - `CA_ISLOWER`: Check if the character is a lowercase letter.
 *        - `CA_ISUPPER`: Check if the character is an uppercase letter.
 *        - `CA_ISTITLE`: Check if the character is a titlecase letter.
 *        - `CA_ISNUMERIC`: Check if the character is numeric.
 *        - `CA_ISDECIMAL`: Check if the character is a decimal digit.
 *
 * @return Returns `true` if the character satisfies the condition for the specified check type,
 *         otherwise returns `false`.
 *
 * @note This function uses static assertions to ensure only valid `check_type` values are used.
 */
template <ca_encoding_t encoding, ca_char_check_types check_type>
inline bool
ca_char_check(ca_char_t c);
}

#endif // CA_CHAR_H