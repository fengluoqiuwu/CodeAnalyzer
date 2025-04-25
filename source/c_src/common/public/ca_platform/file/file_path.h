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
/**
 * @brief Gets the platform-specific path separator ('/' or '\\').
 */
char ca_file_get_path_separator();

/**
 * @brief Joins two path components into a full path.
 */
ca_file_result ca_file_join_path(char* out_path, size_t out_size, const char* path1, const char* path2);

/**
 * @brief Converts a relative path to an absolute one.
 */
ca_file_result ca_file_get_absolute_path(const char* relative, char* out_path, size_t out_size);

/**
 * @brief Normalize path (resolving '.', '..', separator).
 */
ca_file_result ca_file_normalize_path(const char* input, char* out, size_t out_size);

}

#endif //FILE_PATH_H
