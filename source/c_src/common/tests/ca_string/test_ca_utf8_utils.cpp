// ================================
// CodeAnalyzer - source/c_src/common/tests/ca_string/test_ca_utf8_utils.cpp
//
// @file
// @brief Tests utf8 utility functions.
// ================================

#include <gtest/gtest.h>
#include "ca_string.h"

using namespace ca;
using namespace ca::ca_string;
using namespace ca::ca_string::utf8;

struct CharacterTestCase {
    const ca_char_t* utf8_bytes;
    ca_size_t size;
    ca_char4_t utf32_code;

    // Constructor for easy initialization
    CharacterTestCase(const char* bytes, const int s, const ca_char4_t code)
        : size(s), utf32_code(code) {
        utf8_bytes = reinterpret_cast<const ca_char_t *>(bytes);
    }
};

CharacterTestCase character_test_case[] = {
    { R"(A)",                  1, 0x00000041 }, // A
    { "\xC3\xA9",              2, 0x000000E9 }, // é
    { "\xC2\xA2",              2, 0x000000A2 }, // ¢
    { "\xE2\x82\xAC",          3, 0x000020AC }, // €
    { "\xE6\x96\x87",          3, 0x00006587 }, // 文
    { "\xF0\x9F\x98\x8A",      4, 0x0001F60A }, // 😊
    { "\xF0\x90\x8D\x88",      4, 0x00010348 }, // 𐍈
    { R"(~)",                  1, 0x0000007E }, // ~
    { "\xE2\x82\xBF",          3, 0x000020BF }, // ₿
    { "\xF4\x8F\xBF\xBF",      4, 0x0010FFFF }  // max valid Unicode
};

TEST(TestCaUtf8Utils, Test_Utf8CharToUcs4CodeWithoutCheck_ReturnValue) {
    for (const auto [utf8_bytes, size, utf32_code] : character_test_case) {
        ca_char4_t code;
        int result = utf8_char_to_ucs4_code_without_check(utf8_bytes, &code);
        EXPECT_EQ(result, size)
            << "utf8_char_to_ucs4_code_without_check(utf8_bytes, &code)" << std::endl
            << " for utf8_bytes: " << utf8_bytes
            << " expected output size: " << size
            << " but got: " << result;
        EXPECT_EQ(code, utf32_code)
            << "utf8_char_to_ucs4_code_without_check(utf8_bytes, &code)" << std::endl
            << "for utf8_bytes: " << utf8_bytes
            << " expected utf32 code: 0x" << std::hex << utf32_code
            << " but got: 0x" << std::hex << code;
    }
}

TEST(TestCaUtf8Utils, Test_Utf8CharToUcs4CodeWithoutCheck_AssertIvalidInput) {
    const auto* utf8_bytes = reinterpret_cast<const ca_char_t*>(R"(A)");
    ca_char4_t utf32_code;

    EXPECT_DEATH({
        utf8_char_to_ucs4_code_without_check(utf8_bytes, nullptr);
    }, ".*")
        << "utf8_char_to_ucs4_code_without_check(utf8_bytes, nullptr)" << std::endl
        << "Expected assertion failure for nullptr `code` pointer,"
        << " but got no assertion failure.";

    EXPECT_DEATH({
        utf8_char_to_ucs4_code_without_check(nullptr, &utf32_code);
    }, ".*")
        << "utf8_char_to_ucs4_code_without_check(nullptr, &utf32_code)" << std::endl
        << "Expected assertion failure for nullptr `c` pointer,"
        << " but got no assertion failure.";
}

TEST(TestCaUtf8Utils, Test_Ucs4CodeToUtf8CharWithoutCheck_ReturnValue) {
    for (const auto [utf8_bytes, size, utf32_code] : character_test_case) {
        ca_char_t c[4];
        int result = ucs4_code_to_utf8_char_without_check(utf32_code, c);
        EXPECT_EQ(result, size)
            << "ucs4_code_to_utf8_char_without_check(utf32_code, c)" << std::endl
            << "for utf32_code: 0x" << std::hex << utf32_code
            << " expected output utf8 size: " << size
            << " but got: " << result;
        for (int i = 0; i < size; ++i) {
            EXPECT_EQ(c[i], utf8_bytes[i])
                << "ucs4_code_to_utf8_char_without_check(utf32_code, c)" << std::endl
                << "Mismatch at byte index  " << i
                << " for utf32_code: 0x" << std::hex << utf32_code
                << " expected output utf8 code(in bytes): 0x" << std::hex << utf8_bytes[i]
                << " but got: 0x" << std::hex << c[i];
        }
    }
}

