cmake_minimum_required(VERSION 3.20)  # Ensure a sufficient CMake version
project(CodeAnalyzer VERSION 0.1.0 LANGUAGES C CXX)

# ----------------------------
# ✅ Standard
# ----------------------------

if (MSVC)
    # Set C standard to C17
    set(CMAKE_C_STANDARD 17)
    set(CMAKE_C_STANDARD_REQUIRED ON)
    # Set C++ standard to C++20
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
else()
    # Set C standard to C23
    set(CMAKE_C_STANDARD 23)
    set(CMAKE_C_STANDARD_REQUIRED ON)
    # Set C++ standard to C++23
    set(CMAKE_CXX_STANDARD 23)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_EXTENSIONS OFF)

# Display the standards being used
message(STATUS "Using C standard: ${CMAKE_C_STANDARD}")
message(STATUS "Using C++ standard: ${CMAKE_CXX_STANDARD}")

# ----------------------------
# ✅ Pre setup
# ----------------------------

if(WIN32)
    set(IS_WINDOWS TRUE)
elseif(UNIX)
    if(APPLE)
        set(IS_MACOS TRUE)
    else()
        set(IS_LINUX TRUE)
    endif()
endif()

# ----------------------------
# ✅ Test setup
# ----------------------------

enable_testing()

# Define the ca_test_combine function
function(ca_test_combine prefix)
    # Register the super-test
    add_test(NAME ${prefix} COMMAND ${CMAKE_CTEST_COMMAND} --test-dir ${CMAKE_BINARY_DIR} -R ^${prefix}-)
endfunction()

# Define the ca_test_dir function with an additional 'dir' parameter
# `prefix` is the prefix for each test name
# `dep` represents the additional dependencies (other than gtest)
# `dir` is the relative directory to search for test files
function(ca_test_dir prefix dep dir)
    # Find all the test_*.cpp files in the specified directory (and subdirectories)
    file(GLOB_RECURSE TEST_SOURCES "${dir}/test_*.cpp")

    # Iterate through each test source file
    foreach(TEST_SRC ${TEST_SOURCES})
        # Get the test name by extracting the base name without the path and extension
        get_filename_component(TEST_NAME ${TEST_SRC} NAME_WE)

        # Create an executable for each test with the name "prefix-{test_name}"
        add_executable(${prefix}-${TEST_NAME} ${TEST_SRC})
        message("add_executable:" ${prefix}-${TEST_NAME} ${TEST_SRC})

        # Link the created test executable to gtest_main and any other dependencies passed via 'dep'
        target_link_libraries(${prefix}-${TEST_NAME} PRIVATE gtest_main ${dep})
        message("target_link_libraries:" ${prefix}-${TEST_NAME} PRIVATE gtest_main ${dep})

        # Register each test as a CTest test with the name "prefix-{test_name}"
        add_test(NAME ${prefix}-${TEST_NAME} COMMAND ${prefix}-${TEST_NAME})
        message("add_test:" ${prefix}-${TEST_NAME} COMMAND ${prefix}-${TEST_NAME})
    endforeach()

    # Create a super-test with the name "prefix"
    # This super-test will run all individual tests that have the "prefix-" prefix in their names
    add_test(NAME ${prefix} COMMAND ${CMAKE_CTEST_COMMAND} --test-dir ${CMAKE_BINARY_DIR} -R ^${prefix}-)
endfunction()

# Define the ca_test function
# `prefix` is the prefix for each test name
# `list` is the list of test names and their dependencies:
# - "test_name" for a test with no other dependencies
# - "test_name:"dep1 dep2"" for a test with other dependencies
# The function threat direction as "test_name"
function(ca_test prefix list)
    foreach (TEST_INFO ${list})
        string(FIND ${TEST_INFO} ":" TEST_NAME_POS)
        if(TEST_NAME_POS EQUAL -1)
            set(TEST_NAME ${TEST_INFO})
            set(DEP_NAME ${TEST_INFO})
        else()
            string(REPLACE ":" ";" DEP_NAME "${TEST_INFO}")
            list(GET DEP_NAME 0 TEST_NAME)
        endif()
        ca_test_dir(${prefix}-${TEST_NAME} "${DEP_NAME}" ${TEST_NAME})
    endforeach()

    ca_test_combine(${prefix})
endfunction()

# ----------------------------
# ✅ Options Configuration
# ----------------------------
option(ENABLE_THREADING "Enable multi-threaded analysis engine" ON)
option(ENABLE_OPTIMIZER "Enable memory and performance optimizations" ON)
option(ENABLE_64BIT_INDEX "Use 64-bit indexing (for very large projects)" OFF)
option(ENABLE_CLI "Build command-line interface frontend" ON)
option(ENABLE_PYTHON_BINDINGS "Enable Python module bindings (via pybind11)" ON)
option(ENABLE_LOGGING "Enable internal logging and diagnostics output" ON)
option(BUILD_TESTS "Build test targets and analysis validation framework" OFF)
option(ENABLE_PLUGIN_SYSTEM "Enable plugin system for custom analyzers" ON)
option(USE_SYSTEM_PYBIND11 "Use system-installed pybind11 instead of bundled third_party" OFF)
option(ENABLE_AI_MODULE "Enable experimental AI-powered analysis features" OFF)

