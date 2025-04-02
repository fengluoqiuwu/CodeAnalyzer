// ================================
// CodeAnalyzer - source/c_src/common/public/ca_string/ca_buffer.h
//
// @file
// @brief Defines the `ca_buffer` template for handling character buffers with
//        various encodings (ASCII, UTF-8, UTF-32), providing utilities for
//        memory operations, buffer comparison, and character property checks.
// ================================

#ifndef CA_BUFFER_H
#define CA_BUFFER_H

#include "ca_char.h"
#include "ca_math.h"

namespace ca::ca_string {

/**
 * @enum ca_buffer_implemented_unary_functions
 * @brief Enumeration for implemented unary functions.
 */
enum class ca_buffer_implemented_unary_functions {
    ISALPHA,    ///< Checks if a character is alphabetic
    ISDECIMAL,  ///< Checks if a character is decimal
    ISDIGIT,    ///< Checks if a character is a digit
    ISSPACE,    ///< Checks if a character is whitespace
    ISALNUM,    ///< Checks if a character is alphanumeric
    ISLOWER,    ///< Checks if a character is lowercase
    ISUPPER,    ///< Checks if a character is uppercase
    ISTITLE,    ///< Checks if a string is titlecase
    ISNUMERIC,  ///< Checks if a character is numeric
    STR_LEN     ///< Returns the length of a string
};

/**
 * @struct ca_buffer
 * @brief A template struct representing a buffer for handling
 *        different character encodings.
 *
 * @tparam encoding The character encoding type to decode.
 */
template <ca_encoding_t encoding>
struct ca_buffer {
    ca_char_t *buf;         ///< Pointer to the start of the buffer.
    ca_char_t *after;       ///< Pointer to the end of the buffer.

    /**
     * @brief Default constructor initializing the buffer pointers to `nullptr`.
     */
    inline ca_buffer();

    /**
     * @brief Constructs a Buffer with a specified pointer and element size.
     *
     * This constructor initializes the buffer, setting the starting
     * pointer and calculating the endpoint based on the given
     * element size. (Without memory allocation)
     *
     * @param buf_ Pointer to the start of the buffer.
     * @param size Size in bytes of the buffer elements.
     */
    inline ca_buffer(ca_char_t *buf_, ca_size_t size);

    /**
     * @brief Increment the buffer pointer by a specified number of positions.
     *
     * @param n The number of positions to move.
     *
     * @return A reference to the updated Buffer object.
     */
    inline ca_buffer<encoding> &
    operator +=(ca_int64_t n);

    /**
     * @brief Decrement the buffer pointer by a specified number of positions.
     *
     * @param n The number of positions to move.
     *
     * @return A reference to the updated Buffer object.
     */
    inline ca_buffer<encoding> &
    operator -=(ca_int64_t n);

    /**
     * @brief Pre-increment the buffer pointer by one position.
     *
     * @return A reference to the updated Buffer object.
     */
    inline ca_buffer<encoding> &
    operator ++();

    /**
     * @brief Post-increment the buffer pointer by one position.
     *
     * @return A copy of the Buffer object before decrementing.
     */
    inline ca_buffer<encoding>
    operator ++(int);

    /**
     * @brief Pre-decrement the buffer pointer by one position.
     *
     * @return A reference to the updated Buffer object.
     */
    inline ca_buffer<encoding> &
    operator --();

    /**
     * @brief Post-decrement the buffer pointer by one position.
     *
     * @return A copy of the Buffer object before decrementing.
     */
    inline ca_buffer<encoding>
    operator --(int);

    /**
     * @brief Moves the buffer forward by a specified number of elements.
     *
     * @param n The number of elements to move forward.
     *
     * @return A new `Buffer<enc>` at the updated position.
     */
    inline ca_buffer<encoding>
    operator +(ca_int64_t n);

    /**
     * @brief Moves the buffer backward by a specified number of elements.
     *
     * @param n The number of elements to move backward.
     *
     * @return A new `Buffer<enc>` at the updated position.
     */
    inline ca_buffer<encoding>
    operator -(ca_int64_t n);

    /**
     * @brief Computes the difference between two buffers.
     *
     * This function returns the difference in bytes for UTF-8
     * buffers, and the difference in characters for ASCII and UTF-32
     * buffers.
     *
     * @param other The other buffer to compare.
     *
     * @return The difference between the two buffers in bytes for
     *         UTF-8 and in characters for ASCII and UTF-32.
     *
     * @note For UTF-8 strings, the result is only meaningful if
     *       both buffers originate from the same string.
     */
    inline ca_ssize_t
    operator -(ca_buffer<encoding> const &other) const;

