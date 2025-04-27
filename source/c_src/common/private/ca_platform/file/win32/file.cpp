// ================================
// CodeAnalyzer - source/c_src/common/private/ca_platform/file/win32/file.cpp
//
// @file
// @brief Implementations of "file.h" for win32.
// ================================

#include <windows.h>
#include "file/file.h"
#include "file/file_defs.h"
#include "internal/win32_translater.h"

namespace ca::ca_file {

typedef HANDLE ca_file_handle;

ca_file_handle ca_file_open(const char* path, const ca_file_mode mode, ca_file_result* result) {
    if (!path) {
        if (result) {
            *result = ca_file_result::FILE_ERROR_INVALID_PARAMETER;
        }
        return nullptr;
    }

    const DWORD access = internal::ca_translate_access(mode);
    const DWORD creation = internal::ca_translate_creation(mode);

    // ReSharper disable once CppLocalVariableMayBeConst
    HANDLE hFile = CreateFileA(
        path,
        access,
        FILE_SHARE_READ,
        nullptr,
        creation,
        FILE_ATTRIBUTE_NORMAL,
        nullptr
    );

    if (hFile != INVALID_HANDLE_VALUE) {
        if (result) {
            *result = ca_file_result::FILE_OK;
        }
        if (mode == ca_file_mode::FILE_MODE_APPEND || mode == ca_file_mode::FILE_MODE_APPEND_EXISTING) {
            SetFilePointer(hFile, 0, nullptr, FILE_END);
        }
        return hFile;
    } else {
        if (result) {
            *result = internal::ca_translate_win32_error(GetLastError());
        }
        return nullptr;
    }
}

void ca_file_close(const ca_file_handle file, ca_file_result* result) {
    if (!file) {
        if (result) {
            *result = ca_file_result::FILE_ERROR_INVALID_HANDLE;
        }
        return;
    }

    if (CloseHandle(file)) {
        if (result) {
            *result = ca_file_result::FILE_OK;
        }
    } else {
        if (result) {
            *result = internal::ca_translate_win32_error(GetLastError());
        }
    }
}

ca_size_t ca_file_read(const ca_file_handle file, void* buffer, const ca_size_t bytes, ca_file_result* result) {
    if (!file || !buffer || bytes == 0) {
        if (result) {
            *result = ca_file_result::FILE_ERROR_INVALID_PARAMETER;
        }
        return 0;
    }

    DWORD read_bytes = 0;
    if (ReadFile(file, buffer, bytes, &read_bytes, nullptr)) {
        if (result) {
            *result = ca_file_result::FILE_OK;
        }
        return read_bytes;
    } else {
        if (result) {
            *result = internal::ca_translate_win32_error(GetLastError());
        }
        return 0;
    }
}

ca_size_t ca_file_write(const ca_file_handle file, const void* buffer, const ca_size_t bytes, ca_file_result* result) {
    if (!file || !buffer || bytes == 0) {
        if (result) {
            *result = ca_file_result::FILE_ERROR_INVALID_PARAMETER;
        }
        return 0;
    }

    DWORD written_bytes = 0;
    if (WriteFile(file, buffer, bytes, &written_bytes, nullptr)) {
        if (result) {
            *result = ca_file_result::FILE_OK;
        }
        return written_bytes;
    } else {
        if (result) {
            *result = internal::ca_translate_win32_error(GetLastError());
        }
        return 0;
    }
}

int ca_file_seek(const ca_file_handle file, const long offset, const ca_file_seek_origin origin, ca_file_result* result) {
    if (!file) {
        if (result) {
            *result = ca_file_result::FILE_ERROR_INVALID_HANDLE;
        }
        return -1;
    }

    const DWORD moveMethod = (origin == ca_file_seek_origin::FILE_SEEK_SET) ? FILE_BEGIN :
                             (origin == ca_file_seek_origin::FILE_SEEK_CUR) ? FILE_CURRENT :
                             FILE_END;

    const DWORD pos = SetFilePointer(file, offset, nullptr, moveMethod);
    if (pos == INVALID_SET_FILE_POINTER && GetLastError() != NO_ERROR) {
        if (result) {
            *result = internal::ca_translate_win32_error(GetLastError());
        }
        return -1;
    }

    if (result) {
        *result = ca_file_result::FILE_OK;
    }
    return 0;
}

long ca_file_tell(const ca_file_handle file, ca_file_result* result) {
    if (!file) {
        if (result) {
            *result = ca_file_result::FILE_ERROR_INVALID_HANDLE;
        }
        return -1;
    }

    const DWORD pos = SetFilePointer(file, 0, nullptr, FILE_CURRENT);
    if (pos == INVALID_SET_FILE_POINTER && GetLastError() != NO_ERROR) {
        if (result) {
            *result = internal::ca_translate_win32_error(GetLastError());
        }
        return -1;
    }

    if (result) {
        *result = ca_file_result::FILE_OK;
    }
    return static_cast<long>(pos);
}

int ca_file_flush(const ca_file_handle file, ca_file_result* result) {
    if (!file) {
        if (result) {
            *result = ca_file_result::FILE_ERROR_INVALID_HANDLE;
        }
        return -1;
    }

    if (!FlushFileBuffers(file)) {
        if (result) {
            *result = internal::ca_translate_win32_error(GetLastError());
        }
        return -1;
    }

    if (result) {
        *result = ca_file_result::FILE_OK;
    }
    return 0;
}

ca_size_t ca_file_get_size(const ca_file_handle file, ca_file_result* result) {
    if (!file) {
        if (result) {
            *result = ca_file_result::FILE_ERROR_INVALID_HANDLE;
        }
        return 0;
    }

    LARGE_INTEGER size;
    if (!GetFileSizeEx(file, &size)) {
        if (result) *result = internal::ca_translate_win32_error(GetLastError());
        return 0;
    }

    if (result) *result = ca_file_result::FILE_OK;
    return static_cast<ca_size_t>(size.QuadPart);
}

bool ca_file_exists(const char* path, ca_file_result* result) {
    if (!path) {
        if (result) {
            *result = ca_file_result::FILE_ERROR_INVALID_PARAMETER;
        }
        return false;
    }

    if (GetFileAttributesA(path) == INVALID_FILE_ATTRIBUTES) {
        if (result) {
            const DWORD err = GetLastError();
            if (err == ERROR_FILE_NOT_FOUND || err == ERROR_PATH_NOT_FOUND) {
                *result = ca_file_result::FILE_ERROR_NOT_FOUND;
            }
            else {
                *result = internal::ca_translate_win32_error(err);
            }
        }
        return false;
    }

    if (result) {
        *result = ca_file_result::FILE_OK;
    }
    return true;
}

ca_file_result ca_file_get_info(const char* path, ca_file_info* info_out) {
    if (!path || !info_out) {
        return ca_file_result::FILE_ERROR_INVALID_PARAMETER;
    }

    WIN32_FILE_ATTRIBUTE_DATA data;
    if (!GetFileAttributesExA(path, GetFileExInfoStandard, &data)) {
        return internal::ca_translate_win32_error(GetLastError());
    }

    info_out->size_bytes = (static_cast<uint64_t>(data.nFileSizeHigh) << 32) | data.nFileSizeLow;
    info_out->is_directory = (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ? 1 : 0;
    info_out->is_readable = 1;
    info_out->is_writable = !(data.dwFileAttributes & FILE_ATTRIBUTE_READONLY);
    info_out->is_executable = 0;
    info_out->modified_time = (static_cast<uint64_t>(data.ftLastWriteTime.dwHighDateTime) << 32 |
                               data.ftLastWriteTime.dwLowDateTime) / 10000000ULL - 11644473600ULL;

    return ca_file_result::FILE_OK;
}

ca_file_result ca_file_delete(const char* path) {
    if (!path) {
        return ca_file_result::FILE_ERROR_INVALID_PARAMETER;
    }
    if (DeleteFileA(path)) {
        return ca_file_result::FILE_OK;
    } else {
        return internal::ca_translate_win32_error(GetLastError());
    }
}

ca_file_result ca_file_copy(const char* src, const char* dst, const bool overwrite) {
    if (!src || !dst) {
        return ca_file_result::FILE_ERROR_INVALID_PARAMETER;
    }
    if (CopyFileA(src, dst, overwrite ? FALSE : TRUE)) {
        return ca_file_result::FILE_OK;
    } else {
        return internal::ca_translate_win32_error(GetLastError());
    }
}

ca_file_result ca_file_move(const char* src, const char* dst, const bool overwrite) {
    if (!src || !dst) {
        return ca_file_result::FILE_ERROR_INVALID_PARAMETER;
    }

    DWORD flags = MOVEFILE_COPY_ALLOWED;
    if (overwrite) {
        flags |= MOVEFILE_REPLACE_EXISTING;
    }

    if (MoveFileExA(src, dst, flags)) {
        return ca_file_result::FILE_OK;
    } else {
        return internal::ca_translate_win32_error(GetLastError());
    }
}

ca_file_result ca_file_rename(const char* old_name, const char* new_name) {
    if (!old_name || !new_name) {
        return ca_file_result::FILE_ERROR_INVALID_PARAMETER;
    }

    if (MoveFileExA(old_name, new_name, MOVEFILE_COPY_ALLOWED)) {
        return ca_file_result::FILE_OK;
    } else {
        return internal::ca_translate_win32_error(GetLastError());
    }
}

}