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
 */
enum class ca_file_mode {
    FILE_MODE_READ = 1,       /**< Open file for reading. */
    FILE_MODE_WRITE,          /**< Open file for writing (truncate). */
    FILE_MODE_APPEND,         /**< Open file for appending. */
    FILE_MODE_READ_WRITE,     /**< Open file for reading and writing. */
    FILE_MODE_BINARY          /**< Open file in binary mode (platform-dependent). */
 };

/**
 * @enum ca_file_seek_origin
 * @brief Seek origin options.
 */
enum class ca_file_seek_origin {
    FILE_SEEK_SET = 0,  /**< Seek from beginning of file. */
    FILE_SEEK_CUR = 1,  /**< Seek from current position. */
    FILE_SEEK_END = 2   /**< Seek from end of file. */
 };

/**
 * @struct ca_file_info
 * @brief File information metadata.
 */
typedef struct {
    size_t size_bytes;          /**< File size in bytes. */
    int is_directory;           /**< Non-zero if the path is a directory. */
    int is_readable;            /**< Non-zero if readable. */
    int is_writable;            /**< Non-zero if writable. */
    int is_executable;          /**< Non-zero if executable. */
    uint64_t modified_time;     /**< Last modified time (UNIX epoch). */
} ca_file_info;

/**
 * @enum ca_file_result
 * @brief Result codes returned by file operations.
 */
enum class ca_file_result {
    FILE_OK = 0,                   /**< Operation succeeded. */
    FILE_ERROR_GENERIC = -1,      /**< Generic error. */
    FILE_ERROR_NOT_FOUND = -2,    /**< File not found. */
    FILE_ERROR_ACCESS_DENIED = -3,/**< Permission denied. */
    FILE_ERROR_ALREADY_EXISTS = -4,/**< File already exists. */
    FILE_ERROR_INVALID_HANDLE = -5,/**< Invalid file handle. */
    FILE_ERROR_NOT_IMPLEMENTED = -99 /**< Function not implemented. */
 };

}

#endif //FILE_DEFS_H