    /**
     * @brief Checks if two buffers are equal.
     *
     * @param other The buffer to compare with.
     * @return True if the buffers are equal, false otherwise.
     */
    inline bool
    operator ==(ca_buffer<encoding> const &other) const;

    /**
     * @brief Checks if two buffers are not equal.
     *
     * @param other The buffer to compare with.
     * @return True if the buffers are not equal, false otherwise.
     */
    inline bool
    operator !=(ca_buffer<encoding> const &other) const;

    /**
     * @brief Checks if the current buffer is less than the other buffer.
     *
     * @param other The buffer to compare with.
     * @return True if the current buffer is less than the other, false otherwise.
     */
    inline bool
    operator <(ca_buffer<encoding> const &other) const;

    /**
     * @brief Checks if the current buffer is greater than the other buffer.
     *
     * @param other The buffer to compare with.
     * @return True if the current buffer is greater than the other, false otherwise.
     */
    inline bool
    operator >(ca_buffer<encoding> const &other) const;

    /**
     * @brief Checks if the current buffer is less than or equal to the other buffer.
     *
     * @param other The buffer to compare with.
     * @return True if the current buffer is less than or equal to the other, false otherwise.
     */
    inline bool
    operator <=(ca_buffer<encoding> const &other) const;

    /**
     * @brief Checks if the current buffer is greater than or equal to the other buffer.
     *
     * @param other The buffer to compare with.
     * @return True if the current buffer is greater than or equal to the other, false otherwise.
     */
    inline bool
    operator >=(ca_buffer<encoding> const &other) const;

    /**
     * @brief Dereference the buffer to access the current codepoint.
     *
     * @return The current codepoint.
     */
    inline ca_char4_t
    operator *() const;

    /**
     * Check if the buffer is empty.
     * @return True if the buffer is empty, false otherwise.
     */
    [[nodiscard]] inline bool
    empty() const;

    /**
     * @brief Counts the number of codepoints in the buffer based on the encoding type.
     *
     * This function analyzes the internal buffer and determines the number of Unicode
     * codepoints it contains. The count is dependent on the encoding type (ASCII, UTF-8,
     * UTF-32). The function assumes that the buffer is correctly encoded, and
     * it uses the specified encoding type to interpret the data.
     *
     * For example, in UTF-8 encoding, a codepoint can take between 1 and 4 bytes,
     * depending on the specific character, while in UTF-32 encoding, a codepoint is
     * typically represented by 4 bytes.
     *
     * @return The number of codepoints in the buffer. This value is computed based
     *         on the encoding and may vary depending on how the data is stored and interpreted.
     */
    [[nodiscard]] inline ca_size_t
    num_codepoints() const;

    /**
     * @brief Advance the buffer pointer by a specified number of characters or bytes.
     * @param len The number of characters to copy:
     *            - For ASCII and UTF8, this is the number of bytes.
     *            - For UTF32, this is the number of characters.
     */
    inline void
    advance_lens(ca_size_t len);

    /**
     * @brief Get the number of bytes for the next character in the buffer.
     *
     * This function determines the number of bytes required to represent
     * the next character based on the current encoding.
     *
     * @return The number of bytes for the next character.
     */
    [[nodiscard]] inline ca_size_t
    num_bytes_next_character() const;

    /**
     * @brief Apply a unary function to each codepoint in the buffer.
     *
     * This function iterates over each codepoint in the buffer and applies
     * the specified unary function. If the function fails for any codepoint,
     * the iteration stops and returns false.
     *
     * @tparam unary_type The unary function to be applied to each codepoint.
     *
     * @return true if the function succeeds for all codepoints, false otherwise.
     */
    template <ca_buffer_implemented_unary_functions unary_type>
    [[nodiscard]] inline bool
    unary_loop() const;

    /**
     * @brief Check if all characters in the buffer are alphabetic.
     *
     * @return true if all characters are alphabetic, false otherwise.
     */
    [[nodiscard]] inline bool
    is_alpha() const;

    /**
     * @brief Check if all characters in the buffer are decimal digits.
     *
     * @return true if all characters are decimal digits, false otherwise.
     */
    [[nodiscard]] inline bool
    is_decimal() const;

    /**
     * @brief Check if all characters in the buffer are digits.
     *
     * @return true if all characters are digits, false otherwise.
     */
    [[nodiscard]] inline bool
    is_digit() const;

    /**
     * @brief Check if the first character in the buffer is a space.
     *
     * @return true if the first character is space, false otherwise.
     */
    [[nodiscard]] inline bool
    first_character_is_space() const;