TEST(TestCaUtf8Utils, Test_Ucs4CodeToUtf8CharWithoutCheck_AssertIvalidInput) {
    constexpr ca_char4_t utf32_code = 0x0010FFFF;

    EXPECT_DEATH({
        ucs4_code_to_utf8_char_without_check(utf32_code, nullptr);
    }, ".*")
        << "ucs4_code_to_utf8_char_without_check(utf32_code, nullptr)" << std::endl
        << "Expected assertion failure for nullptr `c` pointer,"
        << " but got no assertion failure.";
}

TEST(TestCaUtf8Utils, Test_NumUtf8BytesForUtf8CharacterWithoutCheck_ReturnValue) {
    for (const auto [utf8_bytes, size, utf32_code] : character_test_case) {
        int result = num_utf8_bytes_for_utf8_character_without_check(utf8_bytes);
        EXPECT_EQ(result, size)
            << "num_utf8_bytes_for_utf8_character_without_check(utf8_bytes)" << std::endl
            << "for utf8_bytes: " << utf8_bytes
            << " expected output size: " << size
            << " but got: " << result;
    }
}

TEST(TestCaUtf8Utils, Test_NumUtf8BytesForUtf8CharacterWithoutCheck_InvalidCodepoint) {
    constexpr char tmp[6][4] = {"\xC2", "\xE2\x82", "\xF0\x9F\x98", "\xF0", "\xE0", "\xC0"};
    const int size[6] = {2, 3, 4, 4, 3, 2};

    for (int i = 0; i < 6; ++i) {
        const auto* utf8_bytes = reinterpret_cast<const ca_char_t*>(tmp + i);
        int result = num_utf8_bytes_for_utf8_character_without_check(utf8_bytes);
        EXPECT_EQ(result, size[i])
            << "num_utf8_bytes_for_utf8_character_without_check(utf8_bytes)" << std::endl
            << "for utf8_bytes: " << utf8_bytes
            << " expected output size: " << size[i]
            << " but got: " << result;
    }
}

TEST(TestCaUtf8Utils, Test_NumUtf8BytesForUtf8CharacterWithoutCheck_AssertIvalidInput) {
    EXPECT_DEATH({
        num_utf8_bytes_for_utf8_character_without_check(nullptr);
    }, ".*")
        << "num_utf8_bytes_for_utf8_character_without_check(nullptr)" << std::endl
        << "Expected assertion failure for nullptr `c` pointer,"
        << " but got no assertion failure.";
}

TEST(TestCaUtf8Utils, Test_NumUtf8BytesForCodepoint_ReturnValue) {
    for (const auto [utf8_bytes, size, utf32_code] : character_test_case) {
        int result = num_utf8_bytes_for_codepoint(utf32_code);
        EXPECT_EQ(result, size)
            << "num_utf8_bytes_for_codepoint(utf32_code)" << std::endl
            << "for utf32_code: 0x" << std::hex << utf32_code
            << " expected output size: " << size
            << " but got: " << result;
    }
}

TEST(TestCaUtf8Utils, Test_NumUtf8BytesForCodepoint_InvalidCodepoint) {
    ca_char4_t invalid_codes[] = {
        0xD800, 0xDFFF, 0x110000, 0xFFFFFFFE, 0xFFFFFFFF
    };

    for (const auto invalid_code : invalid_codes) {
        int result = num_utf8_bytes_for_codepoint(invalid_code);
        EXPECT_EQ(result, -1)
            << "num_utf8_bytes_for_codepoint(invalid_code)" << std::endl
            << "for utf32_code: 0x" << std::hex << invalid_code
            << " expected output value (invalid output): " << -1
            << " but got: " << result;
    }
}


struct StringTestCase {
    const ca_char_t* utf8_string;
    ca_size_t size;
    const ca_char4_t* utf32_code;
    ca_size_t num_codepoints;

    // Constructor for easy initialization
    StringTestCase(const char* str, const int s, const ca_char4_t* code, const ca_size_t n)
        : size(s), utf32_code(code), num_codepoints(n) {
        utf8_string = reinterpret_cast<const ca_char_t *>(str);
    }
};

const ca_char4_t utf32_mixed[] = {
    0x41,        // A
    0xE9,        // é
    0xA2,        // ¢
    0x20AC,      // €
    0x6587,      // 文
    0x1F60A,     // 😊
    0x10348,     // 𐍈
    0x7E,        // ~
    0x20BF,      // ₿
    0x10FFFF     // max unicode
};

constexpr ca_char4_t utf32_empty[] = {};

constexpr ca_char4_t utf32_ascii[] = {
    'H', 'e', 'l', 'l', 'o', 'W', 'o', 'r', 'l', 'd'
};

