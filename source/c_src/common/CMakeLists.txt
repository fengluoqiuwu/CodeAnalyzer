# ================================
# CodeAnalyzer - source/c_src/common/CMakeLists.txt
#
# Common engine components and services
# ================================

# ================================
# Misc Library
# ================================
# Collect Misc Library sources
set(CA_MISC_SOURCES
        private/ca_misc/temp.cpp
)

# Collect Misc Library headers to be installed
set(CA_MISC_PUBLIC_HEADERS
        public/ca_misc/ca_base_macros.h
)

# Build Misc Library as a static library
add_library(ca_misc STATIC)

# Set the sources for the Misc Library
target_sources(ca_misc
    PUBLIC ${CA_MISC_PUBLIC_HEADERS}
    PRIVATE ${CA_MISC_SOURCES}
)

# Include directories for Misc Library
target_include_directories(ca_misc
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/public/ca_misc
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/private/ca_misc
)

# ================================
# Math Library
# ================================
# Collect Math Library sources
set(CA_MATH_SOURCES
        private/ca_math/ca_math_func.tpp
)

# Collect Math Library headers to be installed
set(CA_MATH_PUBLIC_HEADERS
        public/ca_math/ca_float.h
        public/ca_math/ca_math.h
        public/ca_math/ca_math_func.h
        public/ca_math/ca_int.h
        public/ca_math/ca_math_type_list.h
)

# Build Math Library as a static library
add_library(ca_math STATIC)

# Set the sources for the Math Library
target_sources(ca_math
    PUBLIC ${CA_MATH_PUBLIC_HEADERS}
    PRIVATE ${CA_MATH_SOURCES}
)

# Include directories for Math Library
target_include_directories(ca_math
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/public/ca_math
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/private/ca_math
)

target_link_libraries(ca_math PRIVATE ca_misc)

# ================================
# String Library
# ================================
# Collect String Library sources
set(CA_STRING_SOURCES
        private/ca_string/ca_buffer.tpp
        private/ca_string/ca_char.tpp
        private/ca_string/ca_fastsearch.tpp
        private/ca_string/ca_stream.cpp
        private/ca_string/ca_utf8_utils.cpp
)

# Collect String Library headers to be installed
set(CA_STRING_PUBLIC_HEADERS
        public/ca_string/ca_buffer.h
        public/ca_string/ca_char.h
        public/ca_string/ca_char_types.h
        public/ca_string/ca_fastsearch.h
        public/ca_string/ca_stream.h
        public/ca_string/ca_string.h
        public/ca_string/ca_utf8_utils.h
)

# Build String Library as a static library
add_library(ca_string STATIC)

# Set the sources for the String Library
target_sources(ca_string
    PUBLIC ${CA_STRING_PUBLIC_HEADERS}
    PRIVATE ${CA_STRING_SOURCES}
)

# Include directories for String Library
target_include_directories(ca_string
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/public/ca_string
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/private/ca_string
)

target_link_libraries(ca_string PRIVATE utf8proc ca_misc ca_math)

# ================================
# Tests
# ================================

add_subdirectory(tests)
