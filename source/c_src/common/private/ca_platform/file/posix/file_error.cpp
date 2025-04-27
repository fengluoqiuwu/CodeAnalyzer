// ================================
// CodeAnalyzer - source/c_src/common/private/ca_platform/file/posix/file_error.cpp
//
// @file
// @brief Implementations of "file_error.h" for posix.
// ================================

#include <string.h>
#include <errno.h>
#include "file/file_error.h"

namespace ca::ca_file {

const char* ca_file_get_last_error_message() {
    return strerror(errno);
}

int ca_file_get_last_error_code() {
    return errno;
}

}