constexpr ca_char4_t utf32_2byte[] = {
    0xA2, 0xA2, 0xA2  // ¢ ¢ ¢
};

constexpr ca_char4_t utf32_3byte[] = {
    0x20AC, 0x20AC  // € €
};

constexpr ca_char4_t utf32_4byte[] = {
    0x1F60A, 0x10348  // 😊 𐍈
};

constexpr ca_char4_t utf32_max[] = {
    0x10FFFF
};

StringTestCase string_test_case[] = {
    {
        "A\xC3\xA9\xC2\xA2\xE2\x82\xAC\xE6\x96\x87"
        "\xF0\x9F\x98\x8A\xF0\x90\x8D\x88~\xE2\x82\xBF"
        "\xF4\x8F\xBF\xBF",
        27, utf32_mixed, 10
    },
    {
        "",
        0, utf32_empty, 0
    },
    {
        "HelloWorld",
        10, utf32_ascii, 10
    },
    {
        "\xC2\xA2\xC2\xA2\xC2\xA2",  // ¢ ¢ ¢
        6, utf32_2byte, 3
    },
    {
        "\xE2\x82\xAC\xE2\x82\xAC",  // € €
        6, utf32_3byte, 2
    },
    {
        "\xF0\x9F\x98\x8A\xF0\x90\x8D\x88",  // 😊 𐍈
        8, utf32_4byte, 2
    },
    {
        "\xF4\x8F\xBF\xBF",  // max Unicode (U+10FFFF)
        4, utf32_max, 1
    }
};

TEST(TestCaUtf8Utils, Test_NumCodepointsForUtf8BytesWithoutCheck_ReturnValue) {
    for (const auto& test_case : string_test_case) {
        ca_size_t num_codepoints = 0;
        num_codepoints_for_utf8_bytes_without_check(test_case.utf8_string, test_case.size, &num_codepoints);
        EXPECT_EQ(num_codepoints, test_case.num_codepoints)
            << "num_codepoints_for_utf8_bytes_without_check(test_case.utf8_string, test_case.size, &num_codepoints)" << std::endl
            << "for utf8_string: " << test_case.utf8_string
            << " expected num_codepoints: " << test_case.num_codepoints
            << " but got: " << num_codepoints;
    }
}

TEST(TestCaUtf8Utils, Test_NumCodepointsForUtf8BytesWithoutCheck_AssertInvalidInput) {
    const auto utf8_string = "A\xC3\xA9\xC2\xA2\xE2\x82\xAC\xE6\x96\x87"
                          "\xF0\x9F\x98\x8A\xF0\x90\x8D\x88~\xE2\x82\xBF"
                          "\xF4\x8F\xBF\xBF";
    const auto* str = reinterpret_cast<const ca_char_t*>(utf8_string);
    size_t num_codepoints;

    EXPECT_DEATH({
        num_codepoints_for_utf8_bytes_without_check(nullptr, 27, &num_codepoints);
    }, ".*")
        << "num_codepoints_for_utf8_bytes_without_check(nullptr, 27, &num_codepoints)" << std::endl
        << "Expected assertion failure for nullptr `buf` pointer,"
        << " but got no assertion failure.";

    EXPECT_DEATH({
        num_codepoints_for_utf8_bytes_without_check(str, 27, nullptr);
    }, ".*")
        << "num_codepoints_for_utf8_bytes_without_check(str, 27, nullptr)" << std::endl
        << "Expected assertion failure for nullptr `num_codepoints` pointer,"
        << " but got no assertion failure.";
}

TEST(TestCaUtf8Utils, Test_NumCodepointsForUtf8Bytes_ReturnValue) {
    for (const auto& test_case : string_test_case) {
        ca_size_t num_codepoints = 0;
        int result = num_codepoints_for_utf8_bytes(test_case.utf8_string, test_case.size, &num_codepoints);
        ASSERT_EQ(result, 0)
            << "num_codepoints_for_utf8_bytes(utf8_string, size, &num_codepoints)" << std::endl
            << "for utf8_string: " << test_case.utf8_string
            << " expected output result: " << 0
            << " but got: " << result;
        ASSERT_EQ(num_codepoints, test_case.num_codepoints)
            << "num_codepoints_for_utf8_bytes(utf8_string, size, &num_codepoints)" << std::endl
            << "for utf8_string: " << test_case.utf8_string
            << " expected output num_codepoints: " << test_case.num_codepoints
            << " but got: " << num_codepoints;
    }
}

