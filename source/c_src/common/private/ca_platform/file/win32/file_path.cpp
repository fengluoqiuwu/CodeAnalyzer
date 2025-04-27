// ================================
// CodeAnalyzer - source/c_src/common/private/ca_platform/file/win32/file_path.cpp
//
// @file
// @brief Implementations of "file_path.h" for win32.
// ================================

#include <cstring>
#include <windows.h>
#include "file/file_path.h"
#include "file/file_defs.h"
#include "internal/win32_translater.h"

namespace ca::ca_file {

char ca_file_get_path_separator() {
    return '\\';
}

ca_file_result ca_file_join_path(char* out_path, const ca_size_t out_size, const char* path1, const char* path2) {
    if (!out_path || !path1 || !path2 || out_size == 0) {
        return ca_file_result::FILE_ERROR_INVALID_PARAMETER;
    }

    const ca_size_t len1 = std::strlen(path1);
    const ca_size_t len2 = std::strlen(path2);
    if (len1 + len2 + 2 > out_size) { // +1 for separator, +1 for '\0'
        return ca_file_result::FILE_ERROR_OUT_OF_MEMORY;
    }

    std::strcpy(out_path, path1);
    if (len1 > 0 && path1[len1 - 1] != '\\') {
        strcat(out_path, "\\");
    }
    std::strcat(out_path, path2);

    return ca_file_result::FILE_OK;
}

ca_file_result ca_file_get_absolute_path(const char* relative, char* out_path, const ca_size_t out_size) {
    if (!relative || !out_path || out_size == 0) {
        return ca_file_result::FILE_ERROR_INVALID_PARAMETER;
    }

    const DWORD result = GetFullPathNameA(relative, out_size, out_path, nullptr);
    if (result == 0) {
        return internal::ca_translate_win32_error(GetLastError());
    } else if (result >= out_size) {
        return ca_file_result::FILE_ERROR_OUT_OF_MEMORY;
    }

    return ca_file_result::FILE_OK;
}

ca_file_result ca_file_normalize_path(const char* input, char* out, const ca_size_t out_size) {
    return ca_file_get_absolute_path(input, out, out_size);
}

}