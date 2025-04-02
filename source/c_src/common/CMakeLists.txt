# ================================
# CodeAnalyzer - source/c_src/common/CMakeLists.txt
#
# Common engine components and services
# ================================

# ================================
# Math Library
# ================================
# Collect Math Library sources
set(CA_MATH_SOURCES
        private/ca_math/ca_math_func.cpp
        private/ca_math/temp.cpp
)

# Collect Math Library headers to be installed
set(CA_MATH_PUBLIC_HEADERS
        public/ca_math/ca_math.h
        public/ca_math/ca_math_func.h
        public/ca_math/ca_int.h
)

# Build Math Library as a static library
add_library(ca_math STATIC ${CA_MATH_SOURCES})

# Include directories for Math Library
target_include_directories(ca_math
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/public/ca_math
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/private/ca_math
)

# ================================
# String Library
# ================================
# Collect String Library sources
set(CA_STRING_SOURCES
        private/ca_string/ca_buffer.cpp
        private/ca_string/ca_char.cpp
        private/ca_string/ca_fastsearch.cpp
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
add_library(ca_string STATIC ${CA_STRING_SOURCES})

# Include directories for String Library
target_include_directories(ca_string
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/public/ca_string
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/private/ca_string
)

target_link_libraries(ca_string PRIVATE Python3::Python ca_math)

# ================================
# Tests
# ================================

add_subdirectory(tests)
