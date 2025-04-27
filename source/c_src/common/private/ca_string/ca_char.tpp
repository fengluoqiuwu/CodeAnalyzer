// ================================
// CodeAnalyzer - source/c_src/common/public/ca_string/ca_char.tpp
//
// @file
// @brief Source file implementing functions for character types and encoding/decoding
//        operations, including utility functions for character classification.
// ================================
#pragma once

extern "C" {
#define UTF8PROC_STATIC
#include "../../../../third_party/utf8proc/utf8proc.h"
}

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
    return utf8::num_utf8_bytes_for_utf8_character_without_check(c);
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
    const utf8proc_category_t cat = utf8proc_category(static_cast<utf8proc_int32_t>(c));
    return (cat == UTF8PROC_CATEGORY_LU ||
            cat == UTF8PROC_CATEGORY_LL ||
            cat == UTF8PROC_CATEGORY_LT ||
            cat == UTF8PROC_CATEGORY_LM ||
            cat == UTF8PROC_CATEGORY_LO);
}

template <>
inline bool
ca_isalpha<ca_encoding_t::CA_ENCODING_UTF32>(const ca_char4_t c) {
    const utf8proc_category_t cat = utf8proc_category(static_cast<utf8proc_int32_t>(c));
    return (cat == UTF8PROC_CATEGORY_LU ||
            cat == UTF8PROC_CATEGORY_LL ||
            cat == UTF8PROC_CATEGORY_LT ||
            cat == UTF8PROC_CATEGORY_LM ||
            cat == UTF8PROC_CATEGORY_LO);
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
    if (utf8proc_category(static_cast<utf8proc_int32_t>(c)) == UTF8PROC_CATEGORY_ND) {
        return true;
    }

    switch (c) {
        case 0x00B2: // ²
        case 0x00B3: // ³
        case 0x00B9: // ¹
        case 0x1369: case 0x136A: case 0x136B: case 0x136C: case 0x136D:
        case 0x136E: case 0x136F: case 0x1370: case 0x1371: // Ethiopic digits 1-9
        case 0x2460: case 0x2461: case 0x2462: case 0x2463: case 0x2464:
        case 0x2465: case 0x2466: case 0x2467: case 0x2468: // Circled numbers 1-9
        case 0x2474: case 0x2475: case 0x2476: case 0x2477: case 0x2478:
        case 0x2479: case 0x247A: case 0x247B: case 0x247C: // Parenthesized digits 1-9
        case 0x2488: case 0x2489: case 0x248A: case 0x248B: case 0x248C:
        case 0x248D: case 0x248E: case 0x248F: case 0x2490: // Fullwidth circled 1-9
            return true;
        default:
            return false;
    }
}

template <>
inline bool
ca_isdigit<ca_encoding_t::CA_ENCODING_UTF32>(const ca_char4_t c) {
    if (utf8proc_category(static_cast<utf8proc_int32_t>(c)) == UTF8PROC_CATEGORY_ND) {
        return true;
    }

    switch (c) {
        case 0x00B2: // ²
        case 0x00B3: // ³
        case 0x00B9: // ¹
        case 0x1369: case 0x136A: case 0x136B: case 0x136C: case 0x136D:
        case 0x136E: case 0x136F: case 0x1370: case 0x1371: // Ethiopic digits 1-9
        case 0x2460: case 0x2461: case 0x2462: case 0x2463: case 0x2464:
        case 0x2465: case 0x2466: case 0x2467: case 0x2468: // Circled numbers 1-9
        case 0x2474: case 0x2475: case 0x2476: case 0x2477: case 0x2478:
        case 0x2479: case 0x247A: case 0x247B: case 0x247C: // Parenthesized digits 1-9
        case 0x2488: case 0x2489: case 0x248A: case 0x248B: case 0x248C:
        case 0x248D: case 0x248E: case 0x248F: case 0x2490: // Fullwidth circled 1-9
            return true;
        default:
            return false;
    }
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
    // ASCII / Latin-1 whitespace
    if (c == 0x0009 || c == 0x000A || c == 0x000B || c == 0x000C ||
        c == 0x000D || c == 0x0020 || c == 0x0085 || c == 0x00A0)
        return true;

    // Unicode white space blocks
    if (c == 0x1680 || c == 0x180E || c == 0x2000 || c == 0x2001 ||
        c == 0x2002 || c == 0x2003 || c == 0x2004 || c == 0x2005 ||
        c == 0x2006 || c == 0x2007 || c == 0x2008 || c == 0x2009 ||
        c == 0x200A || c == 0x2028 || c == 0x2029 || c == 0x202F ||
        c == 0x205F || c == 0x3000)
        return true;

    return false;
}

