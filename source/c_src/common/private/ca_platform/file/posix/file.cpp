// ================================
// CodeAnalyzer - source/c_src/common/private/ca_platform/file/posix/file.cpp
//
// @file
// @brief Implementations of "file.h" for posix.
// ================================

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include "file/file_defs.h"

namespace ca::ca_file {

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

ca_file_handle ca_file_open(const char* path, ca_file_mode mode, ca_file_result* result) {
    if (!path) {
        if (result) *result = ca_file_result::FILE_ERROR_INVALID_PARAMETER;
        return NULL;
    }

    int flags = ca_internal_translate_open_flags(mode);
    int fd = open(path, flags, 0666);
    if (fd >= 0) {
        if (result) *result = ca_file_result::FILE_OK;
        return (void*)(intptr_t)fd;
    } else {
        if (result) *result = ca_internal_translate_errno(errno);
        return NULL;
    }
}

void ca_file_close(ca_file_handle file, ca_file_result* result) {
    if (!file) {
        if (result) *result = ca_file_result::FILE_ERROR_INVALID_HANDLE;
        return;
    }
    int fd = (int)(intptr_t)file;
    if (close(fd) == 0) {
        if (result) *result = ca_file_result::FILE_OK;
    } else {
        if (result) *result = ca_internal_translate_errno(errno);
    }
}

ca_size_t ca_file_read(ca_file_handle file, void* buffer, ca_size_t bytes, ca_file_result* result) {
    if (!file || !buffer || bytes == 0) {
        if (result) *result = ca_file_result::FILE_ERROR_INVALID_PARAMETER;
        return 0;
    }

    int fd = (int)(intptr_t)file;
    ssize_t n = read(fd, buffer, bytes);
    if (n >= 0) {
        if (result) *result = ca_file_result::FILE_OK;
        return (ca_size_t)n;
    } else {
        if (result) *result = ca_internal_translate_errno(errno);
        return 0;
    }
}

ca_size_t ca_file_write(ca_file_handle file, const void* buffer, ca_size_t bytes, ca_file_result* result) {
    if (!file || !buffer || bytes == 0) {
        if (result) *result = ca_file_result::FILE_ERROR_INVALID_PARAMETER;
        return 0;
    }

    int fd = (int)(intptr_t)file;
    ssize_t n = write(fd, buffer, bytes);
    if (n >= 0) {
        if (result) *result = ca_file_result::FILE_OK;
        return (ca_size_t)n;
    } else {
        if (result) *result = ca_internal_translate_errno(errno);
        return 0;
    }
}

int ca_file_seek(ca_file_handle file, long offset, ca_file_seek_origin origin, ca_file_result* result) {
    if (!file) {
        if (result) *result = ca_file_result::FILE_ERROR_INVALID_HANDLE;
        return -1;
    }

    int fd = (int)(intptr_t)file;
    int whence = (origin == ca_file_seek_origin::FILE_SEEK_SET) ? SEEK_SET :
                 (origin == ca_file_seek_origin::FILE_SEEK_CUR) ? SEEK_CUR :
                 SEEK_END;

    if (lseek(fd, offset, whence) == (off_t)-1) {
        if (result) *result = ca_internal_translate_errno(errno);
        return -1;
    }

    if (result) *result = ca_file_result::FILE_OK;
    return 0;
}

long ca_file_tell(ca_file_handle file, ca_file_result* result) {
    if (!file) {
        if (result) *result = ca_file_result::FILE_ERROR_INVALID_HANDLE;
        return -1;
    }

    int fd = (int)(intptr_t)file;
    off_t pos = lseek(fd, 0, SEEK_CUR);
    if (pos == (off_t)-1) {
        if (result) *result = ca_internal_translate_errno(errno);
        return -1;
    }

    if (result) *result = ca_file_result::FILE_OK;
    return (long)pos;
}

int ca_file_flush(ca_file_handle file, ca_file_result* result) {
    if (!file) {
        if (result) *result = ca_file_result::FILE_ERROR_INVALID_HANDLE;
        return -1;
    }

    int fd = (int)(intptr_t)file;
    if (fsync(fd) != 0) {
        if (result) *result = ca_internal_translate_errno(errno);
        return -1;
    }

    if (result) *result = ca_file_result::FILE_OK;
    return 0;
}

ca_size_t ca_file_get_size(ca_file_handle file, ca_file_result* result) {
    if (!file) {
        if (result) *result = ca_file_result::FILE_ERROR_INVALID_HANDLE;
        return 0;
    }

    int fd = (int)(intptr_t)file;
    struct stat st;
    if (fstat(fd, &st) != 0) {
        if (result) *result = ca_internal_translate_errno(errno);
        return 0;
    }

    if (result) *result = ca_file_result::FILE_OK;
    return (ca_size_t)st.st_size;
}

bool ca_file_exists(const char* path, ca_file_result* result) {
    if (!path) {
        if (result) *result = ca_file_result::FILE_ERROR_INVALID_PARAMETER;
        return false;
    }

    if (access(path, F_OK) == 0) {
        if (result) *result = ca_file_result::FILE_OK;
        return true;
    } else {
        if (result) {
            if (errno == ENOENT || errno == ENOTDIR)
                *result = ca_file_result::FILE_ERROR_NOT_FOUND;
            else
                *result = ca_internal_translate_errno(errno);
        }
        return false;
    }
}

ca_file_result ca_file_get_info(const char* path, ca_file_info* info_out) {
    if (!path || !info_out) {
        return ca_file_result::FILE_ERROR_INVALID_PARAMETER;
    }

    struct stat st;
    if (stat(path, &st) != 0) {
        return ca_internal_translate_errno(errno);
    }

    info_out->size_bytes = (size_t)st.st_size;
    info_out->is_directory = S_ISDIR(st.st_mode) ? 1 : 0;
    info_out->is_readable = (access(path, R_OK) == 0) ? 1 : 0;
    info_out->is_writable = (access(path, W_OK) == 0) ? 1 : 0;
#if defined(X_OK) // POSIX可执行权限
    info_out->is_executable = (access(path, X_OK) == 0) ? 1 : 0;
#else
    info_out->is_executable = 0;
#endif
    info_out->modified_time = (uint64_t)st.st_mtime;

    return ca_file_result::FILE_OK;
}

ca_file_result ca_file_delete(const char* path) {
    if (!path) return ca_file_result::FILE_ERROR_INVALID_PARAMETER;
    if (unlink(path) == 0) {
        return ca_file_result::FILE_OK;
    } else {
        return ca_internal_translate_errno(errno);
    }
}

ca_file_result ca_file_copy(const char* src, const char* dst, bool overwrite) {
    if (!src || !dst) return ca_file_result::FILE_ERROR_INVALID_PARAMETER;

    int src_fd = open(src, O_RDONLY);
    if (src_fd < 0) return ca_internal_translate_errno(errno);

    int dst_flags = O_WRONLY | O_CREAT;
    if (!overwrite) {
        dst_flags |= O_EXCL; // 要求如果存在则失败
    }

    int dst_fd = open(dst, dst_flags, 0666);
    if (dst_fd < 0) {
        close(src_fd);
        return ca_internal_translate_errno(errno);
    }

    char buffer[4096];
    ssize_t nread;
    while ((nread = read(src_fd, buffer, sizeof(buffer))) > 0) {
        char* out_ptr = buffer;
        ssize_t nwritten;
        do {
            nwritten = write(dst_fd, out_ptr, nread);
            if (nwritten >= 0) {
                nread -= nwritten;
                out_ptr += nwritten;
            } else {
                close(src_fd);
                close(dst_fd);
                return ca_internal_translate_errno(errno);
            }
        } while (nread > 0);
    }

    close(src_fd);
    close(dst_fd);

    if (nread < 0) {
        return ca_internal_translate_errno(errno);
    }

    return ca_file_result::FILE_OK;
}

ca_file_result ca_file_move(const char* src, const char* dst, bool overwrite) {
    if (!src || !dst) return ca_file_result::FILE_ERROR_INVALID_PARAMETER;

    if (!overwrite) {
        if (access(dst, F_OK) == 0) return ca_file_result::FILE_ERROR_ALREADY_EXISTS;
    }

    if (rename(src, dst) == 0) {
        return ca_file_result::FILE_OK;
    } else {
        return ca_internal_translate_errno(errno);
    }
}

ca_file_result ca_file_rename(const char* old_name, const char* new_name) {
    if (!old_name || !new_name) return ca_file_result::FILE_ERROR_INVALID_PARAMETER;

    if (rename(old_name, new_name) == 0) {
        return ca_file_result::FILE_OK;
    } else {
        return ca_internal_translate_errno(errno);
    }
}

}
