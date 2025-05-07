# ================================
# CodeAnalyzer - source/c_src/common/tests/CMakeLists.txt
#
# Add tests to the common components
# ================================

set(COMMON_TEST_LISTS
        "ca_math"
        "ca_string:ca_math"
)

set(COMMON_TEST_PREFIX source-c_src-common)

ca_test(${COMMON_TEST_PREFIX} "${COMMON_TEST_LISTS}")