template <>
inline bool
ca_isspace<ca_encoding_t::CA_ENCODING_UTF32>(const ca_char4_t c) {
    // ASCII / Latin-1 whitespace
    if (c == 0x0009 || c == 0x000A || c == 0x000B || c == 0x000C ||
        c == 0x000D || c == 0x0020 || c == 0x0085 || c == 0x00A0)
        return true;

    // Unicode white space blocks
    if (c == 0x1680 || c == 0x180E || c == 0x2000 || c == 0x2001 ||
        c == 0x2002 || c == 0x2003 || c == 0x2004 || c == 0x2005 ||
        c == 0x2006 || c == 0x2007 || c == 0x2008 || c == 0x2009 ||
        c == 0x200A || c == 0x2028 || c == 0x2029 || c == 0x202F ||
        c == 0x205F || c == 0x3000)
        return true;

    return false;
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
    return ca_isalpha<ca_encoding_t::CA_ENCODING_UTF8>(c) || ca_isdigit<ca_encoding_t::CA_ENCODING_UTF8>(c);
}

template <>
inline bool
ca_isalnum<ca_encoding_t::CA_ENCODING_UTF32>(const ca_char4_t c) {
    return ca_isalpha<ca_encoding_t::CA_ENCODING_UTF32>(c) || ca_isdigit<ca_encoding_t::CA_ENCODING_UTF8>(c);
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
    return utf8proc_category(static_cast<utf8proc_int32_t>(c)) == UTF8PROC_CATEGORY_LL;
}

template <>
inline bool
ca_islower<ca_encoding_t::CA_ENCODING_UTF32>(const ca_char4_t c) {
    return utf8proc_category(static_cast<utf8proc_int32_t>(c)) == UTF8PROC_CATEGORY_LL;
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
    return utf8proc_category(static_cast<utf8proc_int32_t>(c)) == UTF8PROC_CATEGORY_LU;
}

template <>
inline bool
ca_isupper<ca_encoding_t::CA_ENCODING_UTF32>(const ca_char4_t c) {
    return utf8proc_category(static_cast<utf8proc_int32_t>(c)) == UTF8PROC_CATEGORY_LU;
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
    return utf8proc_category(static_cast<utf8proc_int32_t>(c)) == UTF8PROC_CATEGORY_LT;
}

template <>
inline bool
ca_istitle<ca_encoding_t::CA_ENCODING_UTF32>(const ca_char4_t c) {
    return utf8proc_category(static_cast<utf8proc_int32_t>(c)) == UTF8PROC_CATEGORY_LT;
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
    utf8proc_category_t cat = utf8proc_category(static_cast<utf8proc_int32_t>(c));
    return cat == UTF8PROC_CATEGORY_ND ||
           cat == UTF8PROC_CATEGORY_NL ||
           cat == UTF8PROC_CATEGORY_NO;
}

template <>
inline bool
ca_isnumeric<ca_encoding_t::CA_ENCODING_UTF8>(const ca_char4_t c) {
    utf8proc_category_t cat = utf8proc_category(static_cast<utf8proc_int32_t>(c));
    return cat == UTF8PROC_CATEGORY_ND ||
           cat == UTF8PROC_CATEGORY_NL ||
           cat == UTF8PROC_CATEGORY_NO;
}

template <>
inline bool
ca_isnumeric<ca_encoding_t::CA_ENCODING_UTF32>(const ca_char4_t c) {
    utf8proc_category_t cat = utf8proc_category(static_cast<utf8proc_int32_t>(c));
    return cat == UTF8PROC_CATEGORY_ND ||
           cat == UTF8PROC_CATEGORY_NL ||
           cat == UTF8PROC_CATEGORY_NO;
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
    return utf8proc_category(static_cast<utf8proc_int32_t>(c)) == UTF8PROC_CATEGORY_ND;
}

template <>
inline bool
ca_isdecimal<ca_encoding_t::CA_ENCODING_UTF8>(const ca_char4_t c) {
    return utf8proc_category(static_cast<utf8proc_int32_t>(c)) == UTF8PROC_CATEGORY_ND;
}

template <>
inline bool
ca_isdecimal<ca_encoding_t::CA_ENCODING_UTF32>(const ca_char4_t c) {
    return utf8proc_category(static_cast<utf8proc_int32_t>(c)) == UTF8PROC_CATEGORY_ND;
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
ca_char_check(const ca_char_t c) {
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