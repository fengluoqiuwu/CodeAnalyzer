// ================================
// CodeAnalyzer - source/c_src/common/public/ca_platform/file/file_defs.h
//
// @file
// @brief Defines platform macros for files operating.
// ================================

#ifndef FILE_DEFS_H
#define FILE_DEFS_H

#include "ca_int.h"

namespace ca::ca_file {

/**
 * @brief Opaque file handle type (platform-dependent).
 */
typedef void* ca_file_handle;

/**
 * @enum ca_file_mode
 * @brief File open modes.
 *
 * Defines detailed file open modes for consistent behavior across POSIX and Windows systems.
 * Includes options for creating, truncating, appending, and exclusive access.
 */
enum class ca_file_mode {
    FILE_MODE_READ = 1,         /**< Open existing file for reading only.
                                     - POSIX: O_RDONLY
                                     - Windows: GENERIC_READ + OPEN_EXISTING */
    FILE_MODE_WRITE,            /**< Open file for writing only. Create if not exists, truncate if exists.
                                     - POSIX: O_WRONLY | O_CREAT | O_TRUNC
                                     - Windows: GENERIC_WRITE + CREATE_ALWAYS */
    FILE_MODE_APPEND,           /**< Open file for writing at the end. Create if not exists.
                                     - POSIX: O_WRONLY | O_CREAT | O_APPEND
                                     - Windows: FILE_APPEND_DATA + OPEN_ALWAYS + seek to end */
    FILE_MODE_READ_WRITE,       /**< Open file for reading and writing. Create if not exists.
                                     - POSIX: O_RDWR | O_CREAT
                                     - Windows: GENERIC_READ | GENERIC_WRITE + OPEN_ALWAYS */

    FILE_MODE_READ_EXISTING,    /**< Open file for reading only, fail if not exists.
                                     - POSIX: O_RDONLY
                                     - Windows: GENERIC_READ + OPEN_EXISTING */
    FILE_MODE_WRITE_EXISTING,   /**< Open existing file for writing only, fail if not exists (no create).
                                     - POSIX: O_WRONLY
                                     - Windows: GENERIC_WRITE + OPEN_EXISTING */
    FILE_MODE_READ_WRITE_EXISTING, /**< Open existing file for reading and writing, fail if not exists.
                                     - POSIX: O_RDWR
                                     - Windows: GENERIC_READ | GENERIC_WRITE + OPEN_EXISTING */

    FILE_MODE_APPEND_EXISTING,  /**< Open existing file for appending, fail if not exists.
                                     - POSIX: O_WRONLY | O_APPEND
                                     - Windows: FILE_APPEND_DATA + OPEN_EXISTING + seek to end */

    FILE_MODE_TRUNCATE_WRITE,   /**< Open existing file and truncate it. Fail if file doesn't exist.
                                     - POSIX: O_WRONLY | O_TRUNC
                                     - Windows: GENERIC_WRITE + TRUNCATE_EXISTING */

    FILE_MODE_TRUNCATE_READ_WRITE /**< Open existing file for read/write and truncate it. Fail if not exists.
                                     - POSIX: O_RDWR | O_TRUNC
                                     - Windows: GENERIC_READ | GENERIC_WRITE + TRUNCATE_EXISTING */
};

/**
 * @enum ca_file_seek_origin
 * @brief File seek origin options.
 *
 * Specifies the reference point used for file pointer movement during seek operations.
 * Behaves consistently across POSIX and Windows.
 */
enum class ca_file_seek_origin {
    FILE_SEEK_SET = 0,  /**< Seek relative to the beginning of the file (offset = absolute position). */
    FILE_SEEK_CUR = 1,  /**< Seek relative to the current file pointer position. */
    FILE_SEEK_END = 2   /**< Seek relative to the end of the file (offset usually negative). */
};

/**
 * @struct ca_file_info
 * @brief Metadata describing a file or directory.
 *
 * Retrieved via ca_file_get_info(), this structure provides essential properties
 * such as size, access permissions, type, and modification timestamp.
 */
typedef struct {
    size_t size_bytes;          /**< Size of the file in bytes. Zero for directories on some platforms. */
    int is_directory;           /**< Non-zero if the path refers to a directory. */
    int is_readable;            /**< Non-zero if the file is readable. */
    int is_writable;            /**< Non-zero if the file is writable. */
    int is_executable;          /**< Non-zero if the file is executable (POSIX only; Windows approximated). */
    uint64_t modified_time;     /**< Last modification time in seconds since UNIX epoch (UTC 1970-01-01 00:00:00). */
} ca_file_info;

/**
 * @enum ca_file_result
 * @brief Standard result codes for file operations.
 *
 * These codes are returned by most ca_file_* functions to indicate success or specific failure types.
 */
enum class ca_file_result {
    FILE_OK = 0,                        /**< Operation completed successfully. */
    FILE_ERROR_GENERIC = -1,            /**< Unspecified error occurred. */
    FILE_ERROR_NOT_FOUND = -2,          /**< Target file or directory was not found. */
    FILE_ERROR_ACCESS_DENIED = -3,      /**< Access denied due to permissions. */
    FILE_ERROR_ALREADY_EXISTS = -4,     /**< Destination file already exists when it should not. */
    FILE_ERROR_INVALID_HANDLE = -5,     /**< Invalid file handle or pointer used. */
    FILE_ERROR_IO_ERROR = -6,           /**< Low-level input/output error occurred. */
    FILE_ERROR_OUT_OF_MEMORY = -7,      /**< Memory allocation failed during operation. */
    FILE_ERROR_INVALID_PARAMETER = -8,  /**< Invalid arguments passed to a file function. */
    FILE_ERROR_NOT_SUPPORTED = -9,      /**< Operation not supported by the platform or file system. */
    FILE_ERROR_DISK_FULL = -10,         /**< Disk space exhausted while writing. */
    FILE_ERROR_BUSY = -11,              /**< File is currently in use or locked. */
    FILE_ERROR_NOT_IMPLEMENTED = -99    /**< Operation explicitly not implemented yet. */
};

}

#endif //FILE_DEFS_H
