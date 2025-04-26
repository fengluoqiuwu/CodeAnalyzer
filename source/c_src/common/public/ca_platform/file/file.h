// ================================
// CodeAnalyzer - source/c_src/common/public/ca_platform/file/file.h
//
// @file
// @brief Defines platform functions for files operating.
// ================================

#ifndef FILE_H
#define FILE_H

#include "file/file_defs.h"

namespace ca::ca_file {

/* -------------------------------------------
   Basic file operations
   ------------------------------------------- */
ca_file_handle ca_file_open(const char* path, ca_file_mode mode, ca_file_result* result = nullptr);
void ca_file_close(ca_file_handle file, ca_file_result* result = nullptr);
ca_size_t ca_file_read(ca_file_handle file, void* buffer, ca_size_t bytes, ca_file_result* result = nullptr);
ca_size_t ca_file_write(ca_file_handle file, const void* buffer, ca_size_t bytes, ca_file_result* result = nullptr);
int ca_file_seek(ca_file_handle file, long offset, ca_file_seek_origin origin, ca_file_result* result = nullptr);
long ca_file_tell(ca_file_handle file, ca_file_result* result = nullptr);
int ca_file_flush(ca_file_handle file, ca_file_result* result = nullptr);
ca_size_t ca_file_get_size(ca_file_handle file, ca_file_result* result = nullptr);

/* -------------------------------------------
   File status and management
   ------------------------------------------- */
bool ca_file_exists(const char* path, ca_file_result* result = nullptr);
ca_file_result ca_file_get_info(const char* path, ca_file_info* info_out);
ca_file_result ca_file_delete(const char* path);
ca_file_result ca_file_copy(const char* src, const char* dst, bool overwrite);
ca_file_result ca_file_move(const char* src, const char* dst, bool overwrite);
ca_file_result ca_file_rename(const char* old_name, const char* new_name);


/* -------------------------------------------
   Comments
   ------------------------------------------- */
// NOLINTBEGIN(readability-redundant-declaration)

/**
 * @brief Opens a file with the specified mode.
 *
 * @param path Path to the file to open.
 * @param mode File open mode (read, write, append, etc.).
 * @param result Pointer to result code indicating success or error type.
 * @return A valid file handle on success, or an invalid handle on failure.
 */
ca_file_handle ca_file_open(const char* path, ca_file_mode mode, ca_file_result* result);

/**
 * @brief Closes a previously opened file handle.
 *
 * @param file File handle to close.
 * @param result Pointer to result code indicating success or error type.
 */
void ca_file_close(ca_file_handle file, ca_file_result* result);

/**
 * @brief Reads bytes from file into buffer.
 *
 * @param file File handle to read from.
 * @param buffer Destination buffer to store read bytes.
 * @param bytes Number of bytes to read.
 * @param result Pointer to result code indicating success or error type.
 * @return The number of bytes actually read, or 0 on error or end-of-file.
 */
ca_size_t ca_file_read(ca_file_handle file, void* buffer, ca_size_t bytes, ca_file_result* result);

/**
 * @brief Writes bytes from buffer to file.
 *
 * @param file File handle to write to.
 * @param buffer Source buffer containing data to write.
 * @param bytes Number of bytes to write.
 * @param result Pointer to result code indicating success or error type.
 * @return The number of bytes actually written, or 0 on error.
 */
ca_size_t ca_file_write(ca_file_handle file, const void* buffer, ca_size_t bytes, ca_file_result* result);

/**
 * @brief Moves file pointer to a specific location.
 *
 * @param file File handle to seek.
 * @param offset Byte offset to move to, relative to origin.
 * @param origin Starting point for the offset (beginning, current, or end).
 * @param result Pointer to result code indicating success or error type.
 * @return 0 on success, non-zero on failure.
 */
int ca_file_seek(ca_file_handle file, long offset, ca_file_seek_origin origin, ca_file_result* result);

/**
 * @brief Gets current file pointer position.
 *
 * @param file File handle to query.
 * @param result Pointer to result code indicating success or error type.
 * @return Current position in the file in bytes, or -1 on failure.
 */
long ca_file_tell(ca_file_handle file, ca_file_result* result);

/**
 * @brief Flushes file buffers to disk.
 *
 * @param file File handle to flush.
 * @param result Pointer to result code indicating success or error type.
 * @return 0 on success, non-zero on failure.
 */
int ca_file_flush(ca_file_handle file, ca_file_result* result);

/**
 * @brief Gets total size of the file in bytes.
 *
 * @param file File handle to query.
 * @param result Pointer to result code indicating success or error type.
 * @return File size in bytes, or 0 on error.
 */
ca_size_t ca_file_get_size(ca_file_handle file, ca_file_result* result);

/**
 * @brief Checks if a file exists.
 *
 * @param path Path to the file.
 * @param result Pointer to result code indicating success or error type.
 * @return true if the file exists, false otherwise.
 */
bool ca_file_exists(const char* path, ca_file_result* result);

/**
 * @brief Retrieves file information.
 *
 * @param path Path to the file.
 * @param info_out Pointer to structure that will receive file information.
 * @return Result code indicating success or error type.
 */
ca_file_result ca_file_get_info(const char* path, ca_file_info* info_out);

/**
 * @brief Deletes a file.
 *
 * @param path Path to the file to delete.
 * @return Result code indicating success or error type.
 */
ca_file_result ca_file_delete(const char* path);

/**
 * @brief Copies a file to a new location.
 *
 * @param src Path to the source file.
 * @param dst Path to the destination file.
 * @param overwrite Non-zero to overwrite the destination file if it exists.
 * @return Result code indicating success or error type.
 */
ca_file_result ca_file_copy(const char* src, const char* dst, bool overwrite);

/**
 * @brief Moves or renames a file to a new location.
 *
 * @param src Path to the source file.
 * @param dst Path to the destination file.
 * @param overwrite Non-zero to overwrite the destination file if it exists.
 * @return Result code indicating success or error type.
 */
ca_file_result ca_file_move(const char* src, const char* dst, bool overwrite);

/**
 * @brief Renames a file.
 *
 * @param old_name Current name (or path) of the file.
 * @param new_name New name (or path) to assign to the file.
 * @return Result code indicating success or error type.
 */
ca_file_result ca_file_rename(const char* old_name, const char* new_name);

// NOLINTEND(readability-redundant-declaration)

}

#endif //FILE_H

