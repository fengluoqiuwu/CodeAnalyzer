// ================================
// CodeAnalyzer - source/c_src/common/public/ca_platform/file/file_error.h
//
// @file
// @brief Defines platform functions for file errors.
// ================================

#ifndef FILE_ERROR_H
#define FILE_ERROR_H

namespace ca::ca_file {

/* -------------------------------------------
   Error handling
   ------------------------------------------- */
/**
 * @brief Retrieves the last platform-specific error message.
 */
const char* ca_file_get_last_error_message();

/**
 * @brief Gets the last error code.
 */
int ca_file_get_last_error_code();

}

#endif //FILE_ERROR_H