TEST(TestCaUtf8Utils, Test_NumCodepointsForUtf8Bytes_EmptyInput) {
    const auto* str = reinterpret_cast<const ca_char_t*>(""); // empty string
    ca_size_t num_codepoints = 123; // preset non-zero
    const int result = num_codepoints_for_utf8_bytes(str, 0, &num_codepoints);

    EXPECT_EQ(result, 0)
        << "num_codepoints_for_utf8_bytes(str, 0, &num_codepoints)" << std::endl
        << "for str: " << str
        << " expected output result: " << 0
        << " but got: " << result;
    EXPECT_EQ(num_codepoints, 0)
        << "num_codepoints_for_utf8_bytes(str, 0, &num_codepoints)" << std::endl
        << "for str: " << str
        << " expected output num_codepoints: " << 0
        << " but got: " << num_codepoints;
}

TEST(TestCaUtf8Utils, Test_NumCodepointsForUtf8Bytes_Invalid_BadStartByte) {
    constexpr unsigned char invalid_utf8[] = { 0xFF, 0xA0, 0x00 }; // invalid leading byte
    const auto str = reinterpret_cast<const ca_char_t *>(invalid_utf8);
    ca_size_t num_codepoints = 0;

    const int result = num_codepoints_for_utf8_bytes(str, 2, &num_codepoints);
    EXPECT_EQ(result, -1)
        << "num_codepoints_for_utf8_bytes(str, 2, &num_codepoints)" << std::endl
        << "for str with bad start byte, "
        << " expected output result: " << -1
        << " but got: " << result;
}

TEST(TestCaUtf8Utils, Test_NumCodepointsForUtf8Bytes_Invalid_BadContinuation) {
    constexpr unsigned char invalid_utf8[] = { 0xE2, 0x28, 0xA1, 0x00 }; // 0x28 not a valid continuation
    const auto str = reinterpret_cast<const ca_char_t *>(invalid_utf8);
    ca_size_t num_codepoints = 0;

    const int result = num_codepoints_for_utf8_bytes(str, 3, &num_codepoints);
    EXPECT_EQ(result, -1)
        << "num_codepoints_for_utf8_bytes(str, 2, &num_codepoints)" << std::endl
        << "for str with bad continuation byte, "
        << " expected output result: " << -1
        << " but got: " << result;
}

TEST(TestCaUtf8Utils, Test_NumCodepointsForUtf8Bytes_OverlongEncoding) {
    constexpr unsigned char overlong[] = { 0xC0, 0xAF, 0x00 }; // overlong encoding for '/'
    const auto str = reinterpret_cast<const ca_char_t *>(overlong);
    ca_size_t num_codepoints = 0;

    const int result = num_codepoints_for_utf8_bytes(str, 2, &num_codepoints);
    EXPECT_EQ(result, -1)
        << "num_codepoints_for_utf8_bytes(str, 2, &num_codepoints)" << std::endl
        << "for str with bad overlong encoding byte, "
        << " expected output result: " << -1
        << " but got: " << result;
}

TEST(TestCaUtf8Utils, Test_NumCodepointsForUtf8Bytes_AssertInvalidInput) {
    const auto utf8_string = "A\xC3\xA9\xC2\xA2\xE2\x82\xAC\xE6\x96\x87"
                          "\xF0\x9F\x98\x8A\xF0\x90\x8D\x88~\xE2\x82\xBF"
                          "\xF4\x8F\xBF\xBF";
    const auto* str = reinterpret_cast<const ca_char_t*>(utf8_string);
    size_t num_codepoints;

    EXPECT_DEATH({
        num_codepoints_for_utf8_bytes(nullptr, 27, &num_codepoints);
    }, ".*")
        << "num_codepoints_for_utf8_bytes(nullptr, 27, &num_codepoints)" << std::endl
        << "Expected assertion failure for nullptr `buf` pointer,"
        << " but got no assertion failure.";

    EXPECT_DEATH({
        num_codepoints_for_utf8_bytes(str, 27, nullptr);
    }, ".*")
        << "num_codepoints_for_utf8_bytes(str, 27, nullptr)" << std::endl
        << "Expected assertion failure for nullptr `num_codepoints` pointer,"
        << " but got no assertion failure.";
}

TEST(TestCaUtf8Utils, Test_Utf8BufferSize_ReturnValue) {
    for (const auto& test_case : string_test_case) {
        ca_size_t utf8_bytes = 0;
        int result = utf8_buffer_size(test_case.utf8_string, test_case.size, &utf8_bytes);
        ASSERT_EQ(result, 0)
            << "utf8_buffer_size(utf8_string, size, &utf8_bytes)" << std::endl
            << "for utf8_string: " << test_case.utf8_string
            << " expected output result: " << 0
            << " but got: " << result;
        ASSERT_EQ(utf8_bytes, test_case.size)
            << "utf8_buffer_size(utf8_string, size, &utf8_bytes)" << std::endl
            << "for utf8_string: " << test_case.utf8_string
            << " expected output utf8_bytes: " << test_case.size
            << " but got: " << utf8_bytes;
    }
}

