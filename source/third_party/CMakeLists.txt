# ================================
# CodeAnalyzer -source/third_party/CMakeLists.txt
#
# Central build dispatcher for third party submodules
# ================================

set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS 1 CACHE BOOL "Suppress warnings from third-party")

add_subdirectory(fmt)
add_subdirectory(googletest)
add_subdirectory(nlohmann_json)
add_subdirectory(pybind11)
add_subdirectory(spdlog)
add_subdirectory(utf8proc)

set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS 0 CACHE BOOL "Restore developer warnings")