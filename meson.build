project('CodeAnalyzer', 'cpp',
        version : '1.0.0',
        default_options : ['warning_level=3', 'cpp_std=c++20'])

# These arguments are only used to build the shared library
# not the executables that use the library.
lib_args = ['-DBUILDING_MESON_LIBRARY']

shared_library('CodeAnalyzer', 'main.cpp',
               install : true,
               cpp_args : lib_args
)