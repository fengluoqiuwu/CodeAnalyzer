// ================================
// CodeAnalyzer - source/c_src/common/public/ca_char/ca_char_basic.h
//
// @file
// @brief Defines functions for character basic handling.
// ================================

#ifndef CA_CHAR_BASIC_H
#define CA_CHAR_BASIC_H

#include "ca_char_defs.h"
#include "ca_char_errors.h"

namespace ca::ca_char {
// TODO

template <ca_encoding_t encoding>
inline ca_int8_t
ca_get_bytes(const ca_char_t *c, ca_char_status* status = nullptr);

template <ca_encoding_t encoding>
inline void
ca_getchar(
        const ca_char_t *code, ca_char_type_t<encoding> *buf,
        ca_int8_t *size, ca_char_status* status = nullptr
);

template <ca_encoding_t encoding>
inline void
ca_getcode(
        const ca_char_type_t<encoding> *c, ca_char_t *buf,
        ca_int8_t *size, ca_char_status* status = nullptr
);

template <ca_encoding_t encoding>
inline bool
is_valid(ca_char_type_t<encoding> *c);

template <ca_encoding_t encoding>
inline bool
is_valid(ca_char_t *code);

// ================================
// Comments
// ================================

// NOLINTBEGIN(clang-diagnostic-redundant-decl)

template <ca_encoding_t encoding>
inline ca_int8_t
ca_get_bytes(const ca_char_t *c, ca_char_status* status);

template <ca_encoding_t encoding>
inline void
ca_getchar(
        const ca_char_t *code, ca_char_type_t<encoding> *buf,
        ca_int8_t *size, ca_char_status* status
);

template <ca_encoding_t encoding>
inline void
ca_getcode(
        const ca_char_type_t<encoding> *c, ca_char_t *buf,
        ca_int8_t *size, ca_char_status* status
);

template <ca_encoding_t encoding>
inline bool
is_valid(ca_char_type_t<encoding> *c);

template <ca_encoding_t encoding>
inline bool
is_valid(ca_char_t *code);

// NOLINTEND(clang-diagnostic-redundant-decl)

} // namespace ca::ca_char

#endif //CA_CHAR_BASIC_H
