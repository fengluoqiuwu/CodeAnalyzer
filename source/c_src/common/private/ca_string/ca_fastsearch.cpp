// ================================
// CodeAnalyzer - source/c_src/common/private/ca_string/ca_fastsearch.cpp
//
// @file
// @brief
// ================================

#include "ca_fastsearch.h"
#include "ca_math.h"

#include <cassert>
#include <cstring>
#include <cwchar>

namespace ca::ca_string {

namespace fastsearch {
template <typename char_type>
inline void
bloom_add(uint64_t& mask, const char_type ch) {
    mask |= (1ULL << (static_cast<uint64_t>(ch) & (BLOOM_WIDTH - 1)));
}

template <typename char_type>
inline bool
bloom_find(const uint64_t mask, const char_type ch) {
    return (mask & (1ULL << (static_cast<uint64_t>(ch) & (BLOOM_WIDTH - 1)))) != 0;
}

template <typename char_type, bool reverse>
CheckedIndexer<char_type, reverse>::CheckedIndexer()
{
    buffer = nullptr;
    length = 0;
}

template <typename char_type, bool reverse>
CheckedIndexer<char_type, reverse>::CheckedIndexer(char_type *buf, const size_t len)
{
    if constexpr (!reverse) {
        buffer = buf;
    }
    else {
        buffer = buf + (len - 1);
    }
    length = len;
}

template <typename char_type, bool reverse>
inline char_type
CheckedIndexer<char_type, reverse>::operator*()
{
    assert(buffer != nullptr);
    assert(length != 0);
    return *(this->buffer);
}

template <typename char_type, bool reverse>
inline char_type
CheckedIndexer<char_type, reverse>::operator[](const size_t index)
{
    if (index >= this->length) {
        return static_cast<char_type>(0);
    }

    if constexpr(!reverse) {
        return this->buffer[index];
    }
    else {
        return this->buffer[-index];
    }
}

template <typename char_type, bool reverse>
inline CheckedIndexer<char_type, reverse>
CheckedIndexer<char_type, reverse>::operator+(const size_t rhs)
{
    if (rhs > this->length) {
        rhs = this->length;
    }

    if constexpr (!reverse) {
        return CheckedIndexer(this->buffer + rhs, this->length - rhs);
    }
    else {
        return CheckedIndexer(this->buffer - rhs, this->length - rhs);
    }
}

template <typename char_type, bool reverse>
inline CheckedIndexer<char_type, reverse>&
CheckedIndexer<char_type, reverse>::operator+=(const size_t rhs)
{
    if (rhs > this->length) {
        rhs = this->length;
    }

    if constexpr (!reverse) {
        this->buffer += rhs;
    }
    else {
        this->buffer -= rhs;
    }
    this->length -= rhs;

    return *this;
}

template <typename char_type, bool reverse>
inline CheckedIndexer<char_type, reverse>&
CheckedIndexer<char_type, reverse>::operator++()
{
    *this += 1;
    return *this;
}

template <typename char_type, bool reverse>
inline CheckedIndexer<char_type, reverse>
CheckedIndexer<char_type, reverse>::operator++(int)
{
    CheckedIndexer temp = *this;
    *this += 1;
    return temp;
}

template <typename char_type, bool reverse>
inline CheckedIndexer<char_type, reverse>&
CheckedIndexer<char_type, reverse>::operator-=(const size_t rhs)
{
    if constexpr (!reverse) {
        this->buffer -= rhs;
    }
    else {
        this->buffer += rhs;
    }
    this->length += rhs;
    return *this;
}

template <typename char_type, bool reverse>
inline CheckedIndexer<char_type, reverse>&
CheckedIndexer<char_type, reverse>::operator--()
{
    *this -= 1;
    return *this;
}

template <typename char_type, bool reverse>
inline CheckedIndexer<char_type, reverse>
CheckedIndexer<char_type, reverse>::operator--(int)
{
    CheckedIndexer temp = *this;
    *this -= 1;
    return temp;
}

template <typename char_type, bool reverse>
inline ca_ssize_t
CheckedIndexer<char_type, reverse>::operator-(const CheckedIndexer<char_type, reverse> rhs) const
{
    if constexpr (!reverse) {
        return this->buffer - rhs.buffer;
    }
    else {
        return rhs.buffer - this->buffer;
    }
}

template <typename char_type, bool reverse>
inline CheckedIndexer<char_type, reverse>
CheckedIndexer<char_type, reverse>::operator-(const size_t rhs) const
{
    if constexpr (!reverse) {
        return CheckedIndexer(this->buffer - rhs, this->length + rhs);
    }
    else {
        return CheckedIndexer(this->buffer + rhs, this->length + rhs);
    }
}

template <typename char_type, bool reverse>
inline bool
CheckedIndexer<char_type, reverse>::operator>(const CheckedIndexer<char_type, reverse>& rhs) const
{
    if constexpr (!reverse) {
        return this->buffer > rhs.buffer;
    }
    else {
        return this->buffer < rhs.buffer;
    }
}

template <typename char_type, bool reverse>
inline bool
CheckedIndexer<char_type, reverse>::operator>=(const CheckedIndexer<char_type, reverse>& rhs) const
{
    if constexpr (!reverse) {
        return this->buffer >= rhs.buffer;
    }
    else {
        return this->buffer <= rhs.buffer;
    }
}

template <typename char_type, bool reverse>
inline bool
CheckedIndexer<char_type, reverse>::operator<(const CheckedIndexer<char_type, reverse>& rhs) const
{
    if constexpr (!reverse) {
        return this->buffer < rhs.buffer;
    }
    else {
        return this->buffer > rhs.buffer;
    }
}

template <typename char_type, bool reverse>
inline bool
CheckedIndexer<char_type, reverse>::operator<=(const CheckedIndexer<char_type, reverse>& rhs) const
{
    if constexpr (!reverse) {
        return this->buffer <= rhs.buffer;
    }
    else {
        return this->buffer >= rhs.buffer;
    }
}

template <typename char_type, bool reverse>
inline bool
CheckedIndexer<char_type, reverse>::operator==(const CheckedIndexer<char_type, reverse>& rhs) const
{
    return this->buffer == rhs.buffer;
}

template <typename char_type, bool reverse>
inline bool
CheckedIndexer<char_type, reverse>::operator!=(const CheckedIndexer<char_type, reverse>& rhs) const
{
    return this->buffer != rhs.buffer;
}

template <typename char_type, bool reverse>
inline int
CheckedIndexer<char_type, reverse>::cmp(const CheckedIndexer<char_type, reverse>& rhs, const ca_size_t len) const {
    assert(length >= len);
    assert(rhs.length >= len);

    for (int i = 0; i < len; i++) {
        if (operator[](i) != rhs[i]) {
            if (operator[](i) > rhs[i]) {
                return 1;
            }
            else {
                return -1;
            }
        }
    }

    return 0;
}

template <typename char_type, bool reverse>
inline char_type *
CheckedIndexer<char_type, reverse>::get_buffer() const {
    return buffer;
}

template <typename char_type>
inline bool
find_char(const CheckedIndexer<char_type, false> str, const ca_size_t n, const char_type ch,
          ca_size_t *index) {
    static_assert(!str.is_reverse);
    assert(index != nullptr);

    char_type *buf = str.get_buffer();
    char_type *after = (str + n).get_buffer();

    constexpr bool use_memchr = (sizeof(char_type) == sizeof(char));
    constexpr bool use_wmemchr = (sizeof(char_type) == sizeof(wchar_t));

    if (n > MEMCHR_CUT_OFF && (use_memchr || use_wmemchr)) {
        if constexpr (use_memchr) {
            buf = static_cast<char_type *>(memchr(buf, ch, n));
        } else {
            buf = static_cast<char_type *>(wmemchr(buf, ch, n));
        }

        if (buf != nullptr) {
            *index = buf - str.get_buffer();
            return true;
        }
        return false;
    }

    while (buf < after) {
        if (*buf == ch) {
            *index = buf - str.get_buffer();
            return true;
        }
        ++buf;
    }
    return false;
}

template <typename char_type>
inline bool
rfind_char(const CheckedIndexer<char_type, false> str, const ca_size_t n, const char_type ch,
           ca_size_t *index) {
    static_assert(!str.is_reverse);
    assert(index != nullptr);

    char_type *buf = (str + n).get_buffer();
    while (buf > str.get_buffer()) {
        --buf;
        if (*buf == ch) {
            *index = buf - str.get_buffer();
            return true;
        }
    }
    return false;
}

template <typename char_type, bool from_right>
static inline ca_size_t
count_char(const CheckedIndexer<char_type, from_right> str, const ca_size_t n,
           const char_type ch, const ca_size_t max_count)
{
    ca_size_t count = 0;
    for (ca_size_t i = 0; i < n; i++) {
        if (str[i] == ch) {
            count++;
            if (count == max_count) {
                break;
            }
        }
    }
    return count;
}

template <typename char_type, bool from_right>
inline ca_size_t
lex_search(const CheckedIndexer<char_type, from_right> pattern, const ca_size_t pattern_len,
           ca_size_t *return_period, const bool invert_alphabet)
{
    assert(return_period != nullptr);

    ca_size_t max_suffix = 0; // Index of the current maximal suffix found.
    ca_size_t candidate = 1;  // Candidate index for potential maximal suffix.
    ca_size_t k = 0;          // Offset for comparing characters.
    ca_size_t period = 1;     // Period of the right half.

    while (candidate + k < pattern_len) {
        // each loop increases candidate + k + max_suffix
        char_type a = pattern[candidate + k];
        char_type b = pattern[max_suffix + k];
        // check if the suffix at candidate is better than max_suffix
        if (invert_alphabet ? (b < a) : (a < b)) {
            // Fell short of max_suffix.
            // The next k + 1 characters are non-increasing
            // from candidate, so they won't start a maximal suffix.
            candidate += k + 1;
            k = 0;
            // We've ruled out any period smaller than what's
            // been scanned since max_suffix.
            period = candidate - max_suffix;
        }
        else if (a == b) {
            if (k + 1 != period) {
                // Keep scanning the equal strings
                k++;
            }
            else {
                // Matched a whole period.
                // Start matching the next period.
                candidate += period;
                k = 0;
            }
        }
        else {
            // Did better than max_suffix, so replace it.
            max_suffix = candidate;
            candidate++;
            k = 0;
            period = 1;
        }
    }

    *return_period = period;
    return max_suffix;
}

template <typename char_type, bool from_right>
inline ca_size_t
factorize(const CheckedIndexer<char_type, from_right> pattern, const ca_size_t pattern_len,
          ca_size_t *return_period)
{
    ca_size_t period1, period2, cut, period;

    // Perform lexicographical search to find the first cut (normal order)
    const ca_size_t cut1 = lex_search<char_type>(pattern, pattern_len, &period1, false);
    // Perform lexicographical search to find the second cut (reversed alphabet order)
    const ca_size_t cut2 = lex_search<char_type>(pattern, pattern_len, &period2, true);

    // Take the later cut.
    if (cut1 > cut2) {
        period = period1;
        cut = cut1;
    }
    else {
        period = period2;
        cut = cut2;
    }

    *return_period = period;
    return cut;
}

template <typename char_type, bool from_right>
void
preprocess(const CheckedIndexer<char_type, from_right> pattern, const ca_size_t pattern_len,
           prework<char_type, from_right> *work)
{
    assert(work != nullptr);

    // Store the pattern and its length, find the cut point and period.
    work->str = pattern;
    work->len = pattern_len;
    work->cut = factorize(pattern, pattern_len, &(work->period));
    assert(work->period + work->cut <= pattern_len);

    // Compare parts of the pattern to check for periodicity.
    const int cmp = pattern.cmp((pattern + work->period), work->cut);
    work->is_periodic = (0 == cmp);

    // If periodic, gap is unused; otherwise, calculate period and gap.
    if (work->is_periodic) {
        assert(work->cut <= pattern_len/2);
        assert(work->cut < work->period);
        work->gap = 0; // unused
    }
    else {
        // A lower bound on the period
        work->period = ca_math::ca_max(work->cut, pattern_len - work->cut) + 1;
        // The gap between the last character and the previous
        // occurrence of an equivalent character (modulo TABLE_SIZE)
        work->gap = pattern_len;
        char_type last = pattern[pattern_len - 1] & TABLE_MASK;
        for (ca_size_t i = pattern_len - 1; i > 0; i--) {
            char_type x = pattern[i - 1] & TABLE_MASK;
            if (x == last) {
                work->gap = pattern_len - i;
                break;
            }
        }
    }

    // Fill up a compressed Boyer-Moore "Bad Character" table
    const SHIFT_TYPE not_found_shift =
            ca_math::ca_safe_downcast<ca_size_t, SHIFT_TYPE>(
                    ca_math::ca_min(pattern_len, MAX_SHIFT)
            );
    for (ca_size_t i = 0; i < TABLE_SIZE; i++) {
        work->table[i] = not_found_shift;
    }
    for (ca_size_t i = pattern_len - not_found_shift; i < pattern_len; i++) {
        SHIFT_TYPE shift =
            ca_math::ca_safe_downcast<ca_size_t, SHIFT_TYPE>(pattern_len - 1 - i);
        work->table[pattern[i] & TABLE_MASK] = shift;
    }
}

template <typename char_type, bool from_right>
inline bool
two_way_periodic(const CheckedIndexer<char_type, from_right> str, const ca_size_t str_len,
                 prework<char_type, from_right> *work, ca_size_t *index) {
    // Initialize key variables for search.
    const ca_size_t pattern_len = work->len;
    const ca_size_t cut = work->cut;
    ca_size_t period = work->period;
    SHIFT_TYPE *table = work->table;

    CheckedIndexer<char_type, from_right> pattern = work->str;
    CheckedIndexer<char_type, from_right> window_last = str + (pattern_len - 1);
    CheckedIndexer<char_type, from_right> str_after = str + str_len;
    CheckedIndexer<char_type, from_right> window;

    // Handle the case where the pattern is periodic.
    // Memory optimization is used to skip over already checked segments.
    // It means that the beginning `memory` of characters of `window` have been checked.
    ca_size_t memory = 0;
    while (window_last < str_after) {
        // Bad-character shift loop to skip parts of the str.
        ca_size_t shift = table[window_last[0] & TABLE_MASK];
        while (shift != 0) {
            window_last += shift;
            if (window_last >= str_after) {
                return false;
            }
            shift = table[window_last[0] & TABLE_MASK];
        }

        // Begin checking the window.
        window = window_last - pattern_len + 1;
        assert((window[pattern_len - 1] & TABLE_MASK) ==
               (pattern[pattern_len - 1] & TABLE_MASK));

        // Check if the right half of the pattern matches the str.
        ca_size_t i = ca_math::ca_max(cut, memory);
        for (; i < pattern_len; ++i) {
            if (pattern[i] != window[i]) {

                // Right half does not match.
                window_last += (i - cut + 1);
                memory = 0;
                goto continue_flag;
            }
        }

        // Check if the left half of the pattern matches the str.
        for (i = memory; i < cut; ++i) {
            if (pattern[i] != window[i]) {

                // Left half does not match.
                window_last += period;
                memory = pattern_len - period;
                if (window_last >= str_after) {
                    return false;
                }
                // Apply memory adjustments and shifts if mismatches occur.
                shift = table[window_last[0] & TABLE_MASK];
                if (shift != 0) {
                    // A mismatch has been identified to the right
                    // of where i will next start, so we can jump
                    // at least as far as if the mismatch occurred
                    // on the first comparison.
                    const ca_size_t mem_jump = ca_math::ca_max(cut, memory) - cut + 1;

                    // Skip with Memory.
                    memory = 0;
                    window_last += ca_math::ca_max(shift, mem_jump);
                }
                goto continue_flag;
            }
        }

        // Found a match!
        *index = window - str;
        return true;

        continue_flag:;
    }

    return false;
}

template <typename char_type, bool from_right>
inline bool
two_way_not_periodic(const CheckedIndexer<char_type, from_right> str, const ca_size_t str_len,
                     prework<char_type, from_right> *work, ca_size_t *index) {
    // Initialize key variables for search.
    const ca_size_t pattern_len = work->len;
    const ca_size_t cut = work->cut;
    ca_size_t period = work->period;
    SHIFT_TYPE *table = work->table;

    CheckedIndexer<char_type, from_right> pattern = work->str;
    CheckedIndexer<char_type, from_right> window_last = str + (pattern_len - 1);
    CheckedIndexer<char_type, from_right> str_after = str + str_len;
    CheckedIndexer<char_type, from_right> window;

    // Handle the case where the pattern is non-periodic.
    // General shift logic based on a gap is used to improve performance.
    ca_size_t gap = work->gap;
    period = ca_math::ca_max(gap, period);

    // Pattern is not periodic.
    const ca_size_t gap_jump_end = ca_math::ca_min(pattern_len, cut + gap);

    while (window_last < str_after) {
        // Bad-character shift loop to skip parts of the str.
        ca_size_t shift = table[window_last[0] & TABLE_MASK];
        while (shift != 0) {
            window_last += shift;
            if (window_last >= str_after) {
                return false;
            }
            shift = table[window_last[0] & TABLE_MASK];
        }

        // Begin checking the window.
        window = window_last - pattern_len + 1;
        assert((window[pattern_len - 1] & TABLE_MASK) ==
               (pattern[pattern_len - 1] & TABLE_MASK));

        // Check the right half of the pattern for a match.
        for (ca_size_t i = cut; i < gap_jump_end; ++i) {
            if (pattern[i] != window[i]) {

                // Early right half mismatch: jump by gap.
                assert(gap >= i - cut + 1);
                window_last += gap;
                goto continue_flag;
            }
        }

        // Continue checking the remaining right half of the pattern.
        for (ca_size_t i = gap_jump_end; i < pattern_len; ++i) {
            if (pattern[i] != window[i]) {

                // Late right half mismatch.
                assert(i - cut + 1 > gap);
                window_last += i - cut + 1;
                goto continue_flag;
            }
        }

        // Check the left half of the pattern for a match.
        for (ca_size_t i = 0; i < cut; i++) {
            if (pattern[i] != window[i]) {

                // Left half does not match.
                window_last += period;
                goto continue_flag;
            }
        }

        // Found a match!
        *index = window - str;
        return true;

        continue_flag:;
    }

    return false;
}

template <typename char_type, bool from_right>
bool
two_way(const CheckedIndexer<char_type, from_right> str, const ca_size_t str_len,
        prework<char_type, from_right> *work, ca_size_t *index)
{
    assert(work != nullptr);
    assert(index != nullptr);

    if (work -> is_periodic) {
        return two_way_periodic(str, str_len, work, index);
    }
    else {
        return two_way_not_periodic(str, str_len, work, index);
    }
}

template <typename char_type, bool from_right>
inline bool
two_way_find(const CheckedIndexer<char_type, from_right> str, const ca_size_t str_len,
             const CheckedIndexer<char_type, from_right> pattern, const ca_size_t pattern_len,
             ca_size_t *index)
{
    assert(index != nullptr);
    static_assert(str.is_reverse == pattern.is_reverse);

    prework<char_type, from_right> work;
    preprocess(pattern, pattern_len, &work);

    const bool found = two_way(str, str_len, &work, index);

    if constexpr(str.is_reverse) {
        if (found) {
            *index = str_len  - pattern_len - *index;
        }
    }

    return found;
}

template <typename char_type, bool from_right>
inline ca_size_t
two_way_count(const CheckedIndexer<char_type, from_right> str, const ca_size_t str_len,
              const CheckedIndexer<char_type, from_right> pattern, const ca_size_t pattern_len,
              const ca_size_t max_count)
{
    static_assert(str.is_reverse == pattern.is_reverse);

    prework<char_type, from_right> p;
    preprocess(pattern, pattern_len, &p);
    ca_size_t index = 0;
    ca_size_t count = 0;
    ca_size_t tmp;

    while (count < max_count) {
        if (!two_way(str + index, str_len - index, &p, &tmp)) {
            break;
        }

        count++;
        index += tmp + pattern_len;
    }
    return count;
}

template <typename char_type, bool from_right>
inline bool
default_find(const CheckedIndexer<char_type, from_right> str, const ca_size_t str_len,
             const CheckedIndexer<char_type, from_right> pattern, const ca_size_t pattern_len,
             ca_size_t *index)
{
    assert(index != nullptr);
    static_assert(str.is_reverse == pattern.is_reverse);

    const ca_size_t width = str_len - pattern_len;
    const ca_size_t last_index = pattern_len - 1;
    const char_type last = pattern[last_index];
    ca_size_t gap = pattern_len - 1;
    CheckedIndexer<char_type, from_right> ss = str + (pattern_len - 1);

    // Add pattern to bloom filter and calculate the gap.
    ca_uint64_t mask = 0;
    for (ca_size_t i = 0; i < last_index; i++) {
        bloom_add(mask, pattern[i]);
        if (pattern[i] == last) {
            gap = last_index - i - 1;
        }
    }
    bloom_add(mask, last);

    for (ca_size_t i = 0; i <= width; i++) {
        if (ss[i] == last) {
            /* candidate match */
            ca_size_t j;
            for (j = 0; j < last_index; j++) {
                if (str[i+j] != pattern[j]) {
                    break;
                }
            }
            if (j == last_index) {
                /* got a match! */
                *index = i;
                return true;
            }
            /* miss: check if next character is part of pattern */
            if (!bloom_find(mask, ss[i+1])) {
                i = i + pattern_len;
            }
            else {
                i = i + gap;
            }
        }
        else {
            /* skip: check if next character is part of pattern */
            if (!bloom_find(mask, ss[i+1])) {
                i = i + pattern_len;
            }
        }
    }
    return false;
}

template <typename char_type, bool from_right>
inline ca_size_t
default_count(const CheckedIndexer<char_type, from_right> str, const ca_size_t str_len,
              const CheckedIndexer<char_type, from_right> pattern, const ca_size_t pattern_len,
              const ca_size_t max_count)
{
    static_assert(str.is_reverse == pattern.is_reverse);

    const ca_size_t width = str_len - pattern_len;
    const ca_size_t last_index = pattern_len - 1;
    const char_type last = pattern[last_index];
    ca_size_t count = 0;
    ca_size_t gap = pattern_len - 1;
    CheckedIndexer<char_type, from_right> ss = str + (pattern_len - 1);

    // Add pattern to bloom filter and calculate the gap.
    ca_uint64_t mask = 0;
    for (ca_size_t i = 0; i < last_index; i++) {
        bloom_add(mask, pattern[i]);
        if (pattern[i] == last) {
            gap = last_index - i - 1;
        }
    }
    bloom_add(mask, last);

    for (ca_size_t i = 0; i <= width; i++) {
        if (ss[i] == last) {
            /* candidate match */
            ca_size_t j;
            for (j = 0; j < last_index; j++) {
                if (str[i+j] != pattern[j]) {
                    break;
                }
            }
            if (j == last_index) {
                /* got a match! */
                count++;

                if (count == max_count) {
                    return max_count;
                }

                i = i + last_index;
                continue;
            }
            /* miss: check if next character is part of pattern */
            if (!bloom_find(mask, ss[i+1])) {
                i = i + pattern_len;
            }
            else {
                i = i + gap;
            }
        }
        else {
            /* skip: check if next character is part of pattern */
            if (!bloom_find(mask, ss[i+1])) {
                i = i + pattern_len;
            }
        }
    }

    return count;
}

template <typename char_type, bool from_right>
inline bool
adaptive_find(const CheckedIndexer<char_type, from_right> str, const ca_size_t str_len,
              const CheckedIndexer<char_type, from_right> pattern, const ca_size_t pattern_len,
              ca_size_t *index)
{
    assert(index != nullptr);
    static_assert(str.is_reverse == pattern.is_reverse);

    const ca_size_t width = str_len - pattern_len;
    const ca_size_t last_index = pattern_len - 1;
    const char_type last = pattern[last_index];
    ca_size_t gap = pattern_len - 1;
    ca_size_t hits = 0;
    CheckedIndexer<char_type, from_right> ss = str + (pattern_len - 1);

    // Add pattern to bloom filter and calculate the gap.
    ca_uint64_t mask = 0;
    for (ca_size_t i = 0; i < last_index; i++) {
        bloom_add(mask, pattern[i]);
        if (pattern[i] == last) {
            gap = last_index - i - 1;
        }
    }
    bloom_add(mask, last);

    for (ca_size_t i = 0; i <= width; i++) {
        if (ss[i] == last) {
            /* candidate match */
            ca_size_t j;
            for (j = 0; j < last_index; j++) {
                if (str[i+j] != pattern[j]) {
                    break;
                }
            }
            if (j == last_index) {
                /* got a match! */
                return i;
            }
            hits += j + 1;
            if (hits > pattern_len / 4 && width - i > 2000) {
                const bool res = two_way_find(str + i, str_len - i, pattern, pattern_len, index);
                if (res) {
                    *index += i;
                }
                return res;
            }
            /* miss: check if next character is part of pattern */
            if (!bloom_find(mask, ss[i+1])) {
                i = i + pattern_len;
            }
            else {
                i = i + gap;
            }
        }
        else {
            /* skip: check if next character is part of pattern */
            if (!bloom_find(mask, ss[i+1])) {
                i = i + pattern_len;
            }
        }
    }
    return false;
}

template <typename char_type, bool from_right>
inline ca_size_t
adaptive_count(const CheckedIndexer<char_type, from_right> str, const ca_size_t str_len,
               const CheckedIndexer<char_type, from_right> pattern, const ca_size_t pattern_len,
               const ca_size_t max_count)
{
    static_assert(str.is_reverse == pattern.is_reverse);

    const ca_size_t width = str_len - pattern_len;
    const ca_size_t last_index = pattern_len - 1;
    const char_type last = pattern[last_index];
    ca_size_t count = 0;
    ca_size_t gap = pattern_len - 1;
    ca_size_t hits = 0;
    CheckedIndexer<char_type, from_right> ss = str + (pattern_len - 1);

    // Add pattern to bloom filter and calculate the gap.
    ca_uint64_t mask = 0;
    for (ca_size_t i = 0; i < last_index; i++) {
        bloom_add(mask, pattern[i]);
        if (pattern[i] == last) {
            gap = last_index - i - 1;
        }
    }
    bloom_add(mask, last);

    for (ca_size_t i = 0; i <= width; i++) {
        if (ss[i] == last) {
            /* candidate match */
            ca_size_t j;
            for (j = 0; j < last_index; j++) {
                if (str[i+j] != pattern[j]) {
                    break;
                }
            }
            if (j == last_index) {
                /* got a match! */
                count++;

                if (count == max_count) {
                    return max_count;
                }

                i = i + last_index;
                continue;
            }
            hits += j + 1;
            if (hits > pattern_len / 4 && width - i > 2000) {
                count += two_way_count(str + i, str_len - i, pattern, pattern_len, max_count - count);
                return count;
            }
            /* miss: check if next character is part of pattern */
            if (!bloom_find(mask, ss[i+1])) {
                i = i + pattern_len;
            }
            else {
                i = i + gap;
            }
        }
        else {
            /* skip: check if next character is part of pattern */
            if (!bloom_find(mask, ss[i+1])) {
                i = i + pattern_len;
            }
        }
    }
    return count;
}

}

template<typename char_type, bool from_right>
inline bool
ca_fastsearch(char_type* str, const ca_size_t str_len,
              char_type* pattern, const ca_size_t pattern_len,
              ca_size_t *index) {
    assert(index != nullptr);

    if (str_len < pattern_len || pattern_len == 0) {
        return false;
    }

    if (pattern_len == 1) {
        fastsearch::CheckedIndexer<char_type, false> s(str, str_len);
        if constexpr (!from_right) {
            return fastsearch::find_char(s, str_len, pattern[0], index);
        }
        else {
            return fastsearch::rfind_char(s, str_len, pattern[0], index);
        }
    }

    fastsearch::CheckedIndexer<char_type, from_right> s(str, str_len);
    fastsearch::CheckedIndexer<char_type, from_right> p(pattern, pattern_len);

    bool find;

    if (str_len < 2500 || (pattern_len < 100 && str_len < 30000) || pattern_len < 6) {
        find = fastsearch::default_find(s, str_len, p, pattern_len, index);
    }
    else if ((pattern_len >> 2) * 3 < (str_len >> 2)) {
        /* 33% threshold, but don't overflow. */
        /* For larger problems where the needle isn't a huge
           percentage of the size of the haystack, the relatively
           expensive O(pattern_len) startup cost of the two-way algorithm
           will surely pay off. */
        find = fastsearch::two_way_find(s, str_len, p, pattern_len, index);
    }
    else {
        // ReSharper restore CppRedundantElseKeyword
        /* To ensure that we have good worst-case behavior,
           here's an adaptive version of the algorithm, where if
           we match O(pattern_len) characters without any matches of the
           entire needle, then we predict that the startup cost of
           the two-way algorithm will probably be worth it. */
        find = fastsearch::adaptive_find(s, str_len, p, pattern_len, index);
    }

    if constexpr (from_right) {
        if (find) {
            *index = str_len - pattern_len - *index;
        }
    }
    return find;
}

template<typename char_type, bool from_right>
inline void
ca_fastcount(char_type* str, const ca_size_t str_len,
             char_type* pattern, const ca_size_t pattern_len,
             const ca_size_t max_count, ca_size_t *count) {
    assert(count != nullptr);

    if (str_len < pattern_len || pattern_len == 0 || max_count == 0) {
        *count = 0;
        return;
    }

    fastsearch::CheckedIndexer<char_type, from_right> s(str, str_len);
    fastsearch::CheckedIndexer<char_type, from_right> p(pattern, pattern_len);

    if (pattern_len == 1) {
        *count = fastsearch::count_char(s, str_len, pattern[0], max_count);
        return;
    }

    if (str_len < 2500 || (pattern_len < 100 && str_len < 30000) || pattern_len < 6) {
        *count = fastsearch::default_count(s, str_len, p, pattern_len, max_count);
    }
    else if ((pattern_len >> 2) * 3 < (str_len >> 2)) {
        /* 33% threshold, but don't overflow. */
        /* For larger problems where the needle isn't a huge
           percentage of the size of the haystack, the relatively
           expensive O(pattern_len) startup cost of the two-way algorithm
           will surely pay off. */
        *count = fastsearch::two_way_count(s, str_len, p, pattern_len, max_count);
    }
    else {
        // ReSharper restore CppRedundantElseKeyword
        /* To ensure that we have good worst-case behavior,
           here's an adaptive version of the algorithm, where if
           we match O(pattern_len) characters without any matches of the
           entire needle, then we predict that the startup cost of
           the two-way algorithm will probably be worth it. */
        *count = fastsearch::adaptive_count(s, str_len, p, pattern_len, max_count);
    }
}

}
