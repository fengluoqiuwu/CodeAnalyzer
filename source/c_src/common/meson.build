# ================================
# CodeAnalyzer - source/c_src/common/meson.build
#
# Common engine components and services
# ================================

# ================================
# Math Library
# ================================
# Collect Math Library sources
ca_math_sources = files(

)

# Collect Math Library headers to be installed
ca_math_public_headers = files(
    'public/ca_math/ca_math.h',
    'public/ca_math/ca_int.h'
)

# Build Math Library as a static library
ca_math_lib = static_library(
    'ca_math',
    ca_math_sources,
    include_directories: [
        include_directories('public/ca_math'),
        include_directories('private/ca_math'),
    ],
    install: false
)

# Declare Math Library dependency
ca_math_dep = declare_dependency(
    include_directories: include_directories('public/ca_math'),
    link_with: ca_math_lib
)

# ================================
# String Library
# ================================

# Collect String Library sources
ca_string_sources = files(
    'private/ca_string/ca_buffer.cpp',
    'private/ca_string/ca_char.cpp',
    'private/ca_string/ca_stream.cpp',
    'private/ca_string/ca_utf8_utils.cpp'
)

# Collect String Library headers to be installed
ca_string_public_headers = files(
    'public/ca_string/ca_buffer.h',
    'public/ca_string/ca_char.h',
    'public/ca_string/ca_stream.h',
    'public/ca_string/ca_string.h',
    'public/ca_string/ca_utf8_utils.h'
)

# Build String Library as a static library
ca_string_lib = static_library(
    'ca_string',
    ca_string_sources,
    include_directories: [
        include_directories('public/ca_string'),
        include_directories('private/ca_string'),
    ],
    dependencies: [py_dep, ca_math_dep],
    install: false
)

# Declare String Library dependency
ca_string_dep = declare_dependency(
    include_directories: include_directories('public/ca_string'),
    link_with: ca_string_lib
)

# Install headers related to String Library
install_headers(ca_string_public_headers, subdir: 'code_analyzer/common/ca_string')

# ================================
# Tests
# ================================

subdir('tests')
