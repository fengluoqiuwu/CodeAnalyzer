// ================================
// CodeAnalyzer - source/c_src/common/private/ca_platform/file/win32/file_temp.cpp
//
// @file
// @brief Implementations of "file_temp.h" for win32.
// ================================

#include <cstring>
#include <windows.h>
#include "file/file_temp.h"
#include "file/file_defs.h"
#include "internal/win32_translater.h"

namespace ca::ca_file {

namespace internal {

static ca_file_result ca_win32_generate_temp_path(char* buffer, const size_t buffer_size) {
    const DWORD len = GetTempPathA(buffer_size, buffer);
    if (len == 0) {
        return ca_translate_win32_error(GetLastError());
    } else if (len >= buffer_size) {
        return ca_file_result::FILE_ERROR_OUT_OF_MEMORY;
    }
    return ca_file_result::FILE_OK;
}

}

ca_file_result ca_file_create_temp_file(char* out_path, const size_t out_size) {
    if (!out_path || out_size == 0) {
        return ca_file_result::FILE_ERROR_INVALID_PARAMETER;
    }

    char temp_dir[MAX_PATH];
    const ca_file_result res = internal::ca_win32_generate_temp_path(temp_dir, sizeof(temp_dir));
    if (res != ca_file_result::FILE_OK) {
        return res;
    }

    char temp_file[MAX_PATH];
    if (!GetTempFileNameA(temp_dir, "CA", 0, temp_file)) {
        return internal::ca_translate_win32_error(GetLastError());
    }

    if (strlen(temp_file) + 1 > out_size) {
        DeleteFileA(temp_file);
        return ca_file_result::FILE_ERROR_OUT_OF_MEMORY;
    }

    strcpy(out_path, temp_file);
    return ca_file_result::FILE_OK;
}

ca_file_result ca_file_create_temp_directory(char* out_path, const size_t out_size) {
    if (!out_path || out_size == 0) {
        return ca_file_result::FILE_ERROR_INVALID_PARAMETER;
    }

    char temp_dir[MAX_PATH];
    const ca_file_result res = internal::ca_win32_generate_temp_path(temp_dir, sizeof(temp_dir));
    if (res != ca_file_result::FILE_OK) {
        return res;
    }

    char temp_file[MAX_PATH];
    if (!GetTempFileNameA(temp_dir, "CA", 0, temp_file)) {
        return internal::ca_translate_win32_error(GetLastError());
    }

    DeleteFileA(temp_file);

    if (!CreateDirectoryA(temp_file, nullptr)) {
        return internal::ca_translate_win32_error(GetLastError());
    }

    if (strlen(temp_file) + 1 > out_size) {
        RemoveDirectoryA(temp_file);
        return ca_file_result::FILE_ERROR_OUT_OF_MEMORY;
    }

    strcpy(out_path, temp_file);
    return ca_file_result::FILE_OK;
}

}