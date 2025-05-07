// ================================
// CodeAnalyzer - source/c_src/common/public/ca_char/ca_char_errors.h
//
// @file
// @brief Defines errors in ca_char library.
// ================================

#ifndef CA_CHAR_ERRORS_H
#define CA_CHAR_ERRORS_H

namespace ca::ca_char {

/**
 * @enum ca_char_status
 * @brief Enumeration for character processing status codes.
 */
enum class ca_char_status {
    SUCCESS = 0,            /**< Operation was successful. */
    INVALID_INPUT = 1,      /**< Invalid input provided. */
    INVALID_ENCODING = 2,   /**< Unsupported encoding type. */
    INVALID_CODEPOINT = 4,  /**< Invalid codepoint encountered. */
    BUFFER_OVERFLOW = 8,    /**< Buffer overflow occurred. */
    MEMORY_ALLOCATION_FAILURE = 16, /**< Memory allocation failed. */
    UNKNOWN_ERROR = 32,     /**< An unknown error occurred. */
};

/**
* @brief Retrieves a string representation of a character processing status code.
*
* @param status The `ca_char_status` enumeration value representing the status code.
* @return A constant string describing the status.
*/
constexpr const char *
get_status_string(ca_char_status status);

}

#endif //CA_CHAR_ERRORS_H