TEST(TestCaUtf8Utils, Test_Utf8BufferSize_EmptyInput) {
    const auto* str = reinterpret_cast<const ca_char_t*>(""); // empty string
    ca_size_t utf8_bytes = 123; // preset non-zero
    const int result = utf8_buffer_size(str, 0, &utf8_bytes);

    EXPECT_EQ(result, 0)
        << "utf8_buffer_size(str, 0, &utf8_bytes)" << std::endl
        << "for str: " << str
        << " expected output result: " << 0
        << " but got: " << result;
    EXPECT_EQ(utf8_bytes, 0)
        << "utf8_buffer_size(str, 0, &utf8_bytes)" << std::endl
        << "for str: " << str
        << " expected output utf8_bytes: " << 0
        << " but got: " << utf8_bytes;
}

TEST(TestCaUtf8Utils, Test_Utf8BufferSize_Invalid_BadStartByte) {
    constexpr unsigned char invalid_utf8[] = { 0xFF, 0xA0, 0x00 }; // invalid leading byte
    const auto str = reinterpret_cast<const ca_char_t *>(invalid_utf8);
    ca_size_t utf8_bytes = 0;

    const int result = utf8_buffer_size(str, 2, &utf8_bytes);
    EXPECT_EQ(result, -1)
        << "utf8_buffer_size(str, 2, &utf8_bytes)" << std::endl
        << "for str with bad start byte, "
        << " expected output result: " << -1
        << " but got: " << result;
}

TEST(TestCaUtf8Utils, Test_Utf8BufferSize_Invalid_BadContinuation) {
    constexpr unsigned char invalid_utf8[] = { 0xE2, 0x28, 0xA1, 0x00 }; // 0x28 not a valid continuation
    const auto str = reinterpret_cast<const ca_char_t *>(invalid_utf8);
    ca_size_t utf8_bytes = 0;

    const int result = utf8_buffer_size(str, 3, &utf8_bytes);
    EXPECT_EQ(result, -1)
        << "utf8_buffer_size(str, 2, &utf8_bytes)" << std::endl
        << "for str with bad continuation byte, "
        << " expected output result: " << -1
        << " but got: " << result;
}

TEST(TestCaUtf8Utils, Test_Utf8BufferSize_OverlongEncoding) {
    constexpr unsigned char overlong[] = { 0xC0, 0xAF, 0x00 }; // overlong encoding for '/'
    const auto str = reinterpret_cast<const ca_char_t *>(overlong);
    ca_size_t utf8_bytes = 0;

    const int result = utf8_buffer_size(str, 2, &utf8_bytes);
    EXPECT_EQ(result, -1)
        << "utf8_buffer_size(str, 2, &utf8_bytes)" << std::endl
        << "for str with bad overlong encoding byte, "
        << " expected output result: " << -1
        << " but got: " << result;
}

TEST(TestCaUtf8Utils, Test_Utf8BufferSize_AssertInvalidInput) {
    const auto utf8_string = "A\xC3\xA9\xC2\xA2\xE2\x82\xAC\xE6\x96\x87"
                          "\xF0\x9F\x98\x8A\xF0\x90\x8D\x88~\xE2\x82\xBF"
                          "\xF4\x8F\xBF\xBF";
    const auto* str = reinterpret_cast<const ca_char_t*>(utf8_string);
    size_t utf8_bytes;

    EXPECT_DEATH({
        utf8_buffer_size(nullptr, 27, &utf8_bytes);
    }, ".*")
        << "utf8_buffer_size(nullptr, 27, &utf8_bytes)" << std::endl
        << "Expected assertion failure for nullptr `buf` pointer,"
        << " but got no assertion failure.";

    EXPECT_DEATH({
        utf8_buffer_size(str, 27, nullptr);
    }, ".*")
        << "utf8_buffer_size(str, 27, nullptr)" << std::endl
        << "Expected assertion failure for nullptr `utf8_bytes` pointer,"
        << " but got no assertion failure.";
}

