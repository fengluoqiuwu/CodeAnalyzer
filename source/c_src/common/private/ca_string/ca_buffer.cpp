// ================================
// CodeAnalyzer - source/c_src/common/private/ca_string/ca_string.h
//
// @file
// @brief Implements the `ca_buffer` template class for character buffer manipulation
//        across different encodings (ASCII, UTF-8, UTF-32). Includes methods for
//        buffer management, memory operations, character checks, and comparison.
// ================================

#include "cstring"

#include "ca_buffer.h"

#include <algorithm>

#include "ca_char.h"
#include "ca_utf8_utils.h"

namespace ca::ca_string {

namespace {
/**
 * @brief A functor for calling specific unary buffer functions based on the specified operation.
 *
 * This template struct enables calling various unary functions (e.g., ISALPHA, ISDIGIT, etc.) on a given
 * buffer of type `ca_buffer<encoding>`. The function to be applied is determined by the `function`
 * template parameter, and the result of the function call is returned as the type `T`.
 *
 * The following unary functions are supported:
 * - ISALPHA
 * - ISDECIMAL
 * - ISDIGIT
 * - ISSPACE
 * - ISALNUM
 * - ISNUMERIC
 *
 * The static assertion ensures that only valid function values are used.
 *
 * @tparam encoding The encoding type used by the buffer.
 * @tparam function The unary function to be applied (e.g., ISALPHA).
 * @tparam T The return type of the unary function.
 */
template <ca_encoding_t encoding, ca_buffer_implemented_unary_functions function, typename T>
struct call_buffer_member_function {
    static_assert(function == ca_buffer_implemented_unary_functions::ISALPHA ||
                  function == ca_buffer_implemented_unary_functions::ISDECIMAL ||
                  function == ca_buffer_implemented_unary_functions::ISDIGIT ||
                  function == ca_buffer_implemented_unary_functions::ISSPACE ||
                  function == ca_buffer_implemented_unary_functions::ISALNUM ||
                  function == ca_buffer_implemented_unary_functions::ISNUMERIC,
                  "Invalid ca_buffer_implemented_unary_functions value "
                  "in call_buffer_member_function.");

