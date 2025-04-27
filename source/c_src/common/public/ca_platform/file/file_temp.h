// ================================
// CodeAnalyzer - source/c_src/common/public/ca_platform/file/file_temp.h
//
// @file
// @brief Defines platform functions for temp file.
// ================================

#ifndef FILE_TEMP_H
#define FILE_TEMP_H

#include "file/file_defs.h"

namespace ca::ca_file {

/* -------------------------------------------
   Temporary file/directory creation
   ------------------------------------------- */
ca_file_result ca_file_create_temp_file(char* out_path, size_t out_size);
ca_file_result ca_file_create_temp_directory(char* out_path, size_t out_size);

/* -------------------------------------------
   Comments
   ------------------------------------------- */
// NOLINTBEGIN(readability-redundant-declaration)

/**
 * @brief Creates a unique temporary file.
 *
 * Generates a uniquely named temporary file in the system's temporary directory.
 * The full path of the created file is written to the output buffer.
 *
 * @param out_path Output buffer to receive the full path of the created temporary file.
 * @param out_size Size of the output buffer in bytes.
 * @return FILE_OK on success, or an error code if creation fails or the buffer is too small.
 */
ca_file_result ca_file_create_temp_file(char* out_path, size_t out_size);

/**
 * @brief Creates a unique temporary directory.
 *
 * Generates a uniquely named temporary directory in the system's temporary directory.
 * The full path of the created directory is written to the output buffer.
 *
 * @param out_path Output buffer to receive the full path of the created temporary directory.
 * @param out_size Size of the output buffer in bytes.
 * @return FILE_OK on success, or an error code if creation fails or the buffer is too small.
 */
ca_file_result ca_file_create_temp_directory(char* out_path, size_t out_size);

// NOLINTEND(readability-redundant-declaration)

}

#endif //FILE_TEMP_H
