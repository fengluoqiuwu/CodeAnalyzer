// ================================
// CodeAnalyzer - source/c_src/common/public/ca_string/ca_fastsearch.h
//
// @file
// @brief Defines functions and structures for fast pattern searching and
//        counting occurrences of a pattern within a string.
// ================================

#ifndef CA_FAST_SEARCH_H
#define CA_FAST_SEARCH_H

#include "ca_math.h"

namespace ca::ca_string {

namespace fastsearch {
/**
 * @brief Defines the bloom filter width for CA_BLOOM.
 *
 * This constant defines the width of the bloom filter as 64 bits, which is
 * suitable for use with a 64-bit mask variable (`ca_uint64_t`).
 */
constexpr ca_uint64_t BLOOM_WIDTH = 64;


/**
 * @brief Threshold for using memchr or wmemchr in character search.
 *
 * If the search length exceeds this value, memchr/wmemchr is used.
 */
constexpr ca_size_t MEMCHR_CUT_OFF = 31;

/**
 * @brief Adds a character to the bloom filter mask.
 *
 * This function sets the bit in the bloom filter `mask` corresponding to
 * the character `c`. It ensures that the character is mapped to a valid
 * bit position within the range of `BLOOM_WIDTH`.
 *
 * @param[in,out] mask The bloom filter mask (of type `ca_uint64_t`) where the
 *                     character will be added. This mask is modified in place.
 * @param[in] ch The character to be added to the bloom filter mask.
 * @tparam char_type The type of characters stored in the buffer.
 *
 * @note The bit position is calculated by taking the character's value and
 *       applying a bitwise AND operation with `(CA_BLOOM_WIDTH - 1)`.
 *
 * @warning Ensure that `mask` is properly initialized to `0` before adding
 *          characters to avoid unexpected results.
 *
 * @code
 * uint64_t mask = 0;
 * char ch = 'a';  // ASCII 97
 * ca_bloom_add(mask, ch);  // Adds 'a' to the bloom filter
 * @endcode
 */
template <typename char_type>
inline void
bloom_add(uint64_t& mask, char_type ch);

/**
 * @brief Checks if a character is present in the bloom filter mask.
 *
 * This function checks whether the bit corresponding to the character `c`
 * is set in the bloom filter `mask`.
 *
 * @param[in] mask The bloom filter mask (of type `ca_uint64_t`) to check.
 * @param[in] ch The character to check in the bloom filter mask.
 * @tparam char_type The type of characters stored in the buffer.
 * @return true if the character is present, false otherwise.
 *
 * @note The bit position is calculated by taking the character's value and
 *       applying a bitwise AND operation with `(CA_BLOOM_WIDTH - 1)`.
 *
 * @code
 * uint64_t mask = 0;
 * char ch = 'a';  // ASCII 97
 * ca_bloom_add(mask, ch);  // Add 'a' to the bloom filter
 * if (ca_bloom(mask, ch)) {
 *     std::cout << "'a' is present in the bloom filter." << std::endl;
 * } else {
 *     std::cout << "'a' is not present in the bloom filter." << std::endl;
 * }
 * @endcode
 */
template <typename char_type>
inline bool
bloom_find(uint64_t mask, char_type ch);

/**
 * @struct CheckedIndexer
 * @brief A checked indexer for buffers of a specified character type.
 *
 * This structure provides safe and efficient indexed access to a buffer,
 * supporting both forward and reverse iteration based on the `reverse` flag.
 *
 * @tparam char_type The type of characters stored in the buffer.
 * @tparam reverse Whether to access the buffer in reverse (default: false).
 */
template <typename char_type, bool reverse>
struct CheckedIndexer {
    static constexpr bool is_reverse = reverse; ///< Indicates if the indexer is in reverse mode.

private:
    static constexpr char_type zero = 0; ///< Default value for uninitialized characters.

    char_type *buffer; ///< Pointer to the current position in the buffer.
    ca_size_t length;  ///< Remaining number of accessible elements.

public:

