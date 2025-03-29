// ================================
// CodeAnalyzer - source/c_src/common/public/ca_string/ca_utf8_utils.h
//
// @file
// @brief Utility functions for UTF-8 encoding and decoding.
//
// This header provides various utility functions for handling UTF-8 encoded 
// strings, including converting between UTF-8 and UCS-4, determining the number 
// of bytes for UTF-8 characters, locating previous characters, and calculating 
// buffer sizes.
// 
// References:
// - This file is based on and modified from UTF-8 utilities in NumPy: 
//  https://github.com/numpy/numpy/blob/main/numpy/_core/src/multiarray/stringdtype/utf8_utils.h
// ================================

#ifndef CA_UTF8_UTILS_H
#define CA_UTF8_UTILS_H

#include "ca_math.h"
#include "ca_char_types.h"

namespace ca::ca_string::utf8 {

// ----------------------------
// Encode & Decode functions
// ----------------------------

/**
 * @brief Converts a UTF-8 encoded character to a UCS-4 code point.
 *
 * This function converts a valid UTF-8 encoded character stored in `c` to
 * its corresponding UCS-4 code point.
 *
 * @param c [in] Pointer to the UTF-8 encoded character.
 *          Must not be `nullptr`. Assumed to be valid UTF-8.
 * @param code [out] Pointer to store the resulting UCS-4 code point.
 *             Must not be `nullptr`.
 * @return The size of the character in bytes (1, 2, 3, or 4).
 *
 * @note This function does not perform error checking and assumes valid UTF-8.
 */
int
utf8_char_to_ucs4_code_without_check(const ca_char_t *c, ca_char4_t *code);

/**
 * @brief Converts a UCS-4 code point to a UTF-8 character.
 *
 * @param code [in] A valid UCS-4 Unicode code point.
 * @param c [out] Pointer to a 4-byte buffer to store the UTF-8 character.
 *          Must not be `nullptr`.
 * @return Number of bytes written to `c` (1 to 4).
 */
int
ucs4_code_to_utf8_char_without_check(ca_char4_t code, ca_char_t *c);

// ----------------------------
// Bytes of character getting functions
// ----------------------------

/**
 * @brief Determines the number of bytes for a UTF-8 character.
 *
 * Inspects the first byte of a UTF-8 encoded character and returns its length.
 *
 * @param c [in] Pointer to the first byte of the UTF-8 character (must not be nullptr).
 * @return
 * - `1` for ASCII (1-byte).
 * - `2`, `3`, or `4` for multi-bytes UTF-8 characters.
 *
 * @note This function only checks the first byte and does not validate the entire UTF-8 sequence.
 */
int
num_utf8_bytes_for_utf8_character_without_check(const ca_char_t *c);

/**
 * @brief Returns the number of UTF-8 bytes needed for a UCS-4 code point.
 *
 * @param code A valid UCS-4 Unicode code point.
 * @return
 * - `1` to `4` for valid code points.
 * - `-1` if the code point is invalid (e.g., surrogate pairs or out of range).
 */
int
num_utf8_bytes_for_codepoint(ca_char4_t code);

// ----------------------------
// Buffer Number of codepoints calculation functions
// ----------------------------

/**
 * @brief Counts the number of codepoints without validating UTF-8 sequences.
 *
 * @param buf [in] Pointer to the UTF-8 encoded string.
 * @param max_bytes [in] The maximum number of bytes to process.
 * @param num_codepoints [out] Pointer to store the number of codepoints.
 *
 * @note
 * - This function assumes the input is a well-formed UTF-8 string.
 * - It does not perform any validation to check for invalid UTF-8 sequences.
 */
void
num_codepoints_for_utf8_bytes_without_check(
        const ca_char_t* buf, ca_size_t max_bytes,
        ca_size_t *num_codepoints);

/**
 * @brief Counts the number of codepoints in a UTF-8 encoded string.
 *
 * @param buf [in] Pointer to the UTF-8 encoded string.
 * @param max_bytes [in] Maximum number of bytes to process.
 * @param num_codepoints [out] Pointer to store the number of codepoints.
 * @return
 * - `0` if the string is valid UTF-8.
 * - `-1` if the string contains invalid UTF-8 sequences.
 */
int
num_codepoints_for_utf8_bytes(
        const ca_char_t *buf, ca_size_t max_bytes,
        ca_size_t *num_codepoints);

// ----------------------------
// Buffer Size calculation functions
// ----------------------------

/**
 * @brief Calculates the size in bytes of a UTF-8 encoded string.
 *
 * @param buf [in] Pointer to the UTF-8 encoded string.
 * @param max_bytes [in] Maximum number of bytes to process.
 * @param utf8_bytes [out] Pointer to store the UTF-8 bytes size.
 * @return
 * - `0` on success.
 * - `-1` if the string contains invalid UTF-8 sequences.
 */
int
utf8_buffer_size(
        const ca_char_t *buf, ca_size_t max_bytes,
        ca_size_t utf8_bytes);

/**
 * @brief Calculates the size in bytes needed to encode UCS-4 code points as UTF-8.
 *
 * @param buf_usc4 [in] Pointer to a buffer of UCS-4 code points.
 * @param buf_length [in] Length of buf_usc4.
 * @param num_codepoints [out] Pointer to store the number of buf_usc4 codepoints that aren't trailing nulls.
 * @param utf8_bytes [out] Pointer to store the required UTF-8 bytes size.
 * @return
 * - `0` on success.
 * - `-1` if an invalid code point is encountered.
*/
int
utf8_size_of_utf32_buffer_encode(
        const ca_char4_t *buf_usc4, ca_size_t buf_length,
        ca_size_t *num_codepoints, ca_size_t *utf8_bytes);

// ----------------------------
// Location finding functions
// ----------------------------

/**
 * @brief Finds the location of the previous UTF-8 character from the current position.
 *
 * @param current [in] Pointer to the current position in a UTF-8 encoded string.
 * @param nchar [in] The number of UTF-8 characters to move back.
 * @param previous_loc [out] Pointer to store the byte location of the character found.
 *
 * @note
 * - `current` must point to a valid UTF-8 character within
 *   a well-formed UTF-8 string.
 * - `previous_loc` will be set to `nullptr` if no valid character is found.
 * - The function assumes the input is a valid UTF-8 string
 *   and does not check for boundary conditions.
 * - If `nchar` exceeds the number of available characters,
 *   `previous_loc` may point to an invalid memory location.
 * - The function works by moving backwards and skipping
 *   UTF-8 continuation bytes (`10xxxxxx` format).
 */
void
find_previous_utf8_character(
        const ca_char_t *current, ca_size_t nchar,
        ca_char_t **previous_loc);

/**
 * @brief Finds the byte locations of the specified UTF-8 code point indices in a buffer.
 *
 * @param buf [in] Pointer to the UTF-8 encoded string buffer.
 * @param buffer_size [in] Size of the buffer in bytes.
 * @param start_index [in] The starting code point index (0-based) to locate.
 * @param end_index [in] The ending code point index (0-based) to locate.
 * @param start_loc [out] Pointer to store the byte location of the `start_index`.
 * @param end_loc [out] Pointer to store the byte location of the `end_index`.
 * @return
 * - `0` if both `start_index` and `end_index` are successfully located.
 * - `-1` if the specified indices are not found within the buffer.
 *
 * @note
 * - If `start_index == 0`, `*start_loc` is set to `buf`.
 * - If `end_index == 0`, `*end_loc` is set to `buf`.
 * - If `start_index` or `end_index` is not found, the corresponding
 *   pointer (`start_loc` or `end_loc`) is set to `nullptr`.
 * - `buf`, `start_loc` and `end_loc` must not be `nullptr`.
 */
int
find_start_end_locs(
        ca_char_t *buf, ca_size_t buffer_size,
        ca_size_t start_index, ca_size_t end_index,
        ca_char_t **start_loc, ca_char_t **end_loc);

}

#endif // CA_UTF8_UTILS_H
