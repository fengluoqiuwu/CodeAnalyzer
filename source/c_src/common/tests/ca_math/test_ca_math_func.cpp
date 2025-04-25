// ================================
// CodeAnalyzer - source/c_src/common/tests/ca_math/test_ca_math_func.cpp
//
// @file
// @brief Tests core mathematical functions.
// ================================

#include <gtest/gtest.h>
#include <limits>
#include "ca_math.h"

using namespace ca;
using namespace ca::ca_math;

/* Test ca_max with same types (basic behavior) */
TEST(CaMathFuncTest, Test_CaMax_ReturnValue) {
    EXPECT_EQ(ca_max(3, 5), 5);
    EXPECT_EQ(ca_max(10, 2), 10);
    EXPECT_EQ(ca_max(-3, -5), -3);
    EXPECT_EQ(ca_max(7.5, 7.5), 7.5);
}

/* Test ca_min with same types (basic behavior) */
TEST(CaMathFuncTest, Test_CaMin_ReturnValue) {
    EXPECT_EQ(ca_min(3, 5), 3);
    EXPECT_EQ(ca_min(10, 2), 2);
    EXPECT_EQ(ca_min(-3, -5), -5);
    EXPECT_EQ(ca_min(7.5, 7.5), 7.5);
}

/* Test ca_max with different types (int + float) */
TEST(CaMathFuncTest, Test_CaMax_MixedType) {
    constexpr auto result = ca_max<float>(3, 5.5f);  // explicit float to unify template type
    EXPECT_FLOAT_EQ(result, 5.5f);

    constexpr auto result2 = ca_max<double>(10.0, 2);  // int converted to double
    EXPECT_DOUBLE_EQ(result2, 10.0);
}

/* Test ca_min with different types (int + float) */
TEST(CaMathFuncTest, Test_CaMin_MixedType) {
    constexpr auto result = ca_min<float>(3, 5.5f);  // template type unified to float
    EXPECT_FLOAT_EQ(result, 3.0f);

    constexpr auto result2 = ca_min<double>(10.0, 2);  // template type unified to double
    EXPECT_DOUBLE_EQ(result2, 2.0);
}

/* Static assertions: ensure type constraints work as expected */
TEST(CaMathFuncTest, Test_CaMax_TypeConstraints) {
    static_assert(std::is_same_v<decltype(ca_max(1, 2)), int>);
    static_assert(std::is_same_v<decltype(ca_max(1.1, 2.2)), double>);
}

/* Static assertions: ensure type constraints work as expected */
TEST(CaMathFuncTest, Test_CaMin_TypeConstraints) {
    static_assert(std::is_same_v<decltype(ca_min(1, 2)), int>);
    static_assert(std::is_same_v<decltype(ca_min(1.1, 2.2)), double>);
}

/* Test safe downcast that stays within range */
TEST(CaMathFuncTest, Test_CaSafeDowncast_ReturnValue) {
    constexpr ca_int32_t wide_val = 123;
    constexpr auto casted_val = ca_safe_downcast<ca_int8_t>(wide_val);
    EXPECT_EQ(casted_val, 123);

    constexpr uint32_t u_val = 255;
    constexpr auto u_casted_val = ca_safe_downcast<uint8_t>(u_val);
    EXPECT_EQ(u_casted_val, 255);
}

/* Test that values within range do not trigger assertions */
TEST(CaMathFuncTest, Test_CaSafeDowncast_AssertInRange) {
    EXPECT_NO_FATAL_FAILURE({
        constexpr int val = 127;
        [[maybe_unused]] auto result = ca_safe_downcast<ca_int8_t>(val);
    });

    EXPECT_NO_FATAL_FAILURE({
        constexpr int val = std::numeric_limits<ca_int16_t>::max();
        [[maybe_unused]] auto result = ca_safe_downcast<ca_int16_t>(val);
    });
}

/* Test that out-of-range values correctly trigger assertion failure in debug mode */
TEST(CaMathFuncTest, Test_CaSafeDowncast_AssertOutOfRange) {
    EXPECT_DEATH({
        constexpr int val = 99999;
        [[maybe_unused]] auto result = ca_safe_downcast<ca_int8_t>(val);
    }, ".*");
}


