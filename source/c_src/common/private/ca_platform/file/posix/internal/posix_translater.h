// ================================
// CodeAnalyzer - source/c_src/common/private/ca_platform/file/posix/internal/posix_translater
//
// @file
// @brief Translator for Posix file operations.
// ================================

#ifndef POSIX_TRANSLATER_H
#define POSIX_TRANSLATER_H

#include <fcntl.h>
#include "file/file_defs.h"

namespace ca::ca_file::internal {

static int ca_internal_translate_open_flags(const ca_file_mode mode) {
    switch (mode) {
        case ca_file_mode::FILE_MODE_READ:
        case ca_file_mode::FILE_MODE_READ_EXISTING:
            return O_RDONLY;

        case ca_file_mode::FILE_MODE_WRITE:
            return O_WRONLY | O_CREAT | O_TRUNC;

        case ca_file_mode::FILE_MODE_APPEND:
            return O_WRONLY | O_CREAT | O_APPEND;

        case ca_file_mode::FILE_MODE_READ_WRITE:
            return O_RDWR | O_CREAT;

        case ca_file_mode::FILE_MODE_WRITE_EXISTING:
            return O_WRONLY;

        case ca_file_mode::FILE_MODE_READ_WRITE_EXISTING:
            return O_RDWR;

        case ca_file_mode::FILE_MODE_APPEND_EXISTING:
            return O_WRONLY | O_APPEND;

        case ca_file_mode::FILE_MODE_TRUNCATE_WRITE:
            return O_WRONLY | O_TRUNC;

        case ca_file_mode::FILE_MODE_TRUNCATE_READ_WRITE:
            return O_RDWR | O_TRUNC;

        default:
            return O_RDONLY;
    }
}

static ca_file_result ca_internal_translate_errno(int err) {
    switch (err) {
        case 0: return ca_file_result::FILE_OK;
        case ENOENT: return ca_file_result::FILE_ERROR_NOT_FOUND;
        case EACCES: return ca_file_result::FILE_ERROR_ACCESS_DENIED;
        case EEXIST: return ca_file_result::FILE_ERROR_ALREADY_EXISTS;
        case EBADF: return ca_file_result::FILE_ERROR_INVALID_HANDLE;
        case EIO: return ca_file_result::FILE_ERROR_IO_ERROR;
        case ENOMEM: return ca_file_result::FILE_ERROR_OUT_OF_MEMORY;
        case EINVAL: return ca_file_result::FILE_ERROR_INVALID_PARAMETER;
        case ENOTSUP: return ca_file_result::FILE_ERROR_NOT_SUPPORTED;
        case EROFS: return ca_file_result::FILE_ERROR_ACCESS_DENIED; // read-only file system
        case ENOSPC: return ca_file_result::FILE_ERROR_DISK_FULL;
        case EBUSY: return ca_file_result::FILE_ERROR_BUSY;
        case EPERM: return ca_file_result::FILE_ERROR_ACCESS_DENIED;
        case EFAULT: return ca_file_result::FILE_ERROR_INVALID_PARAMETER;
        case EISDIR: return ca_file_result::FILE_ERROR_ACCESS_DENIED; // try open dir as file
        case ENFILE: return ca_file_result::FILE_ERROR_OUT_OF_MEMORY; // too many open files
        case EMFILE: return ca_file_result::FILE_ERROR_OUT_OF_MEMORY; // process file descriptor limit
        default: return ca_file_result::FILE_ERROR_GENERIC;
    }
}

}



#endif //POSIX_TRANSLATER_H
