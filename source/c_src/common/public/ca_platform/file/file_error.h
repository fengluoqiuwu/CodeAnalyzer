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
const char* ca_file_get_last_error_message();
int ca_file_get_last_error_code();

/* -------------------------------------------
   Comments
   ------------------------------------------- */
// NOLINTBEGIN(readability-redundant-declaration)

/**
 * @brief Retrieves the last platform-specific error message.
 *
 * Returns a human-readable error message string for the most recent system-level error.
 * This function is useful for logging or debugging platform-specific failures.
 *
 * - On POSIX systems, it uses `strerror(errno)`.
 * - On Windows systems, it uses `FormatMessageA(GetLastError())`.
 *
 * @return Pointer to a null-terminated error message string. Lifetime is managed internally and should not be freed by the caller.
 */
const char* ca_file_get_last_error_message();

/**
 * @brief Gets the last platform-specific raw error code.
 *
 * Retrieves the numeric error code corresponding to the last system error.
 *
 * - On POSIX systems, it returns the current value of `errno`.
 * - On Windows systems, it returns the result of `GetLastError()`.
 *
 * @return Last error code as an integer.
 */
int ca_file_get_last_error_code();

// NOLINTEND(readability-redundant-declaration)

}

#endif //FILE_ERROR_H