TEST(TestCaUtf8Utils, Test_Utf8SizeOfUtf32BufferEncode_ReturnValue) {
    for (const auto& test_case : string_test_case) {
        ca_size_t num_codepoints = 0;
        ca_size_t utf8_bytes = 0;
        int result = utf8_size_of_utf32_buffer_encode(test_case.utf32_code, test_case.num_codepoints, &num_codepoints, &utf8_bytes);
        ASSERT_EQ(result, 0)
            << "utf8_size_of_utf32_buffer_encode(utf8_string, size, &num_codepoints, &utf8_bytes)" << std::endl
            << "for utf32 code: " << test_case.utf32_code
            << " expected output result: " << 0
            << " but got: " << result;
        ASSERT_EQ(num_codepoints, test_case.num_codepoints)
            << "utf8_size_of_utf32_buffer_encode(utf8_string, size, &num_codepoints, &utf8_bytes)" << std::endl
            << "for utf32 code: " << test_case.utf32_code
            << " expected output num_codepoints: " << test_case.num_codepoints
            << " but got: " << num_codepoints;
        ASSERT_EQ(utf8_bytes, test_case.size)
            << "utf8_size_of_utf32_buffer_encode(utf8_string, size, &num_codepoints, &utf8_bytes)" << std::endl
            << "for utf32 code: " << test_case.utf32_code
            << " expected output utf8_bytes: " << test_case.size
            << " but got: " << utf8_bytes;
    }
}

TEST(TestCaUtf8Utils, Test_Utf8SizeOfUtf32BufferEncode_EmptyInput) {
    const auto* str = reinterpret_cast<const ca_char4_t*>(""); // empty string
    ca_size_t num_codepoints = 123;
    ca_size_t utf8_bytes = 123; // preset non-zero
    const int result = utf8_size_of_utf32_buffer_encode(str, 0, &num_codepoints, &utf8_bytes);

    EXPECT_EQ(result, 0)
        << "utf8_size_of_utf32_buffer_encode(str, 0, &num_codepoints, &utf8_bytes)" << std::endl
        << "for str: " << str
        << " expected output result: " << 0
        << " but got: " << result;
    EXPECT_EQ(num_codepoints, 0)
        << "utf8_size_of_utf32_buffer_encode(str, 0, &num_codepoints, &utf8_bytes)" << std::endl
        << "for str: " << str
        << " expected output num_codepoints: " << 0
        << " but got: " << num_codepoints;
    EXPECT_EQ(utf8_bytes, 0)
        << "utf8_size_of_utf32_buffer_encode(str, 0, &num_codepoints, &utf8_bytes)" << std::endl
        << "for str: " << str
        << " expected output utf8_bytes: " << 0
        << " but got: " << utf8_bytes;
}

TEST(TestCaUtf8Utils, Test_Utf8SizeOfUtf32BufferEncode_InvalidCodepoints_ReturnsMinusOne) {
    constexpr ca_char4_t invalid_codepoints_1[] = { 0x110000 }; // out of range
    constexpr ca_char4_t invalid_codepoints_2[] = { 0xD800 };   // surrogate pair begin
    constexpr ca_char4_t invalid_codepoints_3[] = { 0xDFFF };   // surrogate pair end
    constexpr ca_char4_t invalid_codepoints_4[] = { 0xFFFFFFFF }; // invalid
    constexpr ca_char4_t invalid_codepoints_5[] = { 0x0041, 0xD800, 0x0042 }; // middle invalid surrogate

    struct Utf32TestCase {
        const ca_char4_t* utf32_code;
        ca_size_t num_codepoints;
    };

    Utf32TestCase invalid_cases[] = {
        { invalid_codepoints_1, 1 },
        { invalid_codepoints_2, 1 },
        { invalid_codepoints_3, 1 },
        { invalid_codepoints_4, 1 },
        { invalid_codepoints_5, 3 },
    };

    for (const auto& test_case : invalid_cases) {
        ca_size_t num_codepoints = 0;
        ca_size_t utf8_bytes = 0;

        int result = utf8_size_of_utf32_buffer_encode(
            test_case.utf32_code,
            test_case.num_codepoints,
            &num_codepoints,
            &utf8_bytes);

        ASSERT_EQ(result, -1)
            << "utf8_size_of_utf32_buffer_encode(utf8_string, size, &num_codepoints, &utf8_bytes)" << std::endl
            << "for invalid utf32 code: " << test_case.utf32_code
            << " expected output result: " << -1
            << " but got: " << result;
        ASSERT_EQ(num_codepoints, 0)
            << "utf8_size_of_utf32_buffer_encode(utf8_string, size, &num_codepoints, &utf8_bytes)" << std::endl
            << "for invalid utf32 code: " << test_case.utf32_code
            << " expected output num_codepoints: " << 0
            << " but got: " << num_codepoints;
        ASSERT_EQ(utf8_bytes, 0)
            << "utf8_size_of_utf32_buffer_encode(utf8_string, size, &num_codepoints, &utf8_bytes)" << std::endl
            << "for invalid utf32 code: " << test_case.utf32_code
            << " expected output utf8_bytes: " << 0
            << " but got: " << utf8_bytes;
    }
}