    /**
     * @brief Calls the specified unary function on the provided buffer.
     *
     * @param buffer The buffer on which the unary function will be applied.
     * @return The result of the unary function.
     */
    T operator()(ca_buffer<encoding> buffer) {
        switch (function) {
            case ca_buffer_implemented_unary_functions::ISALPHA:
                return ca_isalpha<encoding>(*buffer);
            case ca_buffer_implemented_unary_functions::ISDECIMAL:
                return ca_isdecimal<encoding>(*buffer);
            case ca_buffer_implemented_unary_functions::ISDIGIT:
                return ca_isdigit<encoding>(*buffer);
            case ca_buffer_implemented_unary_functions::ISSPACE:
                return ca_isspace<encoding>(*buffer);
            case ca_buffer_implemented_unary_functions::ISALNUM:
                return ca_isalnum<encoding>(*buffer);
            case ca_buffer_implemented_unary_functions::ISNUMERIC:
                return ca_isnumeric<encoding>(*buffer);
            default:
                return false;
        }
    }
};
}

// ----------------------------
// Member function implementations of ca_buffer
// ----------------------------

template<ca_encoding_t encoding>
inline
ca_buffer<encoding>::ca_buffer() {
    buf = nullptr;
    after = nullptr;
}

template<ca_encoding_t encoding>
inline
ca_buffer<encoding>::ca_buffer(ca_char_t *buf_, const ca_size_t size) {
    buf = buf_;
    after = buf + size;
}

template<ca_encoding_t encoding>
inline ca_size_t
ca_buffer<encoding>::num_codepoints() const {
    ca_buffer<encoding> tmp(after, 0);
    ca_size_t num_codepoints;

    switch (encoding) {
        case ca_encoding_t::CA_ENCODING_ASCII:
        case ca_encoding_t::CA_ENCODING_UTF32:
        {
            --tmp;
            while (tmp >= *this && *tmp == '\0') {
                --tmp;
            }
            num_codepoints = static_cast<ca_size_t>(tmp - *this + 1);
            break;
        }
        case ca_encoding_t::CA_ENCODING_UTF8:
        {
            utf8::num_codepoints_for_utf8_bytes_without_check(
                    buf, after - buf, &num_codepoints);
            break;
        }
    }

    return num_codepoints;
}

template<ca_encoding_t encoding>
inline ca_buffer<encoding> &
ca_buffer<encoding>::operator+=(const ca_int64_t n) {
    switch (encoding) {
        case ca_encoding_t::CA_ENCODING_ASCII:
        {
            buf += n;
            break;
        }
        case ca_encoding_t::CA_ENCODING_UTF32:
        {
            buf += n * sizeof(ca_char4_t);
            break;
        }
        case ca_encoding_t::CA_ENCODING_UTF8:
        {
            for (int i = 0; i < n; i++) {
                buf += utf8::num_utf8_bytes_for_utf8_character_without_check(buf);
            }
            break;
        }
    }
    return *this;
}

template<ca_encoding_t encoding>
inline ca_buffer<encoding> &
ca_buffer<encoding>::operator-=(const ca_int64_t n) {
    switch (encoding) {
        case ca_encoding_t::CA_ENCODING_ASCII:
        {
            buf -= n;
            break;
        }
        case ca_encoding_t::CA_ENCODING_UTF32:
        {
            buf -= n * sizeof(ca_char4_t);
            break;
        }
        case ca_encoding_t::CA_ENCODING_UTF8:
        {
            utf8::find_previous_utf8_character(buf, n, &buf);
            break;
        }
    }
    return *this;
}

template<ca_encoding_t encoding>
inline ca_buffer<encoding> &
ca_buffer<encoding>::operator++() {
    *this += 1;
    return *this;
}

template<ca_encoding_t encoding>
inline ca_buffer<encoding>
ca_buffer<encoding>::operator++(int) {
    ca_buffer<encoding> tmp = *this;
    *this += 1;
    return tmp;
}

template<ca_encoding_t encoding>
inline ca_buffer<encoding> &
ca_buffer<encoding>::operator--() {
    *this -= 1;
    return *this;
}

template<ca_encoding_t encoding>
inline ca_buffer<encoding>
ca_buffer<encoding>::operator--(int) {
    ca_buffer<encoding> tmp = *this;
    *this -= 1;
    return tmp;
}

template<ca_encoding_t encoding>
inline ca_char4_t
ca_buffer<encoding>::operator*() const {
    int size;
    return ca_getchar<encoding>(buf, &size);
}

template<ca_encoding_t encoding>
inline bool
ca_buffer<encoding>::empty() const {
    return (buf == nullptr || after == nullptr || buf >= after);
}

template<ca_encoding_t encoding>
inline int
ca_buffer<encoding>::buffer_memcmp(const ca_buffer<encoding> other, const ca_size_t len) const {
    if (len == 0) {
        return 0;
    }

    switch (encoding) {
        case ca_encoding_t::CA_ENCODING_ASCII:
        case ca_encoding_t::CA_ENCODING_UTF8:
            // for UTF8 we treat n_chars as number of bytes
            return memcmp(other.buf, buf, len);
        case ca_encoding_t::CA_ENCODING_UTF32:
            return memcmp(other.buf, buf, len * sizeof(ca_char4_t));
    }

    return 0;
}

template<ca_encoding_t encoding>
inline void
ca_buffer<encoding>::buffer_memcpy(ca_buffer<encoding> other, const ca_size_t len) const {
    if (len == 0) {
        return;
    }

    switch (encoding) {
    case ca_encoding_t::CA_ENCODING_ASCII:
    case ca_encoding_t::CA_ENCODING_UTF8:
    {
        // for UTF8 we treat n_chars as number of bytes
        memcpy(other.buf, buf, len);
        break;
    }
    case ca_encoding_t::CA_ENCODING_UTF32:
    {
        memcpy(other.buf, buf, len * sizeof(ca_char4_t));
        break;
    }
    }
}

template<ca_encoding_t encoding>
inline ca_size_t
ca_buffer<encoding>::buffer_memset(const ca_char4_t fill_char, const ca_size_t n_chars) {
    if (n_chars == 0) {
        return 0;
    }

    switch (encoding) {
    case ca_encoding_t::CA_ENCODING_ASCII:
    {
        memset(buf, static_cast<ca_char_t>(fill_char), n_chars);
        return n_chars;
    }
    case ca_encoding_t::CA_ENCODING_UTF32:
    {
        std::fill_n(buf, n_chars, fill_char);
        return n_chars;
    }
    case ca_encoding_t::CA_ENCODING_UTF8:
    {
        ca_char_t utf8_c[4] = {0};
        ca_char_t *tmp = buf;

        const ca_size_t size = utf8::ucs4_code_to_utf8_char_without_check(fill_char, utf8_c);

        if (n_chars < 64) {
            for (int i = 0; i < n_chars; ++i) {
                memcpy(tmp, utf8_c, size);
                tmp += size;
            }
        }
        else {
            memcpy(tmp, utf8_c, size);
            const ca_size_t times = n_chars / 64;
            const ca_size_t batch_size = size * 64;
            const ca_size_t remain_bytes = (n_chars - times * 64) * size;

            for (ca_uint64_t i = size; i != 64 * size; i *= 2) {
                const ca_char_t *temp = tmp;
                tmp += i;
                memcpy(tmp, temp, i);
            }

            for (int i = 1; i < times; i++) {
                tmp += batch_size;
                memcpy(buf, tmp, batch_size);
            }

            memcpy(buf, tmp, remain_bytes);
        }

        return n_chars * size;
    }
    }

    return 0;
}

template<ca_encoding_t encoding>
void ca_buffer<encoding>::buffer_fill_with_zeros_after_index(size_t start_index) {
    ca_buffer<encoding> tmp = *this + start_index;
    std::fill(tmp.buf, after, 0);
}

template<ca_encoding_t encoding>
void ca_buffer<encoding>::advance_lens(ca_size_t len) {
    switch (encoding) {
    case ca_encoding_t::CA_ENCODING_ASCII:
    case ca_encoding_t::CA_ENCODING_UTF32:
    {
        *this += len;
        break;
    }
    case ca_encoding_t::CA_ENCODING_UTF8:
    {
        buf += len;
        break;
    }
    }
}

template<ca_encoding_t encoding>
ca_size_t ca_buffer<encoding>::num_bytes_next_character() const {
    return ca_get_bytes<encoding>(buf);
}

template<ca_encoding_t encoding>
template<ca_buffer_implemented_unary_functions unary_type>
bool ca_buffer<encoding>::unary_loop() const {
    size_t num_codepoints = num_codepoints();

    if (num_codepoints == 0) {
        return false;
    }

    ca_buffer<encoding> tmp = *this;
    call_buffer_member_function<encoding, unary_type, bool> function;

    for (size_t i = 0; i < num_codepoints; ++i) {
        if (const bool result = function(tmp); !result) {
            return false;
        }
        ++tmp;
    }

    return true;
}

template<ca_encoding_t encoding>
bool ca_buffer<encoding>::is_alpha() const {
    return unary_loop<ca_buffer_implemented_unary_functions::ISALPHA>();
}

template<ca_encoding_t encoding>
bool ca_buffer<encoding>::is_decimal() const {
    return unary_loop<ca_buffer_implemented_unary_functions::ISDECIMAL>();
}

template<ca_encoding_t encoding>
bool ca_buffer<encoding>::is_digit() const {
    return unary_loop<ca_buffer_implemented_unary_functions::ISDIGIT>();
}

template<ca_encoding_t encoding>
bool ca_buffer<encoding>::first_character_is_space() const {
    return ca_isspace<encoding>(buf);
}

template<ca_encoding_t encoding>
bool ca_buffer<encoding>::is_space() const {
    return unary_loop<ca_buffer_implemented_unary_functions::ISSPACE>();
}

template<ca_encoding_t encoding>
bool ca_buffer<encoding>::is_alphanumeric() const {
    return unary_loop<ca_buffer_implemented_unary_functions::ISALNUM>();
}

template<ca_encoding_t encoding>
bool ca_buffer<encoding>::islower() const {
    size_t num_codepoints = num_codepoints();
    if (num_codepoints == 0) {
        return false;
    }

    ca_buffer<encoding> tmp = *this;
    bool cased = false;
    for (size_t i = 0; i < num_codepoints; ++i) {
        if (ca_isupper<encoding>(*tmp) || ca_istitle<encoding>(*tmp)) {
            return false;
        }
        else if (!cased && ca_islower<encoding>(*tmp)) {
            cased = true;
        }
        ++tmp;
    }
    return cased;
}

template<ca_encoding_t encoding>
bool ca_buffer<encoding>::isupper() const {
    size_t num_codepoints = num_codepoints();
    if (num_codepoints == 0) {
        return false;
    }

    ca_buffer<encoding> tmp = *this;
    bool cased = false;
    for (size_t i = 0; i < num_codepoints; ++i) {
        if (ca_islower<encoding>(*tmp) || ca_istitle<encoding>(*tmp)) {
            return false;
        }
        else if (!cased && ca_isupper<encoding>(*tmp)) {
            cased = true;
        }
        ++tmp;
    }
    return cased;
}

template<ca_encoding_t encoding>
bool ca_buffer<encoding>::istitle() const {
    size_t num_codepoints = num_codepoints();
    if (num_codepoints == 0) {
        return false;
    }

    ca_buffer<encoding> tmp = *this;
    bool cased = false;
    bool prev_cased = false;
    for (size_t i = 0; i < num_codepoints; ++i) {
        if (ca_isupper<encoding>(*tmp) || ca_istitle<encoding>(*tmp)) {
            if (prev_cased) {
                return false;
            }
            prev_cased = true;
            cased = true;
        }
        else if (ca_islower<encoding>(*tmp)) {
            if (!prev_cased) {
                return false;
            }
            cased = true;
        }
        else {
            prev_cased = false;
        }
        ++tmp;
    }
    return cased;
}

template<ca_encoding_t encoding>
bool ca_buffer<encoding>::isnumeric() const {
    return unary_loop<ca_buffer_implemented_unary_functions::ISNUMERIC>();
}

template<ca_encoding_t encoding>
ca_buffer<encoding> ca_buffer<encoding>::rstrip() const {
    ca_buffer<encoding> tmp(after, 0);

    --tmp;
    while (tmp >= *this && (*tmp == '\0' || ca_isspace<encoding>(*tmp))) {
        --tmp;
    }
    ++tmp;

    after = tmp.buf;
    return *this;
}

template<ca_encoding_t encoding>
int ca_buffer<encoding>::strcmp(ca_buffer<encoding> other, bool ignore_trailing_whitespace) const {
    ca_buffer<encoding> tmp1 = ignore_trailing_whitespace ? rstrip() : *this;
    ca_buffer<encoding> tmp2 = ignore_trailing_whitespace ? other.rstrip() : other;

    while (!tmp1.empty() && !tmp2.empty()) {
        if (*tmp1 < *tmp2) {
            return -1;
        }
        if (*tmp2 < *tmp1) {
            return 1;
        }
        ++tmp1;
        ++tmp2;
    }
    while (!tmp1.empty()) {
        if (*tmp1) {
            return 1;
        }
        ++tmp1;
    }
    while (!tmp2.empty()) {
        if (*tmp2) {
            return -1;
        }
        ++tmp2;
    }
    return 0;
}

}