    /**
     * @brief Default constructor that initializes the buffer to `nullptr` and length to 0.
     */
    CheckedIndexer();

    /**
     * @brief Constructor that initializes the indexer with a given buffer and length.
     *
     * In reverse mode, the internal pointer will point to the last element in the buffer.
     *
     * @param buf Pointer to the character buffer.
     * @param len Length of the buffer.
     */
    CheckedIndexer(char_type *buf, size_t len);

    /**
     * @brief Dereference operator to access the current character.
     *
     * @return The character at the current position.
     *
     * @note Asserts internally that buffer is not null and length is non-zero.
     */
    inline char_type&
    operator*();

    /**
     * @brief Dereference operator to access the current character. (const version)
     *
     * @return The character at the current position.
     *
     * @note Asserts internally that buffer is not null and length is non-zero.
     */
    inline const char_type&
    operator*() const;

    /**
     * @brief Safe indexed access to the buffer.
     *
     * In forward mode, accesses `buffer[index]`;
     * in reverse mode, accesses `buffer[-index]` (i.e., going backward).
     *
     * @param index The offset index from the current position.
     * @return The character at the offset or 0 if index is out of bounds.
     * @note Be careful when it returns 0(out of range sign), and do not change it value.
     */
    inline char_type&
    operator[](size_t index);

    /**
     * @brief Safe indexed access to the buffer. (const version)
     *
     * In forward mode, accesses `buffer[index]`;
     * in reverse mode, accesses `buffer[-index]` (i.e., going backward).
     *
     * @param index The offset index from the current position.
     * @return The character at the offset or 0 if index is out of bounds.
     */
    inline const char_type&
    operator[](size_t index) const;

    /**
     * @brief Move the indexer forward by a specified number of elements.
     *
     * In reverse mode, this moves backward in the buffer.
     *
     * @param rhs Number of elements to move.
     * @return A new CheckedIndexer at the new position with reduced length.
     */
    inline CheckedIndexer<char_type, reverse>
    operator+(size_t rhs) const;

    /**
     * @brief Move the indexer forward in-place.
     *
     * In reverse mode, this moves backward.
     *
     * @param rhs Number of elements to move.
     * @return Reference to the modified indexer.
     */
    inline CheckedIndexer<char_type, reverse> &
    operator+=(size_t rhs);

    /**
     * @brief Prefix increment: move by 1 element.
     *
     * In reverse mode, moves backward.
     *
     * @return Reference to the updated indexer.
     */
    inline CheckedIndexer<char_type, reverse> &
    operator++();

    /**
     * @brief Postfix increment: move by 1 element, return previous state.
     *
     * In reverse mode, moves backward.
     *
     * @return Copy of the indexer before incrementing.
     */
    inline CheckedIndexer<char_type, reverse>
    operator++(int);

    /**
     * @brief Move the indexer backward in-place.
     *
     * In reverse mode, this moves forward in the buffer.
     *
     * @param rhs Number of elements to move.
     * @return Reference to the updated indexer.
     */
    inline CheckedIndexer<char_type, reverse> &
    operator-=(size_t rhs);

    /**
     * @brief Prefix decrement: move backward by 1 element.
     *
     * In reverse mode, moves forward.
     *
     * @return Reference to the updated indexer.
     */
    inline CheckedIndexer<char_type, reverse> &
    operator--();

    /**
     * @brief Postfix decrement: move backward by 1 element, return previous state.
     *
     * In reverse mode, moves forward.
     *
     * @return Copy of the indexer before decrementing.
     */
    inline CheckedIndexer<char_type, reverse>
    operator--(int);

    /**
     * @brief Compute the distance between two indexers.
     *
     * In forward mode: `this - rhs` returns `this->buffer - rhs.buffer`.
     * In reverse mode: `this - rhs` returns `rhs.buffer - this->buffer`.
     *
     * @param rhs Another CheckedIndexer to subtract.
     * @return Signed offset between the positions.
     */
    inline ca_ssize_t
    operator-(CheckedIndexer<char_type, reverse> rhs) const;

