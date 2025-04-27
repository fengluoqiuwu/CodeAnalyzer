// ================================
// CodeAnalyzer - source/c_src/common/private/ca_platform/file/win32/internal/win32_translater
//
// @file
// @brief Translator for Win32 file operations.
// ================================

#ifndef WIN32_TRANSLATER_H
#define WIN32_TRANSLATER_H

#include <windows.h>
#include "file/file_defs.h"

namespace ca::ca_file::internal {

inline DWORD
ca_translate_access(const ca_file_mode mode) {
    switch (mode) {
        case ca_file_mode::FILE_MODE_READ:
        case ca_file_mode::FILE_MODE_READ_EXISTING:
            return GENERIC_READ;

        case ca_file_mode::FILE_MODE_WRITE:
        case ca_file_mode::FILE_MODE_WRITE_EXISTING:
        case ca_file_mode::FILE_MODE_TRUNCATE_WRITE:
            return GENERIC_WRITE;

        case ca_file_mode::FILE_MODE_APPEND:
        case ca_file_mode::FILE_MODE_APPEND_EXISTING:
            return FILE_APPEND_DATA;

        case ca_file_mode::FILE_MODE_READ_WRITE:
        case ca_file_mode::FILE_MODE_READ_WRITE_EXISTING:
        case ca_file_mode::FILE_MODE_TRUNCATE_READ_WRITE:
            return GENERIC_READ | GENERIC_WRITE;

        default:
            return GENERIC_READ;
    }
}

inline DWORD
ca_translate_creation(const ca_file_mode mode) {
    switch (mode) {
        case ca_file_mode::FILE_MODE_READ:
        case ca_file_mode::FILE_MODE_READ_EXISTING:
        case ca_file_mode::FILE_MODE_WRITE_EXISTING:
        case ca_file_mode::FILE_MODE_READ_WRITE_EXISTING:
        case ca_file_mode::FILE_MODE_APPEND_EXISTING:
            return OPEN_EXISTING;

        case ca_file_mode::FILE_MODE_WRITE:
            return CREATE_ALWAYS;

        case ca_file_mode::FILE_MODE_APPEND:
        case ca_file_mode::FILE_MODE_READ_WRITE:
            return OPEN_ALWAYS;

        case ca_file_mode::FILE_MODE_TRUNCATE_WRITE:
        case ca_file_mode::FILE_MODE_TRUNCATE_READ_WRITE:
            return TRUNCATE_EXISTING;

        default:
            return OPEN_EXISTING;
    }
}

inline ca_file_result
ca_translate_win32_error(const DWORD err) {
    switch (err) {
        case ERROR_SUCCESS: return ca_file_result::FILE_OK;
        case ERROR_FILE_NOT_FOUND:
        case ERROR_PATH_NOT_FOUND: return ca_file_result::FILE_ERROR_NOT_FOUND;
        case ERROR_ACCESS_DENIED: return ca_file_result::FILE_ERROR_ACCESS_DENIED;
        case ERROR_ALREADY_EXISTS:
        case ERROR_FILE_EXISTS: return ca_file_result::FILE_ERROR_ALREADY_EXISTS;
        case ERROR_INVALID_HANDLE: return ca_file_result::FILE_ERROR_INVALID_HANDLE;
        case ERROR_GEN_FAILURE:
        case ERROR_WRITE_FAULT:
        case ERROR_READ_FAULT:
        case ERROR_SECTOR_NOT_FOUND: return ca_file_result::FILE_ERROR_IO_ERROR;
        case ERROR_NOT_ENOUGH_MEMORY:
        case ERROR_OUTOFMEMORY: return ca_file_result::FILE_ERROR_OUT_OF_MEMORY;
        case ERROR_INVALID_PARAMETER: return ca_file_result::FILE_ERROR_INVALID_PARAMETER;
        case ERROR_NOT_SUPPORTED: return ca_file_result::FILE_ERROR_NOT_SUPPORTED;
        case ERROR_DISK_FULL: return ca_file_result::FILE_ERROR_DISK_FULL;
        case ERROR_BUSY:
        case ERROR_DEVICE_IN_USE:
        case ERROR_SHARING_VIOLATION: return ca_file_result::FILE_ERROR_BUSY; // file in use
        case ERROR_WRITE_PROTECT: return ca_file_result::FILE_ERROR_ACCESS_DENIED; // write-protected disk
        default: return ca_file_result::FILE_ERROR_GENERIC;
    }
}

}



#endif //WIN32_TRANSLATER_H
