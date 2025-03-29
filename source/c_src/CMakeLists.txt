# ================================
# CodeAnalyzer - source/c_src/CMakeLists.txt
#
# Main entry for common analysis engine and components
# ================================

# Add subdirectories for analyzers and common components
#add_subdirectory(analyzers)
add_subdirectory(common)

# Add subdirectories for tests

set(C_SRC_TEST_PREFIX source-c_src)

ca_test_combine(${C_SRC_TEST_PREFIX})