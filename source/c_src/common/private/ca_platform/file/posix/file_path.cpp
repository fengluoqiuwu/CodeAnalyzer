// ================================
// CodeAnalyzer - source/c_src/common/private/ca_platform/file/posix/file_path.cpp
//
// @file
// @brief Implementations of "file_path.h" for posix.
// ================================

#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include "file/file_path.h"
#include "file/file_defs.h"
#include "internal/posix_translater.h"

namespace ca::ca_file {


char ca_file_get_path_separator() {
    return '/';
}

ca_file_result ca_file_join_path(char* out_path, ca_size_t out_size, const char* path1, const char* path2) {
    if (!out_path || !path1 || !path2 || out_size == 0) {
        return ca_file_result::FILE_ERROR_INVALID_PARAMETER;
    }

    size_t len1 = strlen(path1);
    size_t len2 = strlen(path2);
    if (len1 + len2 + 2 > out_size) { // +1 for separator, +1 for '\0'
        return ca_file_result::FILE_ERROR_OUT_OF_MEMORY;
    }

    strcpy(out_path, path1);
    if (len1 > 0 && path1[len1 - 1] != '/') {
        strcat(out_path, "/");
    }
    strcat(out_path, path2);

    return ca_file_result::FILE_OK;
}

ca_file_result ca_file_get_absolute_path(const char* relative, char* out_path, ca_size_t out_size) {
    if (!relative || !out_path || out_size == 0) {
        return ca_file_result::FILE_ERROR_INVALID_PARAMETER;
    }

    char temp[PATH_MAX];
    if (!realpath(relative, temp)) {
        return internal::ca_translate_errno(errno);
    }

    if (strlen(temp) + 1 > out_size) {
        return ca_file_result::FILE_ERROR_OUT_OF_MEMORY;
    }

    strcpy(out_path, temp);
    return ca_file_result::FILE_OK;
}

ca_file_result ca_file_normalize_path(const char* input, char* out, ca_size_t out_size) {
    return ca_file_get_absolute_path(input, out, out_size);
    // POSIX realpath已经做了完整的标准化
}

}