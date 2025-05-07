// ================================
// CodeAnalyzer - source/c_src/common/public/ca_io/core/ca_io_path.h
//
// @file
// @brief Defines function declarations for path operations.
// ================================

#ifndef CA_IO_PATH_H
#define CA_IO_PATH_H

//TODO
namespace ca::ca_io {
class ca_io_path {
public:
    explicit ca_io_path(const char* path);
    ca_io_path join(const char* path) const;
    ca_io_path parent() const;
    char* name() const;
    char* suffix() const;
    char* stem() const;
    bool is_absolute() const;
    ca_io_path to_absolute() const;
    ca_io_path normalize() const;
    char* as_posix() const;
    char* as_windows() const;
    bool is_valid() const;
    bool exists() const;

private:
    const char* path;
    const bool isWindows;

    static char preferred_separator() const;
    static char* normalize_path(const char* path) const;
    static bool validate_path(const char* path) const;
};
}

#endif //CA_IO_PATH_H
