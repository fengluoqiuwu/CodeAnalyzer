

#include "ca_buffer.h"

namespace ca::ca_string {

// ----------------------------
// Member function implementations of ca_buffer
// ----------------------------

template<ca_encoding_t encoding>
ca_buffer<encoding>::ca_buffer() {
    buf = nullptr;
    after = nullptr;
}

template<ca_encoding_t encoding>
ca_buffer<encoding>::ca_buffer(ca_char_t *buf_, const ca_size_t size) {
    buf = buf_;
    after = buf + size;
}

template<ca_encoding_t encoding>
ca_size_t ca_buffer<encoding>::num_codepoints() const {
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
            break;
        }
    }

    return num_codepoints;
}

template<ca_encoding_t encoding>
ca_buffer<encoding> & ca_buffer<encoding>::operator+=(ca_int64_t n) {
}

template<ca_encoding_t encoding>
ca_buffer<encoding> & ca_buffer<encoding>::operator-=(ca_int64_t n) {
}

template<ca_encoding_t encoding>
ca_buffer<encoding> & ca_buffer<encoding>::operator++() {
}

template<ca_encoding_t encoding>
ca_buffer<encoding> & ca_buffer<encoding>::operator++(int) {
}

template<ca_encoding_t encoding>
ca_buffer<encoding> & ca_buffer<encoding>::operator--() {
}

template<ca_encoding_t encoding>
ca_buffer<encoding> & ca_buffer<encoding>::operator--(int) {
}

template<ca_encoding_t encoding>
ca_char4_t ca_buffer<encoding>::operator*() const {
}

template<ca_encoding_t encoding>
int ca_buffer<encoding>::buffer_memcmp(ca_buffer<encoding> other, ca_size_t len) const {
}

template<ca_encoding_t encoding>
void ca_buffer<encoding>::buffer_memcpy(ca_buffer<encoding> other, ca_size_t len) const {
}

template<ca_encoding_t encoding>
void ca_buffer<encoding>::buffer_memset(ca_char4_t fill_char, ca_size_t n_chars) const {
}

template<ca_encoding_t encoding>
void ca_buffer<encoding>::buffer_fill_with_zeros_after_index(size_t start_index) {
}

template<ca_encoding_t encoding>
void ca_buffer<encoding>::advance_lens(ca_size_t len) {
}

template<ca_encoding_t encoding>
ca_size_t ca_buffer<encoding>::num_bytes_next_character() const {
}

template<ca_encoding_t encoding>
template<ca_buffer_implemented_unary_functions unary_type>
bool ca_buffer<encoding>::unary_loop() const {
}

template<ca_encoding_t encoding>
bool ca_buffer<encoding>::is_alpha() const {
}

template<ca_encoding_t encoding>
bool ca_buffer<encoding>::is_decimal() const {
}

template<ca_encoding_t encoding>
bool ca_buffer<encoding>::is_digit() const {
}

template<ca_encoding_t encoding>
bool ca_buffer<encoding>::first_character_is_space() const {
}

template<ca_encoding_t encoding>
bool ca_buffer<encoding>::is_space() const {
}

template<ca_encoding_t encoding>
bool ca_buffer<encoding>::is_alphanumeric() const {
}

template<ca_encoding_t encoding>
bool ca_buffer<encoding>::islower() const {
}

template<ca_encoding_t encoding>
bool ca_buffer<encoding>::isupper() const {
}

template<ca_encoding_t encoding>
bool ca_buffer<encoding>::istitle() const {
}

template<ca_encoding_t encoding>
bool ca_buffer<encoding>::isnumeric() const {
}

template<ca_encoding_t encoding>
ca_buffer<encoding> ca_buffer<encoding>::rstrip() const {
}

template<ca_encoding_t encoding>
int ca_buffer<encoding>::strcmp(ca_buffer<encoding> other, bool ignore_trailing_whitespace) const {
}

// ----------------------------
// Miscellaneous functions
// ----------------------------

template <ca_encoding_t encoding, ca_buffer_implemented_unary_functions function, typename T>
T call_buffer_member_function<encoding, function, T>::operator()(ca_buffer<encoding> buffer) {
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

}
