// ================================
// CodeAnalyzer - source/c_src/common/public/ca_platform/file/file_path.h
//
// @file
// @brief Defines platform functions for file path.
// ================================

#ifndef FILE_PATH_H
#define FILE_PATH_H

#include "file/file_defs.h"

namespace ca::ca_file {

/* -------------------------------------------
   Path utilities (platform-independent)
   ------------------------------------------- */
char ca_file_get_path_separator();
ca_file_result ca_file_join_path(char* out_path, ca_size_t out_size, const char* path1, const char* path2);
ca_file_result ca_file_get_absolute_path(const char* relative, char* out_path, ca_size_t out_size);
ca_file_result ca_file_normalize_path(const char* input, char* out, ca_size_t out_size);

/* -------------------------------------------
   Comments
   ------------------------------------------- */
// NOLINTBEGIN(readability-redundant-declaration)

/**
 * @brief Gets the platform-specific path separator ('/' or '\\').
 *
 * This function returns the default file path separator character for the underlying platform:
 * - POSIX (Linux, macOS): '/'
 * - Windows: '\\'
 *
 * @return Path separator character.
 */
char ca_file_get_path_separator();

/**
 * @brief Joins two path components into a full path.
 *
 * Safely concatenates two paths, inserting a platform-specific separator if needed.
 * Ensures that the resulting path fits within the provided output buffer.
 *
 * @param out_path Output buffer to store the combined path.
 * @param out_size Size of the output buffer in bytes.
 * @param path1 First part of the path.
 * @param path2 Second part of the path.
 * @return FILE_OK on success, or an error code if the buffer is too small or parameters are invalid.
 */
ca_file_result ca_file_join_path(char* out_path, ca_size_t out_size, const char* path1, const char* path2);

/**
 * @brief Converts a relative path to an absolute one.
 *
 * Resolves a relative file path into an absolute path.
 * This process also resolves symbolic links, redundant components (e.g., ".", ".."),
 * and returns a normalized absolute path.
 *
 * @param relative Relative or partially qualified path to resolve.
 * @param out_path Output buffer to store the absolute path.
 * @param out_size Size of the output buffer in bytes.
 * @return FILE_OK on success, or an error code if the path cannot be resolved or the buffer is too small.
 */
ca_file_result ca_file_get_absolute_path(const char* relative, char* out_path, ca_size_t out_size);

/**
 * @brief Normalizes a file path by resolving '.', '..', and redundant separators.
 *
 * Converts the input path to a clean, canonical form by resolving references to
 * the current directory ('.'), parent directory ('..'), and correcting inconsistent
 * separator usage based on platform rules.
 *
 * On POSIX systems, internally calls `realpath()`.
 * On Windows, internally calls `GetFullPathNameA()`.
 *
 * @param input Input path string to normalize.
 * @param out Output buffer to store the normalized path.
 * @param out_size Size of the output buffer in bytes.
 * @return FILE_OK on success, or an error code if normalization fails or the buffer is too small.
 */
ca_file_result ca_file_normalize_path(const char* input, char* out, ca_size_t out_size);

// NOLINTEND(readability-redundant-declaration)
}

#endif //FILE_PATH_H