# ----------------------------
# ✅ SIMD Optimization Level
# ----------------------------
set(SIMD_LEVEL "auto" CACHE STRING "SIMD optimization level to target (auto, AVX2, AVX512)")

# ----------------------------
# ✅ Compiler setup
# ----------------------------
# Set the compilers to CMake
find_program(CCACHE_PROGRAM ccache)
set(CMAKE_C_COMPILER "gcc" CACHE STRING "Path to C Compiler")
set(CMAKE_CXX_COMPILER "g++" CACHE STRING "Path to C++ Compiler")

# ----------------------------
# ✅ Compiler version checks
# ----------------------------
# GCC Check
if("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
    execute_process(
            COMMAND ${CMAKE_C_COMPILER} --version
            OUTPUT_VARIABLE GCC_VERSION_OUTPUT
    )
    string(REGEX MATCH "([0-9]+)\\.([0-9]+)\\.([0-9]+)" GCC_VERSION_MATCH ${GCC_VERSION_OUTPUT})
    if(GCC_VERSION_MATCH)
        list(GET GCC_VERSION_MATCH 0 GCC_MAJOR_VERSION)
        if(${GCC_MAJOR_VERSION} LESS 13)
            message(FATAL_ERROR "CodeAnalyzer requires GCC >= 13.0 for full C23/C++23 support")
        endif()
    endif()
elseif("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
    message(FATAL_ERROR "❌ Clang is not officially supported for this project (C23/C++23 incomplete)")
elseif("${CMAKE_C_COMPILER_ID}" STREQUAL "MSVC")
    execute_process(
            COMMAND ${CMAKE_C_COMPILER} --version
            OUTPUT_VARIABLE MSVC_VERSION_OUTPUT
    )
    if(MSVC_VERSION_OUTPUT MATCHES ".*([0-9]+)\\.")
        string(REGEX MATCH "([0-9]+)" MSVC_VERSION_MATCH ${MSVC_VERSION_OUTPUT})
        if(${MSVC_VERSION_MATCH} LESS 19.36)
            message(FATAL_ERROR "CodeAnalyzer requires MSVC >= 19.36 (Visual Studio 2022 version 17.6+) for C++23")
        endif()
    endif()
else()
    message(FATAL_ERROR "❌ Unsupported compiler: ${CMAKE_C_COMPILER_ID}")
endif()

# ----------------------------
# ✅ Python.h header check for both C and C++
# ----------------------------
find_package(Python3 REQUIRED COMPONENTS Interpreter Development)

include_directories(${Python3_INCLUDE_DIRS})

# Check for Python.h for C and C++ compiler
include(CheckCCompilerFlag)
include(CheckCXXCompilerFlag)

check_c_compiler_flag("-I${Python3_INCLUDE_DIRS}" HAS_PYTHON_H_C)
check_cxx_compiler_flag("-I${Python3_INCLUDE_DIRS}" HAS_PYTHON_H_CPP)

if(NOT MSVC)
    if(NOT HAS_PYTHON_H_C)
        message(FATAL_ERROR "❌ Missing Python.h for C compiler. Please install python-dev / python-devel")
    endif()
endif()

if(NOT HAS_PYTHON_H_CPP)
    message(FATAL_ERROR "❌ Missing Python.h for C++ compiler. Please install python-dev / python-devel")
endif()

# ----------------------------
# ✅ Global compile arguments (both C and C++)
# ----------------------------
add_compile_options(-fno-strict-aliasing)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

# ----------------------------
# ✅ Clang-specific floating-point handling
# ----------------------------
if("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
    execute_process(
            COMMAND ${CMAKE_C_COMPILER} -ftrapping-math -o /dev/null -c /dev/null
            RESULT_VARIABLE clang_fp_result
    )
    if(clang_fp_result EQUAL 0)
        add_compile_options(-ftrapping-math -DNPY_HAVE_CLANG_FPSTRICT)
    else()
        message(WARNING "Clang does not fully support strict floating-point exceptions. Compatibility workaround enabled.")
        add_compile_options(-ftrapping-math -Wno-unsupported-floating-point-opt)
    endif()
endif()

# ----------------------------
# ✅ macOS-specific linker workaround
# ----------------------------
if(APPLE AND EXISTS "/usr/bin/clang")
    execute_process(
            COMMAND ${CMAKE_C_COMPILER} -Wl,-ld_classic -o /dev/null -c /dev/null
            RESULT_VARIABLE macos_link_check
    )
    if(NOT macos_link_check)
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-ld_classic")
    endif()
endif()

# ----------------------------
# ✅ Subdirectory structure
# ----------------------------
add_subdirectory(source)
add_subdirectory(temp)
