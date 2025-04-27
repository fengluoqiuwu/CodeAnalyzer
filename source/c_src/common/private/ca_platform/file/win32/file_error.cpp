// ================================
// CodeAnalyzer - source/c_src/common/private/ca_platform/file/win32/file_error.cpp
//
// @file
// @brief Implementations of "file_error.h" for win32.
// ================================

#include <cstring>
#include <windows.h>
#include "file/file_error.h"
#include "file/file_defs.h"

namespace ca::ca_file {

const char* ca_file_get_last_error_message() {
    static char buffer[512];
    const DWORD errorCode = GetLastError();
    if (errorCode == 0) {
        return "No error.";
    }

    const DWORD len = FormatMessageA(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        errorCode,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        buffer,
        sizeof(buffer),
        nullptr
    );

    if (len == 0) {
        return "Unknown error.";
    }

    return buffer;
}

int ca_file_get_last_error_code() {
    return static_cast<int>(GetLastError());
}

}