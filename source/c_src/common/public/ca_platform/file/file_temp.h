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
/**
 * @brief Creates a unique temporary file.
 */
ca_file_result ca_file_create_temp_file(char* out_path, size_t out_size);

/**
 * @brief Creates a unique temporary directory.
 */
ca_file_result ca_file_create_temp_directory(char* out_path, size_t out_size);

}

#endif //FILE_TEMP_H