    /**
     * @brief Check if all characters in the buffer are space.
     *
     * @return true if all characters are space, false otherwise.
     */
    [[nodiscard]] inline bool
    is_space() const;

    /**
     * @brief Check if all characters in the buffer are alphanumeric.
     *
     * @return true if all characters are alphanumeric, false otherwise.
     */
    [[nodiscard]] inline bool
    is_alphanumeric() const;

    /**
     * @brief Check if the buffer is in lower case.
     *
     * This function iterates through the characters in the buffer
     * and determines whether they are all lowercase letters. It
     * returns true if at least one lowercase letter is present and
     * no uppercase or title case letters are found.
     *
     * @return True if the buffer is in lower case; otherwise, false.
     */
    [[nodiscard]] inline bool
    islower() const;

    /**
     * @brief Check if the buffer is in upper case.
     *
     * This function iterates through the characters in the buffer
     * and determines whether they are all uppercase letters. It
     * returns true if at least one uppercase letter is present and
     * no lowercase or title case letters are found.
     *
     * @return True if the buffer  is in upper case; otherwise, false.
     */
    [[nodiscard]] inline bool
    isupper() const;

    /**
     * @brief Check if the buffer follows title case conventions.
     *
     * This function checks if the characters in the buffer are
     * formatted as title case. A title case string has each word's
     * first character capitalized, while other characters in the
     * word are lowercase. This function returns true if the buffer
     * adheres to this rule, and false otherwise.
     *
     * @return True if the buffer is in title case; otherwise, false.
     */
    [[nodiscard]] inline bool
    istitle() const;

    /**
     * @brief Check if all characters in the buffer are numeric.
     *
     * @return True if all characters are numeric; otherwise, false.
     */
    [[nodiscard]] inline bool
    isnumeric() const;

    /**
     * @brief Compare memory of two buffers.
     *
     * @param other The buffer to compare with.
     * @param len The number of bytes to compare:
     *            - For ASCII and UTF8, this is the number of bytes.
     *            - For UTF32, this is the number of characters.
     *
     * @return A negative value if the first buffer is less than the second,
     *         zero if they are equal, or a positive value if the first
     *         buffer is greater.
     */
    inline int
    buffer_memcmp(ca_buffer<encoding> other, ca_size_t len) const;

    /**
     * @brief Copy memory from self to another buffer.
     *
     * @param other The destination buffer.
     * @param len The number of characters to copy:
     *            - For ASCII and UTF8, this is the number of bytes.
     *            - For UTF32, this is the number of characters.
     */
    inline void
    buffer_memcpy(ca_buffer<encoding> other, ca_size_t len) const;

    /**
     * @brief Set a range of memory in the buffer to a specific character.
     *
     * @param fill_char The character to fill the buffer with.
     * @param n_chars The number of characters to set.
     *
     * @return The len of characters set in the buffer.
     *
     * @note len:
     *       - For ASCII and UTF8, this is the number of bytes.
     *       - For UTF32, this is the number of characters.
     */
    inline ca_size_t
    buffer_memset(ca_char4_t fill_char, ca_size_t n_chars);

    /**
     * @brief Fill the buffer with zeros starting from a specified index.
     *
     * This method fills the buffer with zero bytes starting from the specified
     * index (`start_index`) to the end of the buffer.
     *
     * @param start_index The index from which to start filling with zeros.
     */
    inline void
    buffer_fill_with_zeros_after_index(size_t start_index);

    /**
     * @brief Remove trailing whitespace and null characters from the buffer.
     *
     * This function modifies the buffer by removing any trailing whitespace characters
     * (such as spaces, tabs, and newlines) as well as null characters ("\0") from the end
     * of the buffer. It works in-place and does not allocate additional memory.
     *
     * @return The modified buffer with trailing whitespace and null characters removed.
     */
    inline ca_buffer<encoding>
    rstrip() const;

    /**
     * @brief Compare two buffers for equality or ordering.
     *
     * This function compares the current buffer with another buffer (`other`),
     * optionally ignoring trailing whitespace from both buffers before comparison,
     * depending on the value of `ignore_trailing_whitespace`.
     *
     * @param other The buffer to compare against.
     * @param ignore_trailing_whitespace If true, trailing whitespace will be ignored
     *                                   in both buffers before comparison.
     *
     * @return
     *   - A negative integer if the current buffer is less than `other`.
     *   - A positive integer if the current buffer is greater than `other`.
     *   - Zero if both buffers are equal.
     */
    inline int
    strcmp(ca_buffer<encoding> other, bool ignore_trailing_whitespace = false) const;
};

}

#endif //CA_BUFFER_H