    /**
     * @brief Move the indexer backward by a specified number of elements.
     *
     * In reverse mode, this moves forward.
     *
     * @param rhs Number of elements to move.
     * @return A new indexer at the updated position.
     */
    inline CheckedIndexer<char_type, reverse>
    operator-(size_t rhs) const;

    /**
     * @brief Greater-than comparison based on pointer position.
     *
     * In reverse mode, the direction is reversed.
     *
     * @param rhs Another CheckedIndexer.
     * @return True if this is greater than rhs in logical order.
     */
    inline bool
    operator>(const CheckedIndexer<char_type, reverse>& rhs) const;

    /**
     * @brief Greater-than-or-equal comparison.
     *
     * Takes reverse mode into account.
     */
    inline bool
    operator>=(const CheckedIndexer<char_type, reverse>& rhs) const;

    /**
     * @brief Less-than comparison based on pointer position.
     *
     * Takes reverse mode into account.
     */
    inline bool
    operator<(const CheckedIndexer<char_type, reverse>& rhs) const;

    /**
     * @brief Less-than-or-equal comparison.
     *
     * Takes reverse mode into account.
     */
    inline bool
    operator<=(const CheckedIndexer<char_type, reverse>& rhs) const;

    /**
     * @brief Equality comparison operator.
     *
     * Checks if both indexers point to the same position.
     */
    inline bool
    operator==(const CheckedIndexer<char_type, reverse>& rhs) const;

    /**
     * @brief Inequality comparison operator.
     *
     * Checks if both indexers point to the different position.
     */
    inline bool
    operator!=(const CheckedIndexer<char_type, reverse>& rhs) const;

    /**
     * @brief Compares two indexers over a specified number of elements.
     *
     * Performs lexicographical comparison between this indexer and another
     * indexer over the first `len` elements.
     *
     * @param rhs The other CheckedIndexer to compare with.
     * @param len The number of elements to compare.
     * @return 0 if equal, -1 if this < rhs, 1 if this > rhs.
     *
     * @note Both indexers must have at least `len` elements.
     */
    inline int
    cmp(const CheckedIndexer<char_type, reverse>& rhs, ca_size_t len) const;

    /**
     * @brief Returns the internal raw buffer pointer.
     *
     * @return Pointer to the current buffer.
     * @note The buffer is kept private to prevent unintended direct access or modification.
     */
    inline char_type *
    get_buffer() const;

