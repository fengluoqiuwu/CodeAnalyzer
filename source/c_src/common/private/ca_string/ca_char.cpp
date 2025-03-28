// ================================
// CodeAnalyzer - source/c_src/common/public/ca_string/ca_char.cpp
//
// @file
// @brief Source file implementing functions for character types and encoding/decoding
//        operations, including utility functions for character classification.
// ================================

#include <Python.h>
#include "ca_char.h"
#include "ca_utf8_utils.h"

namespace ca::ca_string {
// ----------------------------
// template <ca_encoding_t encoding>
// inline int
// ca_get_bytes(const ca_char_t *c);
// ----------------------------

template <>
inline int
ca_get_bytes<ca_encoding_t::CA_ENCODING_ASCII>(const ca_char_t *c) {
    return 1; // ASCII is always 1 byte
}

template <>
inline int
ca_get_bytes<ca_encoding_t::CA_ENCODING_UTF8>(const ca_char_t *c) {
    // Check the first byte to determine the length of the UTF-8 character
    const auto byte1 = static_cast<ca_char_t>(*c);

    if ((byte1 & 0x80) == 0) {
        // 1-byte character (ASCII)
        return 1;
    }
    else if ((byte1 & 0xE0) == 0xC0) {
        // 2-byte character
        return 2;
    }
    else if ((byte1 & 0xF0) == 0xE0) {
        // 3-byte character
        return 3;
    }
    else if ((byte1 & 0xF8) == 0xF0) {
        // 4-byte character
        return 4;
    }

    return -1;
}

template <>
inline int
ca_get_bytes<ca_encoding_t::CA_ENCODING_UTF32>(const ca_char_t *c) {
    return 4; // UTF32 is always 4 bytes
}

template <ca_encoding_t encoding>
inline int
ca_get_bytes(const ca_char_t *c) {
    return -1;
}

// ----------------------------
// template <ca_encoding_t encoding>
// inline ca_char4_t
// ca_getchar(const ca_char_t *c, int *size);
// ----------------------------

template <>
inline ca_char4_t
ca_getchar<ca_encoding_t::CA_ENCODING_ASCII>(const ca_char_t *c, int *size) {
    *size = 1;
    // ASCII is a single byte character, directly cast
    return static_cast<ca_char4_t>(*c);
}

template <>
inline ca_char4_t
ca_getchar<ca_encoding_t::CA_ENCODING_UTF8>(const ca_char_t *c, int *size) {
    ca_char4_t result;
    *size = utf8::utf8_char_to_ucs4_code_without_check(c, &result);
    return result;
}

template <>
inline ca_char4_t
ca_getchar<ca_encoding_t::CA_ENCODING_UTF32>(const ca_char_t *c, int *size) {
    *size = 4;
    // Directly reinterpret the byte stream as a 4-byte character
    return *reinterpret_cast<const ca_char4_t *>(c);
}

template <ca_encoding_t encoding>
inline ca_char4_t
ca_getchar(const ca_char_t *c, int *size) {
    return CA_ERROR_VALUE;
}

// ----------------------------
// template <ca_encoding_t encoding>
// inline bool
// ca_isalpha(ca_char4_t c);
// ----------------------------

template <>
inline bool
ca_isalpha<ca_encoding_t::CA_ENCODING_ASCII>(const ca_char4_t c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

template <>
inline bool
ca_isalpha<ca_encoding_t::CA_ENCODING_UTF8>(const ca_char4_t c) {
    return Py_UNICODE_ISALPHA(c);
}

template <>
inline bool
ca_isalpha<ca_encoding_t::CA_ENCODING_UTF32>(const ca_char4_t c) {
    return Py_UNICODE_ISALPHA(c);
}

template <ca_encoding_t encoding>
inline bool
ca_isalpha(const ca_char4_t c) {
    return false;
}

// ----------------------------
// template <ca_encoding_t encoding>
// inline bool
// ca_isdigit(ca_char4_t c);
// ----------------------------

template <>
inline bool
ca_isdigit<ca_encoding_t::CA_ENCODING_ASCII>(const ca_char4_t c) {
    return (c >= '0' && c <= '9');
}

template <>
inline bool
ca_isdigit<ca_encoding_t::CA_ENCODING_UTF8>(const ca_char4_t c) {
    return Py_UNICODE_ISDIGIT(c);
}

template <>
inline bool
ca_isdigit<ca_encoding_t::CA_ENCODING_UTF32>(const ca_char4_t c) {
    return Py_UNICODE_ISDIGIT(c);
}

template <ca_encoding_t encoding>
inline bool
ca_isdigit(const ca_char4_t c) {
    return false;
}

// ----------------------------
// template <ca_encoding_t encoding>
// inline bool
// ca_isspace(ca_char4_t c);
// ----------------------------

template <>
inline bool
ca_isspace<ca_encoding_t::CA_ENCODING_ASCII>(const ca_char4_t c) {
    return c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\t' || c == '\v';
}

template <>
inline bool
ca_isspace<ca_encoding_t::CA_ENCODING_UTF8>(const ca_char4_t c) {
    return Py_UNICODE_ISSPACE(c);
}

template <>
inline bool
ca_isspace<ca_encoding_t::CA_ENCODING_UTF32>(const ca_char4_t c) {
    return Py_UNICODE_ISSPACE(c);
}

template <ca_encoding_t encoding>
inline bool
ca_isspace(const ca_char4_t c) {
    return false;
}

// ----------------------------
// template <ca_encoding_t encoding>
// inline bool
// ca_isalnum(ca_char4_t c);
// ----------------------------

template <>
inline bool
ca_isalnum<ca_encoding_t::CA_ENCODING_ASCII>(const ca_char4_t c) {
    return ca_isalpha<ca_encoding_t::CA_ENCODING_ASCII>(c) || ca_isdigit<ca_encoding_t::CA_ENCODING_UTF8>(c);
}

template <>
inline bool
ca_isalnum<ca_encoding_t::CA_ENCODING_UTF8>(const ca_char4_t c) {
    return Py_UNICODE_ISALNUM(c);
}

template <>
inline bool
ca_isalnum<ca_encoding_t::CA_ENCODING_UTF32>(const ca_char4_t c) {
    return Py_UNICODE_ISALNUM(c);
}

template <ca_encoding_t encoding>
inline bool
ca_isalnum(const ca_char4_t c) {
    return false;
}

// ----------------------------
// template <ca_encoding_t encoding>
// inline bool
// ca_islower(ca_char4_t c);
// ----------------------------

template <>
inline bool
ca_islower<ca_encoding_t::CA_ENCODING_ASCII>(const ca_char4_t c) {
    return c >= 'a' && c <= 'z';
}

template <>
inline bool
ca_islower<ca_encoding_t::CA_ENCODING_UTF8>(const ca_char4_t c) {
    return Py_UNICODE_ISLOWER(c);
}

template <>
inline bool
ca_islower<ca_encoding_t::CA_ENCODING_UTF32>(const ca_char4_t c) {
    return Py_UNICODE_ISLOWER(c);
}

template <ca_encoding_t encoding>
inline bool
ca_islower(const ca_char4_t c) {
    return false;
}

// ----------------------------
// template <ca_encoding_t encoding>
// inline bool
// ca_isupper(ca_char4_t c);
// ----------------------------

template <>
inline bool
ca_isupper<ca_encoding_t::CA_ENCODING_ASCII>(const ca_char4_t c) {
    return c >= 'A' && c <= 'Z';
}

template <>
inline bool
ca_isupper<ca_encoding_t::CA_ENCODING_UTF8>(const ca_char4_t c) {
    return Py_UNICODE_ISUPPER(c);
}

template <>
inline bool
ca_isupper<ca_encoding_t::CA_ENCODING_UTF32>(const ca_char4_t c) {
    return Py_UNICODE_ISUPPER(c);
}

template <ca_encoding_t encoding>
inline bool
ca_isupper(const ca_char4_t c) {
    return false;
}

// ----------------------------
// template <ca_encoding_t encoding>
// inline bool
// ca_istitle(ca_char4_t c);
// ----------------------------

template <>
inline bool
ca_istitle<ca_encoding_t::CA_ENCODING_ASCII>(const ca_char4_t c) {
    return false;
}

template <>
inline bool
ca_istitle<ca_encoding_t::CA_ENCODING_UTF8>(const ca_char4_t c) {
    return Py_UNICODE_ISTITLE(c);
}

template <>
inline bool
ca_istitle<ca_encoding_t::CA_ENCODING_UTF32>(const ca_char4_t c) {
    return Py_UNICODE_ISTITLE(c);
}

template <ca_encoding_t encoding>
inline bool
ca_istitle(const ca_char4_t c) {
    return false;
}

// ----------------------------
// template <ca_encoding_t encoding>
// inline bool
// ca_isnumeric(ca_char4_t c);
// ----------------------------

template <>
inline bool
ca_isnumeric<ca_encoding_t::CA_ENCODING_ASCII>(const ca_char4_t c) {
    return Py_UNICODE_ISNUMERIC(c);
}

template <>
inline bool
ca_isnumeric<ca_encoding_t::CA_ENCODING_UTF8>(const ca_char4_t c) {
    return Py_UNICODE_ISNUMERIC(c);
}

template <>
inline bool
ca_isnumeric<ca_encoding_t::CA_ENCODING_UTF32>(const ca_char4_t c) {
    return Py_UNICODE_ISNUMERIC(c);
}

template <ca_encoding_t encoding>
inline bool
ca_isnumeric(const ca_char4_t c) {
    return false;
}

// ----------------------------
// template <ca_encoding_t encoding>
// inline bool
// ca_isdecimal(ca_char4_t c);
// ----------------------------

template <>
inline bool
ca_isdecimal<ca_encoding_t::CA_ENCODING_ASCII>(const ca_char4_t c) {
    return Py_UNICODE_ISDECIMAL(c);
}

template <>
inline bool
ca_isdecimal<ca_encoding_t::CA_ENCODING_UTF8>(const ca_char4_t c) {
    return Py_UNICODE_ISDECIMAL(c);
}

template <>
inline bool
ca_isdecimal<ca_encoding_t::CA_ENCODING_UTF32>(const ca_char4_t c) {
    return Py_UNICODE_ISDECIMAL(c);
}

template <ca_encoding_t encoding>
inline bool
ca_isdecimal(const ca_char4_t c) {
    return false;
}

// ----------------------------
// Misc Function
// ----------------------------

template <ca_encoding_t encoding, ca_char_check_types check_type>
inline bool
ca_char_check(ca_char_t c) {
    static_assert(check_type == ca_char_check_types::CA_ISALPHA ||
                  check_type == ca_char_check_types::CA_ISDIGIT ||
                  check_type == ca_char_check_types::CA_ISSPACE ||
                  check_type == ca_char_check_types::CA_ISALNUM ||
                  check_type == ca_char_check_types::CA_ISLOWER ||
                  check_type == ca_char_check_types::CA_ISUPPER ||
                  check_type == ca_char_check_types::CA_ISTITLE ||
                  check_type == ca_char_check_types::CA_ISNUMERIC ||
                  check_type == ca_char_check_types::CA_ISDECIMAL,
                  "Invalid ca_char_check_types value "
                  "in ca_char_check.");

    switch (check_type) {
        case ca_char_check_types::CA_ISALPHA:
            return ca_isalpha<encoding>(c);
        case ca_char_check_types::CA_ISDIGIT:
            return ca_isdigit<encoding>(c);
        case ca_char_check_types::CA_ISSPACE:
            return ca_isspace<encoding>(c);
        case ca_char_check_types::CA_ISALNUM:
            return ca_isalnum<encoding>(c);
        case ca_char_check_types::CA_ISLOWER:
            return ca_islower<encoding>(c);
        case ca_char_check_types::CA_ISUPPER:
            return ca_isupper<encoding>(c);
        case ca_char_check_types::CA_ISTITLE:
            return ca_istitle<encoding>(c);
        case ca_char_check_types::CA_ISNUMERIC:
            return ca_isnumeric<encoding>(c);
        case ca_char_check_types::CA_ISDECIMAL:
            return ca_isdecimal<encoding>(c);
    }

    return false;
}
}