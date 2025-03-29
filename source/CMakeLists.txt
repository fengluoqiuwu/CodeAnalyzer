# ================================
# CodeAnalyzer -source/CMakeLists.txt
#
# Central build dispatcher for submodules
# ================================

# Always include third-party dependencies
add_subdirectory(third_party)

# Build the common C/C++ source code
add_subdirectory(c_src)

# Conditionally build CLI interface
if(ENABLE_CLI)
    add_subdirectory(cli)
endif()

# Conditionally build Python bindings
if(ENABLE_PYTHON_BINDINGS)
    add_subdirectory(bindings)
endif()

# Add subdirectories for tests
set(SOURCE_TEST_PREFIX source)

ca_test_combine(${SOURCE_TEST_PREFIX})
