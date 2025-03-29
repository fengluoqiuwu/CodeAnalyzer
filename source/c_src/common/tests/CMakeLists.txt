# ================================
# CodeAnalyzer - source/c_src/common/tests/CMakeLists.txt
#
# Add tests to the common components
# ================================

set(COMMON_TEST_LISTS
        ca_math
        ca_string
)

set(COMMON_TEST_PREFIX source-c_src-common)

foreach (TEST_NAME ${COMMON_TEST_LISTS})
    ca_test_dir(${COMMON_TEST_PREFIX}-${TEST_NAME} ${TEST_NAME} ${TEST_NAME})
endforeach ()

ca_test_combine(${COMMON_TEST_PREFIX})