    /**
     * @brief Returns the length of the buffer.
     *
     * @return Length of the buffer.
     */
    [[nodiscard]] inline ca_size_t
    get_length() const;
};

/**
 * @brief Finds the first occurrence of a character in a string.
 *
 * @tparam char_type Type of the character (e.g., char or wchar_t).
 * @param str [in] Input string wrapped in CheckedIndexer. (Must not be reverse!)
 * @param n [in] Maximum number of characters to search. (From left)
 * @param ch [in] Character to locate.
 * @param index [out] The pointer to store the position
 *              of the found character.
 * @return True if the character is found, otherwise false.
 */
template <typename char_type>
inline bool
find_char(CheckedIndexer<char_type, false> str, ca_size_t n, char_type ch,
          ca_size_t *index);

/**
 * @brief Finds the last occurrence of a character in a string.
 *
 * @tparam char_type Type of the character (e.g., char or wchar_t).
 * @param str [in] Input string wrapped in CheckedIndexer. (Must not be reverse!)
 * @param n [in] Maximum number of characters to search. (From left)
 * @param ch [in] Character to locate.
 * @param index [out] The pointer to store the position
 *              of the found character.
 * @return True if the character is found, otherwise false.
 */
template <typename char_type>
inline bool
rfind_char(CheckedIndexer<char_type, false> str, ca_size_t n, char_type ch,
          ca_size_t *index);

/**
 * @brief Counts occurrences of a specified character in a given string.
 *
 * This function iterates through the string `str` and counts how many times
 * the character `ch` appears, stopping when the count reaches `max_count`.
 *
 * @tparam char_type The type of characters in the string.
 * @tparam from_right If true, the counting starts from the right side of the string;
 *                    otherwise, it starts from the left side.
 * @param str [in] The string in which to count occurrences of the character.
 * @param n [in] The length of the string `s`.
 * @param ch [in] The character to count in the string.
 * @param max_count [in] The maximum number of occurrences to count before stopping.
 * @return The total count of occurrences of `ch` in `str`, or `max_count`
 *         if that many occurrences were found.
 */
template <typename char_type, bool from_right>
static inline ca_size_t
count_char(CheckedIndexer<char_type, from_right> str, ca_size_t n,
           char_type ch, ca_size_t max_count);

/**
 * @brief Perform a lexicographic search for the maximal (minimal) suffix in
 *        a given string.
 *
 * This function searches through the `pattern` to find the maximal suffix,
 * which is essentially the largest lexicographic suffix.
 * Essentially this:
 * - maximal-suffix = max(pattern[i:] for i in range(len(pattern)+1))
 *
 * Additionally, it computes the period of the right half of the string.
 *
 * @tparam char_type Type of the character (e.g., char or wchar_t).
 * @tparam from_right If true, the search starts from the right side of the string;
 *                    otherwise, it starts from the left side.
 * @param pattern [in] The string to search in.
 * @param pattern_len [in] The length of the input pattern.
 * @param return_period [out] Pointer to store the period of the found suffix.
 * @param invert_alphabet [in] Flag to invert the comparison logic.
 * @return The index of the maximal (minimal) suffix found in the input pattern.
 *
 * @note
 *
 * - If `invert_alphabet` is true, character comparisons are reversed,
 *   treating smaller characters as larger.
 * - In both cases, if one suffix A is longer than suffix B and A starts with B,
 *   A is considered larger.
 *
 * Example:
 *
 * - [Input-1] pattern = "abcdabcabc"; pattern_len = 10;
 * - [Output-1] index = 3; period = 7;
 *
 * - [Input-2] pattern = "abcdabcabc"; pattern_len = 10; invert_alphabet = true;
 * - [Output-2] index = 4; period = 3;
 */
template <typename char_type, bool from_right>
inline ca_size_t
lex_search(CheckedIndexer<char_type, from_right> pattern, ca_size_t pattern_len,
           ca_size_t *return_period, bool invert_alphabet = false);

/**
 * @brief Perform a critical factorization on a string.
 *
 * This function splits the input string into two parts where the local
 * period is maximal.
 *
 * The function divides the input string as follows:
 * - str = (left := pattern[:cut]) + (right := pattern[cut:])
 *
 * The local period is the minimal length of a string `w` such that:
 * - left ends with `w` or `w` ends with left.
 * - right starts with `w` or `w` starts with right.
 *
 * According to the Critical Factorization Theorem, this maximal local
 * period is the global period of the string. The algorithm finds the
 * cut using lexicographical order and its reverse to compute the maximal
 * period, as shown by Crochemore and Perrin (1991).
 *
 * Example:
 * For the string "GCAGAGAG", the split position (cut) is at 2, resulting in:
 * - left = "GC"
 * - right = "AGAGAG"
 * The period of the right half is 2,  and the repeated substring
 * pattern "AG" verifies that this is the correct factorization.
 *
 * @tparam char_type Type of the character (e.g., char or wchar_t).
 * @tparam from_right If true, the search starts from the right side of the string;
 *                    otherwise, it starts from the left side.
 * @param pattern [in] The input string as a CheckedIndexer<char_type>.
 * @param pattern_len [in] Length of the input string.
 * @param return_period [out] Pointer to store the computed period of the right half.
 * @return The cut position where the string is factorized.
 */
template <typename char_type, bool from_right>
inline ca_size_t
factorize(CheckedIndexer<char_type, from_right> pattern, ca_size_t pattern_len,
          ca_size_t *return_period);

namespace internal {

/**
 * @brief Define the shift type used in the table.
 */
using SHIFT_TYPE = ca_uint8_t;

/**
 * @file_internal
 * @brief Internal macro to define the maximum shift value.
 */
constexpr ca_size_t MAX_SHIFT = CA_UINT8_MAX;

/**
 * @brief Define the number of bits for the table size.
 */
constexpr ca_size_t TABLE_SIZE_BITS = 6u;

/**
 * @brief Define the table size based on TABLE_SIZE_BITS.
 */
constexpr ca_size_t TABLE_SIZE = (1U << TABLE_SIZE_BITS);

/**
 * @brief Define the table mask used for bitwise operations.
 */
constexpr ca_size_t TABLE_MASK = (TABLE_SIZE - 1U);

/**
 * @brief Struct to store precomputed data for string search algorithms.
 *
 * This structure holds all the necessary precomputed values needed
 * to perform efficient string search operations on the given string.
 *
 * @tparam char_type Type of the characters in the string.
 * @tparam from_right If true, the search starts from the right side of the string;
 *                    otherwise, it starts from the left side.
 */
template <typename char_type, bool from_right>
struct prework {
    CheckedIndexer<char_type, from_right> str; ///< Indexer for the str (substring).
    ca_size_t len;                 ///< Length of the str.
    ca_size_t cut;                 ///< Critical factorization cut point.
    bool is_periodic;              ///< If the str is periodic or not.
    ca_size_t period;              ///< Period of the right half of the str.
    ca_size_t gap;                 ///< Gap value for skipping during search.
    SHIFT_TYPE table[TABLE_SIZE];  ///< Shift table for optimizing search.
};

/**
 * @brief Preprocesses the `pattern` (substring) for optimized string search.
 *
 * This function performs preprocessing on the given `pattern` (substring)
 * to prepare auxiliary data that will be used to optimize the string
 * search algorithm. The preprocessing involves factorization of the
 * substring, periodicity detection, gap computation, and the generation
 * of a Boyer-Moore "Bad Character" shift table.
 *
 * @tparam char_type The character type of the string.
 * @tparam from_right If true, the search starts from the right side of the string;
 *                    otherwise, it starts from the left side.
 * @param pattern The substring to be searched.
 * @param pattern_len The length of the substring.
 * @param work A pointer to the search_prep_data structure where
 *             the preprocessing results will be stored.
 *              - `str`: The pattern to be searched.
 *              - `len`: The length of the pattern.
 *              - `cut`: The cut position for factorization.
 *              - `is_periodic`: Indicates if the pattern is periodic.
 *              - `period`: The period of the pattern if is periodic else
 *                          `1 + max(cut, len - cut)`.
 *              - `gap`: The gap value for skipping during search, which is
 *                       the distance between the last character and the
 *                       previous character with the same masked value as
 *                       the last character.
 *              - `table`: The shift table for optimizing search.
 */
template <typename char_type, bool from_right>
void
preprocess(CheckedIndexer<char_type, from_right> pattern, ca_size_t pattern_len,
           prework<char_type, from_right> *work);

/**
 * @brief Performs string search using the Two-Way algorithm optimized
 *        for periodic patterns.
 *
 * This function is a specialized implementation of the Two-Way string
 * matching algorithm used when the pattern has periodic structure
 * (i.e., `work->is_periodic == true`). It leverages pattern periodicity
 * to reduce redundant comparisons and improve performance by using memory
 * and period-based shifting strategies.
 *
 * @tparam char_type The type of the characters in `str` and `pattern`
 * @tparam from_right If true, the search starts from the right side of the string;
 *                    otherwise, it starts from the left side.
 * @param str [in] The string to search within, wrapped in CheckedIndexer.
 * @param str_len [in] The length of `str`.
 * @param work [in] A pointer to the preprocessed pattern data.
 * @param index [out] Pointer to store the starting index of a match.
 * @return True if a match is found, false otherwise.
 */
template <typename char_type, bool from_right>
inline bool
two_way_periodic(CheckedIndexer<char_type, from_right> str, ca_size_t str_len,
                 prework<char_type, from_right> *work, ca_size_t *index);

/**
 * @brief Performs string search using the Two-Way algorithm optimized
 *        for non-periodic patterns.
 *
 * This function handles the case when the pattern has no periodic
 * structure (i.e., `work->is_periodic == false`). It follows the standard
 * Two-Way algorithm logic, using a cut point to split the pattern and
 * a bad character shift table to skip non-matching windows efficiently.
 *
 * @tparam char_type The type of the characters in `str` and `pattern`
 * @tparam from_right If true, the search starts from the right side of the string;
 *                    otherwise, it starts from the left side.
 * @param str [in] The string to search within, wrapped in CheckedIndexer.
 * @param str_len [in] The length of `str`.
 * @param work [in] A pointer to the preprocessed pattern data.
 * @param index [out] Pointer to store the starting index of a match.
 * @return True if a match is found, false otherwise.
 */
template <typename char_type, bool from_right>
inline bool
two_way_not_periodic(CheckedIndexer<char_type, from_right> str, ca_size_t str_len,
prework<char_type, from_right> *work, ca_size_t *index);

/**
 * @brief Searches for a pattern (substring) within `str` (string)
 *        using the Two-Way string matching algorithm.
 *
 * This function efficiently searches for a pattern within `str` using
 * preprocessed data. It handles both periodic and non-periodic patterns
 * and optimizes the search process with a bad character shift table. The
 * function iterates through `str` in windows, skipping over sections
 * that do not match, improving performance and reducing comparisons.
 *
 * For more details, refer to the following resources:
 * - Crochemore and Perrin's (1991) Two-Way algorithm:
 *   [Two-Way Algorithm](http://www-igm.univ-mlv.fr/~lecroq/string/node26.html#SECTION00260).
 *
 * @tparam char_type The type of the characters in `str` and `pattern`
 * @tparam from_right If true, the search starts from the right side of the string;
 *                    otherwise, it starts from the left side.
 * @param str [in] The string to search within, wrapped in CheckedIndexer.
 * @param str_len [in] The length of `str`.
 * @param work [in] A pointer to the search_prep_data structure containing
 *             preprocessed data for `pattern`.
 * @param index [out] Pointer to store the starting index of the first
 *              occurrence of `pattern` within `str`.
 * @return If the pattern is found.
 */
template <typename char_type, bool from_right>
bool
two_way(CheckedIndexer<char_type, from_right> str, ca_size_t str_len,
        prework<char_type, from_right> *work, ca_size_t *index);

}

/**
 * @brief Finds the first occurrence of a pattern (substring) within a str (string).
 *
 * This function applies the two-way string matching algorithm to efficiently
 * search for a pattern (substring) within a str (main string).
 *
 * @tparam char_type The character type of the strings.
 * @tparam from_right If true, the search starts from the right side of the string;
 *                    otherwise, it starts from the left side.
 * @param str [in] The string in which to search for the pattern.
 * @param str_len [in] The length of the str string.
 * @param pattern [in] The substring to search for in the str.
 * @param pattern_len [in] The length of the pattern substring.
 * @param index [out] Pointer to store the starting index of the first
 *              occurrence of `pattern` within `str`.
 * @return If the pattern is found.
 */
template <typename char_type, bool from_right>
inline bool
two_way_find(CheckedIndexer<char_type, from_right> str, ca_size_t str_len,
             CheckedIndexer<char_type, from_right> pattern, ca_size_t pattern_len,
             ca_size_t *index);

/**
 * @brief Counts the occurrences of a pattern (substring) within a str (string).
 *
 * This function applies the two-way string matching algorithm to count how many
 * times a pattern (substring) appears within a str (main string). It stops
 * counting when the maximum number of occurrences (`max_count`) is reached.
 *
 * @tparam char_type The character type of the strings.
 * @tparam from_right If true, the counting starts from the right side of the string;
 *                    otherwise, it starts from the left side.
 * @param str [in] The string in which to search for occurrences of the pattern.
 * @param str_len [in] The length of the str string.
 * @param pattern [in] The substring to search for in the str.
 * @param pattern_len [in] The length of the pattern substring.
 * @param max_count [in] The maximum number of occurrences to count before returning.
 * @return The number of occurrences of the pattern in the str.
 *         If the maximum count is reached, it returns `max_count`.
 */
template <typename char_type, bool from_right>
inline ca_size_t
two_way_count(CheckedIndexer<char_type, from_right> str, ca_size_t str_len,
              CheckedIndexer<char_type, from_right> pattern, ca_size_t pattern_len,
              ca_size_t max_count);

/**
 * @brief A function that counts the occurrences of a substring `pattern`
 * in the string `str` using a bloom filter to optimize character matching.
 *
 * This function  counts the occurrences of a pattern `pattern` in
 * the given string `str`. It uses a bloom filter for fast rejection
 * of non-matching characters and performs character-by-character
 * comparison for potential matches. The algorithm is based on the
 * Boyer-Moore string search technique.
 *
 * @tparam char_type The character type of the strings.
 * @tparam from_right If true, the search starts from the right side of the string;
 *                    otherwise, it starts from the left side.
 * @param str [in] The string in which to search for the pattern.
 * @param str_len [in] The length of the str string.
 * @param pattern [in] The substring to search for in the str.
 * @param pattern_len [in] The length of the pattern substring.
 * @param index [out] Pointer to store the starting index of the first
 *              occurrence of `pattern` within `str`.
 * @return If the pattern is found.
 */
template <typename char_type, bool from_right>
inline bool
default_find(CheckedIndexer<char_type, from_right> str, ca_size_t str_len,
             CheckedIndexer<char_type, from_right> pattern, ca_size_t pattern_len,
             ca_size_t *index);

/**
 * @brief A function that searches for a substring `pattern` in the
 * string `str` using a bloom filter to optimize character matching.
 *
 * This function searches for occurrences of a pattern `pattern` in
 * the given string `str`. It uses a bloom filter for fast rejection
 * of non-matching characters and performs character-by-character
 * comparison for potential matches. The algorithm is based on the
 * Boyer-Moore string search technique.
 *
 * @tparam char_type The character type of the strings.
 * @tparam from_right If true, the counting starts from the right side of the string;
 *                    otherwise, it starts from the left side.
 * @param str [in] The string in which to search for occurrences of the pattern.
 * @param str_len [in] The length of the str string.
 * @param pattern [in] The substring to search for in the str.
 * @param pattern_len [in] The length of the pattern substring.
 * @param max_count [in] The maximum number of occurrences to count before returning.
 * @return The number of occurrences of the pattern in the str.
 *         If the maximum count is reached, it returns `max_count`.
 */
template <typename char_type, bool from_right>
inline ca_size_t
default_count(CheckedIndexer<char_type, from_right> str, ca_size_t str_len,
              CheckedIndexer<char_type, from_right> pattern, ca_size_t pattern_len,
              ca_size_t max_count);

/**
 * @brief Performs an adaptive string search using a bloom filter and fallback
 *        to two-way search for large data.
 *
 * @tparam char_type The character type of the strings.
 * @tparam from_right If true, the search starts from the right side of the string;
 *                    otherwise, it starts from the left side.
 * @param str [in] The string in which to search for the pattern.
 * @param str_len [in] The length of the str string.
 * @param pattern [in] The substring to search for in the str.
 * @param pattern_len [in] The length of the pattern substring.
 * @param index [out] Pointer to store the starting index of the first
 *              occurrence of `pattern` within `str`.
 * @return If the pattern is found.
 */
template <typename char_type, bool from_right>
inline bool
adaptive_find(CheckedIndexer<char_type, from_right> str, ca_size_t str_len,
              CheckedIndexer<char_type, from_right> pattern, ca_size_t pattern_len,
              ca_size_t *index);

/**
 * @brief Performs an adaptive string count using a bloom filter and fallback
 *        to two-way search for large data.
 *
 * @tparam char_type The character type of the strings.
 * @tparam from_right If true, the counting starts from the right side of the string;
 *                    otherwise, it starts from the left side.
 * @param str [in] The string in which to search for occurrences of the pattern.
 * @param str_len [in] The length of the str string.
 * @param pattern [in] The substring to search for in the str.
 * @param pattern_len [in] The length of the pattern substring.
 * @param max_count [in] The maximum number of occurrences to count before returning.
 * @return The number of occurrences of the pattern in the str.
 *         If the maximum count is reached, it returns `max_count`.
 */
template <typename char_type, bool from_right>
inline ca_size_t
adaptive_count(CheckedIndexer<char_type, from_right> str, ca_size_t str_len,
               CheckedIndexer<char_type, from_right> pattern, ca_size_t pattern_len,
               ca_size_t max_count);

}

/**
 * @brief Performs a fast search for a pattern within a string.
 *
 * This function searches for the occurrence of a given pattern within a string and returns
 * whether the pattern was found. If the pattern is found, the index of its occurrence is
 * stored in the variable pointed to by `index`. The search can be performed from the left
 * or from the right depending on the `from_right` flag.
 *
 * @tparam char_type The type of characters used in the string and the pattern.
 * @tparam from_right If true, the search starts from the right side of the string;
 *                    otherwise, it starts from the left side.
 * @param[in] str Pointer to the string where the search is performed.
 * @param[in] str_len The length of the string `str`.
 * @param[in] pattern Pointer to the pattern being searched.
 * @param[in] pattern_len The length of the pattern `pattern`.
 * @param[out] index Pointer to the variable where the index of the found pattern is stored.
 *                   If the pattern is not found, the value of `index` is undefined.
 * @return True if the pattern is found in the string, otherwise false.
 *
 * @note If the pattern length `m` is zero or greater than the string length `n`,
 *       the function will return false.
 *
 * @code
 * char str[] = "hello world";
 * char pattern[] = "world";
 * ca_size_t index;
 * bool found = ca_fastsearch(str, 11, pattern, 5, &index);
 * if (found) {
 *     printf("Pattern found at index %zu\n", index);
 * } else {
 *     printf("Pattern not found\n");
 * }
 * @endcode
 */
template <typename char_type, bool from_right>
inline bool
ca_fastsearch(char_type* str, ca_size_t str_len,
              char_type* pattern, ca_size_t pattern_len,
              ca_size_t *index);

/**
 * @brief Counts the occurrences of a pattern in a string.
 *
 * This function counts the number of occurrences of a specified pattern within a string.
 * It stops counting once the number of occurrences reaches `max_count`. The counting can
 * be done from left to right or from right to left based on the `from_right` flag.
 *
 * @tparam char_type The type of characters used in the string and the pattern.
 * @tparam from_right If true, the counting starts from the right side of the string;
 *                    otherwise, it starts from the left side.
 * @param[in] str Pointer to the string where the counting is performed.
 * @param[in] str_len The length of the string `str`.
 * @param[in] pattern Pointer to the pattern whose occurrences are to be counted.
 * @param[in] pattern_len The length of the pattern `pattern`.
 * @param[in] max_count The maximum number of occurrences to be counted.
 * @param[out] count Pointer to the variable where the number of occurrences is stored.
 *                   If no occurrence is found, `count` is set to 0.
 *
 * @note If the pattern length `m` is zero or greater than the string length `n`,
 *       the function will set `count` to 0.
 *
 * @code
 * char str[] = "ababababa";
 * char pattern[] = "aba";
 * ca_size_t count;
 * ca_fastcount(str, 9, pattern, 3, 5, &count);
 * printf("Pattern found %zu times\n", count);
 * @endcode
 */
template<typename char_type, bool from_right>
inline void
ca_fastcount(char_type* str, ca_size_t str_len,
             char_type* pattern, ca_size_t pattern_len,
             ca_size_t max_count, ca_size_t *count);

}

#include "../../private/ca_string/ca_fastsearch.tpp"

#endif //CA_FAST_SEARCH_H
