# ================================
# CodeAnalyzer - source/c_src/common/public/platform/CMakeLists.txt
#
# Platform information
# ================================

add_library(ca_platform_config STATIC)

set_target_properties(ca_platform_config PROPERTIES LINKER_LANGUAGE CXX)

include(${CMAKE_CURRENT_SOURCE_DIR}/config/PlatformDetect.cmake)

configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/config/config.h.in
        ${CMAKE_CURRENT_BINARY_DIR}/config/config.h
)

target_sources(ca_platform_config
        PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/config/config.h
)

target_include_directories(ca_platform_config
        PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/config
)

