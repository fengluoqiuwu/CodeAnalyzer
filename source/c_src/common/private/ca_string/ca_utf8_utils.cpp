// ================================
// CodeAnalyzer - source/c_src/common/public/ca_string/ca_char.cpp
//
// @file
// @brief Implementation of UTF-8 utility functions, modified from NumPy utf8_utils.cpp.
// ================================

#include <Python.h>
#include <cassert>

#include "ca_utf8_utils.h"

namespace ca::ca_string::utf8 {

namespace {
/*******************************************************************************/
// Everything until the closing /***/ block below is a copy of the
// Bjoern Hoerhmann DFA UTF-8 validator
// License: MIT
// Copyright (c) 2008-2009 Bjoern Hoehrmann <bjoern@hoehrmann.de>
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//
// See http://bjoern.hoehrmann.de/utf-8/decoder/dfa/ for details.
//
// in principle could use something like simdutf to accelerate this

#define UTF8_ACCEPT 0
#define UTF8_REJECT 1

const ca_char_t utf8d[] = {
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 00..1f
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 20..3f
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 40..5f
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 60..7f
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9, // 80..9f
  7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7, // a0..bf
  8,8,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, // c0..df
  0xa,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x4,0x3,0x3, // e0..ef
  0xb,0x6,0x6,0x6,0x5,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8, // f0..ff
  0x0,0x1,0x2,0x3,0x5,0x8,0x7,0x1,0x1,0x1,0x4,0x6,0x1,0x1,0x1,0x1, // s0..s0
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,0,1,0,1,1,1,1,1,1, // s1..s2
  1,2,1,1,1,1,1,2,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1, // s3..s4
  1,2,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,3,1,3,1,1,1,1,1,1, // s5..s6
  1,3,1,1,1,1,1,3,1,3,1,1,1,1,1,1,1,3,1,1,1,1,1,1,1,1,1,1,1,1,1,1, // s7..s8
};

ca_uint8_t inline
utf8_decode(ca_uint8_t* state, ca_char4_t* codep, const ca_char4_t byte) {
    const ca_char4_t type = utf8d[byte];

    *codep = (*state != UTF8_ACCEPT) ?
            (byte & 0x3fu) | (*codep << 6) :
            (0xff >> type) & (byte);

    *state = utf8d[256 + *state*16 + type];
    return *state;
}

/*******************************************************************************/
}

// ----------------------------
// Encode & Decode functions
// ----------------------------

int
utf8_char_to_ucs4_code_without_check(const ca_char_t *c, ca_char4_t *code) {
    assert(c != nullptr);
    assert(code != nullptr);

    if (c[0] <= 0x7F) {
        // 0zzzzzzz -> 0zzzzzzz
        *code = static_cast<ca_char4_t>(c[0]);
        return 1;
    }
    else if (c[0] <= 0xDF) {
        // 110yyyyy 10zzzzzz -> 00000yyy yyzzzzzz
        *code = static_cast<ca_char4_t>(((c[0] << 6) + c[1]) - ((0xC0 << 6) + 0x80));
        return 2;
    }
    else if (c[0] <= 0xEF) {
        // 1110xxxx 10yyyyyy 10zzzzzz -> xxxxyyyy yyzzzzzz
        *code = static_cast<ca_char4_t>(((c[0] << 12) + (c[1] << 6) + c[2]) -
                                        ((0xE0 << 12) + (0x80 << 6) + 0x80));
        return 3;
    }
    else {
        // 11110www 10xxxxxx 10yyyyyy 10zzzzzz -> 000wwwxx xxxxyyyy yyzzzzzz
        *code = static_cast<ca_char4_t>(((c[0] << 18) + (c[1] << 12) + (c[2] << 6) + c[3]) -
                                        ((0xF0 << 18) + (0x80 << 12) + (0x80 << 6) + 0x80));
        return 4;
    }
}

int
ucs4_code_to_utf8_char_without_check(const ca_char4_t code, ca_char_t *c) {
    assert(c != nullptr);

    if (code <= 0x7F) {
        // 0zzzzzzz -> 0zzzzzzz
        c[0] = static_cast<ca_char_t>(code);
        return 1;
    }
    else if (code <= 0x07FF) {
        // 00000yyy yyzzzzzz -> 110yyyyy 10zzzzzz
        c[0] = (0xC0 | (code >> 6));
        c[1] = (0x80 | (code & 0x3F));
        return 2;
    }
    else if (code <= 0xFFFF) {
        // xxxxyyyy yyzzzzzz -> 110yyyyy 10zzzzzz
        c[0] = (0xe0 | (code >> 12));
        c[1] = (0x80 | ((code >> 6) & 0x3f));
        c[2] = (0x80 | (code & 0x3f));
        return 3;
    }
    else {
        // 00wwwxx xxxxyyyy yyzzzzzz -> 11110www 10xxxxxx 10yyyyyy 10zzzzzz
        c[0] = (0xf0 | (code >> 18));
        c[1] = (0x80 | ((code >> 12) & 0x3f));
        c[2] = (0x80 | ((code >> 6) & 0x3f));
        c[3] = (0x80 | (code & 0x3f));
        return 4;
    }
}

// ----------------------------
// Bytes of character getting functions
// ----------------------------

int
num_utf8_bytes_for_utf8_character_without_check(const ca_char_t *c) {
    assert(c != nullptr);

    if (c[0] <= 0x7F) {
        return 1;
    }
    else if (c[0] <= 0xDF) {
        return 2;
    }
    else if (c[0] <= 0xEF) {
        return 3;
    }
    return 4;
}

int
num_utf8_bytes_for_codepoint(const ca_char4_t code) {
    if (code <= 0x7F) {
        return 1;
    }
    else if (code <= 0x07FF) {
        return 2;
    }
    else if (code <= 0xFFFF) {
        if ((code >= 0xD800) && (code <= 0xDFFF)) {
            // surrogates are invalid UCS4 code points
            return -1;
        }
        return 3;
    }
    else if (code <= 0x10FFFF) {
        return 4;
    }
    else {
        // codepoint is outside the valid unicode range
        return -1;
    }
}

// ----------------------------
// Buffer Number of codepoints calculation functions
// ----------------------------

void
num_codepoints_for_utf8_bytes_without_check(
        const ca_char_t *buf, const ca_size_t max_bytes,
        ca_size_t *num_codepoints) {
    ca_size_t bytes_consumed = 0;
    ca_size_t codepoints_count = 0;

    while (bytes_consumed < max_bytes) {
        const int num_bytes = num_utf8_bytes_for_utf8_character_without_check(buf);
        codepoints_count += 1;
        bytes_consumed += num_bytes;
        buf += num_bytes;
    }

    *num_codepoints = codepoints_count;
}

int
num_codepoints_for_utf8_bytes(
        const ca_char_t *buf, ca_size_t max_bytes,
        ca_size_t *num_codepoints) {
    ca_char4_t codepoint;

    ca_uint8_t state = UTF8_ACCEPT;
    *num_codepoints = 0;

    // ignore trailing nulls
    while (max_bytes > 0 && buf[max_bytes - 1] == 0) {
        max_bytes--;
    }

    if (max_bytes == 0) {
        return 0;
    }

    for (size_t num_bytes = 0; num_bytes < max_bytes; ++buf)
    {
        utf8_decode(&state, &codepoint, *buf);
        if (state == UTF8_REJECT)
        {
            return -1;
        }
        else if(state == UTF8_ACCEPT)
        {
            *num_codepoints += 1;
        }
        num_bytes += 1;
    }

    return state == UTF8_ACCEPT ? 0 : -1;
}

// ----------------------------
// Buffer Size calculation functions
// ----------------------------

int
utf8_buffer_size(
        const ca_char_t *buf, ca_size_t max_bytes,
        ca_size_t *utf8_bytes) {
    ca_char4_t codepoint;

    ca_uint8_t state = UTF8_ACCEPT;
    ca_ssize_t encoded_size_in_bytes = 0;
    *utf8_bytes = 0;

    // ignore trailing nulls
    while (max_bytes > 0 && buf[max_bytes - 1] == 0) {
        max_bytes--;
    }

    if (max_bytes == 0) {
        return 0;
    }

    for (ca_size_t num_bytes = 0; num_bytes < max_bytes; ++buf)
    {
        utf8_decode(&state, &codepoint, *buf);
        if (state == UTF8_REJECT)
        {
            return -1;
        }
        else if(state == UTF8_ACCEPT)
        {
            encoded_size_in_bytes += num_utf8_bytes_for_codepoint(codepoint);
        }
        num_bytes += 1;
    }

    if (state != UTF8_ACCEPT) {
        return -1;
    }

    *utf8_bytes = encoded_size_in_bytes;
    return 0;
}

int
utf8_size_of_utf32_buffer_encode(
        const ca_char4_t *buf_usc4, const ca_size_t buf_length,
        ca_size_t *num_codepoints, ca_size_t *utf8_bytes) {
    // ucs4_len is now the number of buf_usc4 codepoints that aren't trailing nulls.
    ca_size_t ucs4_len = buf_length;

    while (ucs4_len > 0 && buf_usc4[ucs4_len - 1] == 0) {
        ucs4_len--;
    }

    ca_size_t num_bytes = 0;

    for (ca_size_t i = 0; i < ucs4_len; i++) {
        const ca_char4_t code = buf_usc4[i];
        const int codepoint_bytes = num_utf8_bytes_for_codepoint(code);
        if (codepoint_bytes == -1) {
            return -1;
        }
        num_bytes += codepoint_bytes;
    }

    *num_codepoints = ucs4_len;
    *utf8_bytes = num_bytes;

    return 0;
}

// ----------------------------
// Location finding functions
// ----------------------------

void
find_previous_utf8_character(
        const ca_char_t *current, ca_size_t nchar,
        ca_size_t **previous_loc) {
    assert(current != nullptr);
    assert(previous_loc != nullptr);

    *previous_loc = nullptr;

    while (nchar > 0) {
        do
        {
            // this assumes well-formed UTF-8 and does not check if we go
            // before the start of the string
            previous_loc--;
            // the first byte of a UTF8 character either has
            // the topmost bit clear or has both topmost bits set
        } while ((*current & 0xC0) == 0x80);
        nchar--;
    }
}

int
find_start_end_locs(
        ca_char_t *buf, const ca_size_t buffer_size,
        const ca_size_t start_index, const ca_size_t end_index,
        ca_char_t **start_loc, ca_char_t **end_loc) {
    assert(buf != nullptr);
    assert(start_loc != nullptr);
    assert(end_loc != nullptr);

    *start_loc = nullptr;
    *end_loc = nullptr;

    ca_size_t bytes_consumed = 0;
    ca_size_t num_codepoints = 0;

    int match_count = 0;    // Count of locations found

    if (start_index == 0) {
        *start_loc = buf;
        match_count += 1;
    }
    if (end_index == 0) {
        *end_loc = buf;
        match_count += 1;
    }

    if (match_count >= 2) {
        return 0;
    }

    while (bytes_consumed < buffer_size && num_codepoints < end_index) {
        const ca_size_t num_bytes = num_utf8_bytes_for_utf8_character_without_check(buf);

        num_codepoints += 1;
        bytes_consumed += num_bytes;
        buf += num_bytes;

        if (start_index == num_codepoints) {
            *start_loc = buf;
            match_count += 1;
            if (match_count >= 2) {
                return 0;
            }
        }
        if (end_index == num_codepoints) {
            *end_loc = buf;
            match_count += 1;
            if (match_count >= 2) {
                return 0;
            }
        }
    }

    return -1;
}

}