TEST(TestCaUtf8Utils, Test_Utf8SizeOfUtf32BufferEncode_AssertInvalidInput) {

    const auto* str = utf32_mixed;
    size_t num_codepoints;
    size_t utf8_bytes;

    EXPECT_DEATH({
        utf8_size_of_utf32_buffer_encode(nullptr, 27, &num_codepoints, &utf8_bytes);
    }, ".*")
        << "utf8_size_of_utf32_buffer_encode(nullptr, 27, &num_codepoints, &utf8_bytes)" << std::endl
        << "Expected assertion failure for nullptr `buf_usc4` pointer,"
        << " but got no assertion failure.";

    EXPECT_DEATH({
        utf8_size_of_utf32_buffer_encode(str, 27, nullptr, &utf8_bytes);
    }, ".*")
        << "utf8_size_of_utf32_buffer_encode(str, 27, nullptr, &utf8_bytes)" << std::endl
        << "Expected assertion failure for nullptr `num_codepoints` pointer,"
        << " but got no assertion failure.";

    EXPECT_DEATH({
        utf8_size_of_utf32_buffer_encode(str, 27, &num_codepoints, nullptr);
    }, ".*")
        << "utf8_size_of_utf32_buffer_encode(str, 27, &num_codepoints, nullptr)" << std::endl
        << "Expected assertion failure for nullptr `utf8_bytes` pointer,"
        << " but got no assertion failure.";
}

ca_char_t* utf8_char_offset(ca_char_t* str, const int char_index) {
    ca_char_t* ptr = str;
    int count = 0;
    while (*ptr && count < char_index) {
        if ((*ptr & 0xC0) != 0x80) {
            ++count;
        }
        ++ptr;
    }
    --ptr;
    return ptr;
}

TEST(TestCaUtf8Utils, Test_FindPreviousUtf8Character_ReturnValue) {
    // UTF-8 string: "A¢€𐍈" = [0x41, 0xC2 0xA2, 0xE2 0x82 0xAC, 0xF0 0x90 0x8D 0x88]
    const auto* str = u8"A¢€𐍈";
    auto* utf8 = const_cast<ca_char_t *>(reinterpret_cast<const ca_char_t*>(str));

    // Map character index to expected starting byte pointer
    struct TestCase {
        int char_index;       // Starting character index
        ca_size_t nchar_back; // Characters to move back
        int expected_index;   // Expected byte index (or -1 for null result)
    };

    std::vector<TestCase> test_cases = {
        {4, 1, 3},  // From '𐍈' back 1 → '€'
        {4, 2, 2},  // From '𐍈' back 2 → '¢'
        {4, 3, 1},  // From '𐍈' back 3 → 'A'
        {3, 1, 2},  // From '€' back 1 → '¢'
    };

    for (const auto& test : test_cases) {
        ca_char_t* current_ptr = utf8_char_offset(utf8, test.char_index);
        ca_char_t* previous = nullptr;

        find_previous_utf8_character(current_ptr, test.nchar_back, &previous);

        ca_char_t* expected_ptr = utf8_char_offset(utf8, test.expected_index);
        ASSERT_EQ(previous, expected_ptr)
            << "find_previous_utf8_character(current_ptr, nchar_back, &previous)" << std::endl
            << "for current_ptr: " << current_ptr
            << " nchar_back: " << test.nchar_back
            << " expected output previous: " << expected_ptr
            << " but got: " << previous;
    }
}

TEST(TestCaUtf8Utils, Test_FindPreviousUtf8Character_AssertInvalidInput) {
    const auto* str = u8"A¢€𐍈";
    auto* utf8 = const_cast<ca_char_t *>(reinterpret_cast<const ca_char_t*>(str)) + 5;
    ca_char_t* previous = nullptr;

    ASSERT_DEATH({
        find_previous_utf8_character(nullptr, 0, &previous);
    }, ".*")
        << "find_previous_utf8_character(nullptr, 0, &previous)" << std::endl
        << "Expected assertion failure for nullptr `current` pointer,"
        << " but got no assertion failure.";

    ASSERT_DEATH({
        find_previous_utf8_character(utf8, 1, nullptr);
    }, ".*")
        << "find_previous_utf8_character(utf8, 1, nullptr)" << std::endl
        << "Expected assertion failure for nullptr `previous_loc` pointer,"
        << " but got no assertion failure.";
}

