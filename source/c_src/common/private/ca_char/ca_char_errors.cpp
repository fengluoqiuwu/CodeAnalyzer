// ================================
// CodeAnalyzer - source/c_src/common/private/ca_char/common/ca_char_errors.cpp
//
// @file
// @brief Implement error handling functions in "ca_char_errors.h".
// ================================

#include "ca_char_errors.h"

namespace ca::ca_char {
constexpr const char *
get_status_string(const ca_char_status status) {
    switch (status) {
        case ca_char_status::SUCCESS:
            return "Success";
        case ca_char_status::INVALID_INPUT:
            return "Invalid input";
        case ca_char_status::INVALID_ENCODING:
            return "Invalid encoding";
        case ca_char_status::INVALID_CODEPOINT:
            return "Invalid codepoint";
        case ca_char_status::BUFFER_OVERFLOW:
            return "Buffer overflow";
        case ca_char_status::MEMORY_ALLOCATION_FAILURE:
            return "Memory allocation failure";
        case ca_char_status::UNKNOWN_ERROR:
            return "Unknown error";
        default:
            return "Unknown status code";
    }
}
} // namespace ca::ca_char