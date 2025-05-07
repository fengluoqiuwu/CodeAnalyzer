// ================================
// CodeAnalyzer - source/c_src/common/private/ca_char/common/ca_char_defs.cpp
//
// @file
// @brief Implement type handling functions in "ca_char_defs.h".
// ================================

#include "ca_char_defs.h"

namespace ca::ca_char {

template<>
struct ca_char_type_map<ca_encoding_t::ENCODING_ASCII> {
    using type = ca_ascii_t;
};

template<>
struct ca_char_type_map<ca_encoding_t::ENCODING_UTF8> {
    using type = ca_utf8_t;
};

template<>
struct ca_char_type_map<ca_encoding_t::ENCODING_UTF16> {
    using type = ca_utf16_t;
};

template<>
struct ca_char_type_map<ca_encoding_t::ENCODING_UTF32> {
    using type = ca_utf32_t;
};

template<>
struct ca_char_type_map<ca_encoding_t::ENCODING_GBK> {
    using type = ca_gbk_t;
};

template<>
struct ca_char_type_map<ca_encoding_t::ENCODING_LATIN1> {
    using type = ca_latin1_t;
};

} // namespace ca::ca_char