TEST(TestCaUtf8Utils, Test_FindStartEndLocs_ReturnValue) {
    const auto* str = u8"A¢€𐍈";
    auto* utf8 = const_cast<ca_char_t *>(reinterpret_cast<const ca_char_t*>(str));

    struct TestCase {
        ca_size_t start_index;
        ca_size_t end_index;
        int expected_start_byte; // -1 means nullptr expected
        int expected_end_byte;
    };

    std::vector<TestCase> test_cases = {
        {0, 1, 0, 1},    // Start at 'A', end at '¢'
        {1, 2, 1, 3},    // Start at '¢', end at '€'
        {2, 3, 3, 6},    // Start at '€', end at '𐍈'
        {3, 4, 6, -1},   // End index out of range → end_loc = nullptr, return -1
        {4, 5, -1, -1},  // Both out of range
        {0, 0, 0, 0}     // Both point to start
    };

    for (const auto& test : test_cases) {
        constexpr ca_size_t buffer_size = 10;
        ca_char_t* start_loc = nullptr;
        ca_char_t* end_loc = nullptr;

        int result = find_start_end_locs(utf8, buffer_size,
                                         test.start_index, test.end_index,
                                         &start_loc, &end_loc);

        const bool start_valid = test.expected_start_byte != -1;
        const bool end_valid = test.expected_end_byte != -1;

        int expected_result = (start_valid && end_valid) ? 0 : -1;
        ASSERT_EQ(result, expected_result)
            << "find_start_end_locs(buf, size, start_index, end_index, &start_loc, &end_loc)" << std::endl
            << "start_index: " << test.start_index
            << ", end_index: " << test.end_index
            << ", expected return: " << expected_result
            << ", actual: " << result;

        if (start_valid) {
            ASSERT_EQ(start_loc, utf8 + test.expected_start_byte)
                << "find_start_end_locs(buf, size, start_index, end_index, &start_loc, &end_loc)" << std::endl
                << "for start_index: " << test.start_index
                << ", end_index: " << test.end_index
                << ", expected return start_loc at byte offset " << utf8 + test.expected_start_byte
                << ", actual: " << start_loc;
        } else {
            ASSERT_EQ(start_loc, nullptr)
                << "find_start_end_locs(buf, size, start_index, end_index, &start_loc, &end_loc)" << std::endl
                << "for start_index: " << test.start_index
                << ", end_index: " << test.end_index
                << ", expected return start_loc to be nullptr for out-of-bounds start_index"
                << ", actual: " << start_loc;
        }

        if (end_valid) {
            ASSERT_EQ(end_loc, utf8 + test.expected_end_byte)
                << "find_start_end_locs(buf, size, start_index, end_index, &start_loc, &end_loc)" << std::endl
                << "for start_index: " << test.start_index
                << ", end_index: " << test.end_index
                << ", expected return end_loc at byte offset " << utf8 + test.expected_end_byte
                << ", actual: " << end_loc;
        } else {
            ASSERT_EQ(end_loc, nullptr)
                << "find_start_end_locs(buf, size, start_index, end_index, &start_loc, &end_loc)" << std::endl
                << "for start_index: " << test.start_index
                << ", end_index: " << test.end_index
                << ", expected return end_loc to be nullptr for out-of-bounds end_index"
                << ", actual: " << end_loc;
        }
    }
}

TEST(TestCaUtf8Utils, Test_FindStartEndLocs_AssertInvalidInput) {
    const auto* str = u8"A¢€𐍈";
    auto* utf8 = const_cast<ca_char_t *>(reinterpret_cast<const ca_char_t*>(str));
    constexpr ca_size_t buffer_size = 10;
    ca_char_t* start = nullptr;
    ca_char_t* end = nullptr;

    ASSERT_DEATH({
        find_start_end_locs(nullptr, buffer_size, 0, 1, &start, &end);
    }, ".*")
        << "find_start_end_locs(nullptr, buffer_size, 0, 1, &start, &end)" << std::endl
        << "Expected assertion failure for nullptr `buf` pointer,"
        << " but got no assertion failure.";

    ASSERT_DEATH({
        find_start_end_locs(utf8, buffer_size, 0, 1, nullptr, &end);
    }, ".*")
        << "find_start_end_locs(utf8, buffer_size, 0, 1, nullptr, &end)" << std::endl
        << "Expected assertion failure for nullptr `start_loc` pointer,"
        << " but got no assertion failure.";

    ASSERT_DEATH({
        find_start_end_locs(utf8, buffer_size, 0, 1, &start, nullptr);
    }, ".*")
        << "find_start_end_locs(utf8, buffer_size, 0, 1, &start, nullptr)" << std::endl
        << "Expected assertion failure for nullptr `end_loc` pointer,"
        << " but got no assertion failure.";
}