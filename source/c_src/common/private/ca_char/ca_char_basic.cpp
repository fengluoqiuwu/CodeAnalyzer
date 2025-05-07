// ================================
// CodeAnalyzer - source/c_src/common/private/ca_char/common/ca_char_basic.cpp
//
// @file
// @brief Implement type handling functions in "ca_char_basic.h".
// ================================

#include "ca_char_basic.h"

template<ca::ca_char::ca_encoding_t encoding>
ca_int8_t
ca::ca_char::ca_get_bytes(const ca_char_t *c, ca_char_status *status) {
}

template<ca::ca_char::ca_encoding_t encoding>
void
ca::ca_char::ca_getchar(const ca_char_t *code, ca_char_type_t<encoding> *buf, ca_int8_t *size, ca_char_status *status) {
}

template<ca::ca_char::ca_encoding_t encoding>
void
ca::ca_char::ca_getcode(const ca_char_type_t<encoding> *c, ca_char_t *buf, ca_int8_t *size, ca_char_status *status) {
}

template<ca::ca_char::ca_encoding_t encoding>
bool
ca::ca_char::is_valid(ca_char_type_t<encoding> *c) {
}

template<ca::ca_char::ca_encoding_t encoding>
bool
ca::ca_char::is_valid(ca_char_t *code) {
}
