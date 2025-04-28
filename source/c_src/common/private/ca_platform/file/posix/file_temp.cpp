// ================================
// CodeAnalyzer - source/c_src/common/private/ca_platform/file/posix/file_temp.cpp
//
// @file
// @brief Implementations of "file_temp.h" for posix.
// ================================

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <limits.h>
#include "file/file_temp.h"
#include "file/file_defs.h"
#include "internal/posix_translater.h"

namespace ca::ca_file {

namespace internal {

static const char* ca_posix_get_temp_dir() {
    const char* tmp = getenv("TMPDIR");
    return tmp ? tmp : "/tmp";
}

}

ca_file_result ca_file_create_temp_file(char* out_path, size_t out_size) {
    if (!out_path || out_size == 0) {
        return ca_file_result::FILE_ERROR_INVALID_PARAMETER;
    }

    const char* temp_dir = internal::ca_posix_get_temp_dir();
    char template_path[PATH_MAX];
    snprintf(template_path, sizeof(template_path), "%s/ca_tempfile_XXXXXX", temp_dir);

    int fd = mkstemp(template_path);
    if (fd == -1) {
        return internal::ca_translate_errno(errno);
    }
    close(fd);

    if (strlen(template_path) + 1 > out_size) {
        unlink(template_path);
        return ca_file_result::FILE_ERROR_OUT_OF_MEMORY;
    }

    strcpy(out_path, template_path);
    return ca_file_result::FILE_OK;
}

ca_file_result ca_file_create_temp_directory(char* out_path, size_t out_size) {
    if (!out_path || out_size == 0) {
        return ca_file_result::FILE_ERROR_INVALID_PARAMETER;
    }

    const char* temp_dir = internal::ca_posix_get_temp_dir();
    char template_path[PATH_MAX];
    snprintf(template_path, sizeof(template_path), "%s/ca_tempdir_XXXXXX", temp_dir);

    if (!mkdtemp(template_path)) {
        return internal::ca_translate_errno(errno);
    }

    if (strlen(template_path) + 1 > out_size) {
        return ca_file_result::FILE_ERROR_OUT_OF_MEMORY;
    }

    strcpy(out_path, template_path);
    return ca_file_result::FILE_OK;
}

}