// ================================
// CodeAnalyzer - source/c_src/common/tests/ca_string/test_ca_char.cpp
//
// @file
// @brief Tests ca char basic functions.
// ================================

#include "gtest/gtest.h"
#include "ca_string.h"

using namespace ca::ca_string;

// Unicode test characters
constexpr ca_char4_t CHAR_LATIN_SMALL_A = 'a';
constexpr ca_char4_t CHAR_LATIN_CAPITAL_A = 'A';
constexpr ca_char4_t CHAR_DIGIT_ONE = '1';
constexpr ca_char4_t CHAR_SPACE = ' ';
constexpr ca_char4_t CHAR_ARABIC_NUMERAL = 0x0669;    // ٩ (Arabic numeral nine)
constexpr ca_char4_t CHAR_ROMAN_NUMERAL_IV = 0x2163;  // Ⅳ
constexpr ca_char4_t CHAR_GREEK_ALPHA = 0x0391;       // Α (Greek capital alpha)
constexpr ca_char4_t CHAR_HIRAGANA_A = 0x3042;        // あ (Hiragana "a")
constexpr ca_char4_t CHAR_EN_DASH = 0x2013;           // – (punctuation)
constexpr ca_char4_t CHAR_TITLE_DZ = 0x01C5;          // ǅ
constexpr ca_char4_t CHAR_TITLE_LJ = 0x01C8;          // ǈ

// ===============================
// ca_getchar & ca_get_bytes
// ===============================

TEST(CaCharTest, GetChar_ASCII) {
    constexpr ca_char_t ascii[] = { 'A', '\0' };
    int size = 0;
    const auto result = ca_getchar<ca_encoding_t::CA_ENCODING_ASCII>(ascii, &size);
    EXPECT_EQ(result, 'A');
    EXPECT_EQ(size, 1);
}

TEST(CaCharTest, GetBytes_ASCII) {
    constexpr ca_char_t ascii[] = { 'B', '\0' };
    const int size = ca_get_bytes<ca_encoding_t::CA_ENCODING_ASCII>(ascii);
    EXPECT_EQ(size, 1);
}

TEST(CaCharTest, GetChar_UTF8) {
    constexpr ca_char_t utf8[4][5] = {
        { 0x41, '\0' },  // 'A'
        { 0xC3, 0xB1, '\0' }, // ñ
        { 0xE2, 0x82, 0xAC, '\0' }, // €
        { 0xF0, 0x9F, 0x90, 0x8D, '\0' } // 🐍
    };
    constexpr ca_char4_t expect_results[4] = { 0x41 , 0x00F1, 0x20AC, 0x1F40D};
    constexpr int expect_sizes[4] = { 1, 2, 3, 4 };
    int size = 0;

    for (int i = 0; i < 4; i++) {
        ca_char4_t c = ca_getchar<ca_encoding_t::CA_ENCODING_UTF8>(utf8[i], &size);
        EXPECT_EQ(expect_results[i], c);
        EXPECT_EQ(expect_sizes[i], size);
    }
}

TEST(CaCharTest, GetBytes_UTF8) {
    constexpr ca_char_t utf8[4][5] = {
        { 0x41, '\0' },  // 'A'
        { 0xC3, 0xB1, '\0' }, // ñ
        { 0xE2, 0x82, 0xAC, '\0' }, // €
        { 0xF0, 0x9F, 0x90, 0x8D, '\0' } // 🐍
    };
    constexpr int expect_results[4] = { 1, 2, 3, 4 };
    int size = 0;

    for (int i = 0; i < 4; i++) {
        size = ca_get_bytes<ca_encoding_t::CA_ENCODING_UTF8>(utf8[i]);
        EXPECT_EQ(expect_results[i], size);
    }
}

// UTF-32: Just extract the first 4 bytes as a code point
TEST(CaCharTest, GetChar_UTF32) {
    constexpr ca_char_t utf32_char[] = { 0x0D, 0xF4, 0x01, 0x00 }; // 0x1F40D (🐍) little endian
    int size = 0;
    const auto result = ca_getchar<ca_encoding_t::CA_ENCODING_UTF32>(utf32_char, &size);
    EXPECT_EQ(result, 0x1F40D);
    EXPECT_EQ(size, 4);
}

TEST(CaCharTest, GetBytes_UTF32) {
    constexpr ca_char_t utf32_char[] = { 0x0D, 0xF4, 0x01, 0x00 };
    const int size = ca_get_bytes<ca_encoding_t::CA_ENCODING_UTF32>(utf32_char);
    EXPECT_EQ(size, 4);
}

// ===============================
// ca_isalpha
// ===============================
TEST(CaCharTest, IsAlpha_True) {
    EXPECT_TRUE(ca_isalpha<ca_encoding_t::CA_ENCODING_ASCII>('B'));
    EXPECT_TRUE(ca_isalpha<ca_encoding_t::CA_ENCODING_UTF8>(CHAR_GREEK_ALPHA));
    EXPECT_TRUE(ca_isalpha<ca_encoding_t::CA_ENCODING_UTF32>(CHAR_HIRAGANA_A));
}

TEST(CaCharTest, IsAlpha_False) {
    EXPECT_FALSE(ca_isalpha<ca_encoding_t::CA_ENCODING_ASCII>('3'));
    EXPECT_FALSE(ca_isalpha<ca_encoding_t::CA_ENCODING_UTF8>(CHAR_DIGIT_ONE));
    EXPECT_FALSE(ca_isalpha<ca_encoding_t::CA_ENCODING_UTF32>(CHAR_SPACE));
}

// ===============================
// ca_isdigit
// ===============================
TEST(CaCharTest, IsDigit_True) {
    EXPECT_TRUE(ca_isdigit<ca_encoding_t::CA_ENCODING_ASCII>('5'));
    EXPECT_TRUE(ca_isdigit<ca_encoding_t::CA_ENCODING_UTF8>(CHAR_ARABIC_NUMERAL));
    EXPECT_TRUE(ca_isdigit<ca_encoding_t::CA_ENCODING_UTF32>('2'));
}

TEST(CaCharTest, IsDigit_False) {
    EXPECT_FALSE(ca_isdigit<ca_encoding_t::CA_ENCODING_UTF8>(CHAR_GREEK_ALPHA));
    EXPECT_FALSE(ca_isdigit<ca_encoding_t::CA_ENCODING_UTF32>(CHAR_EN_DASH));
}

// ===============================
// ca_isspace
// ===============================
TEST(CaCharTest, IsSpace_True) {
    EXPECT_TRUE(ca_isspace<ca_encoding_t::CA_ENCODING_ASCII>(' '));
    EXPECT_TRUE(ca_isspace<ca_encoding_t::CA_ENCODING_UTF8>('\t'));
    EXPECT_TRUE(ca_isspace<ca_encoding_t::CA_ENCODING_UTF32>('\n'));
}

TEST(CaCharTest, IsSpace_False) {
    EXPECT_FALSE(ca_isspace<ca_encoding_t::CA_ENCODING_UTF8>('Z'));
    EXPECT_FALSE(ca_isspace<ca_encoding_t::CA_ENCODING_UTF32>(CHAR_HIRAGANA_A));
}

// ===============================
// ca_isalnum
// ===============================
TEST(CaCharTest, IsAlnum_True) {
    EXPECT_TRUE(ca_isalnum<ca_encoding_t::CA_ENCODING_ASCII>('G'));
    EXPECT_TRUE(ca_isalnum<ca_encoding_t::CA_ENCODING_UTF8>('3'));
    EXPECT_TRUE(ca_isalnum<ca_encoding_t::CA_ENCODING_UTF32>(CHAR_GREEK_ALPHA));
}

TEST(CaCharTest, IsAlnum_False) {
    EXPECT_FALSE(ca_isalnum<ca_encoding_t::CA_ENCODING_UTF8>(CHAR_EN_DASH));
    EXPECT_FALSE(ca_isalnum<ca_encoding_t::CA_ENCODING_UTF32>('!'));
}

// ===============================
// ca_islower
// ===============================
TEST(CaCharTest, IsLower_True) {
    EXPECT_TRUE(ca_islower<ca_encoding_t::CA_ENCODING_ASCII>('z'));
    EXPECT_TRUE(ca_islower<ca_encoding_t::CA_ENCODING_UTF8>('m'));
}

TEST(CaCharTest, IsLower_False) {
    EXPECT_FALSE(ca_islower<ca_encoding_t::CA_ENCODING_UTF8>('Q'));
    EXPECT_FALSE(ca_islower<ca_encoding_t::CA_ENCODING_UTF32>('1'));
}

// ===============================
// ca_isupper
// ===============================
TEST(CaCharTest, IsUpper_True) {
    EXPECT_TRUE(ca_isupper<ca_encoding_t::CA_ENCODING_ASCII>('H'));
    EXPECT_TRUE(ca_isupper<ca_encoding_t::CA_ENCODING_UTF8>('D'));
    EXPECT_TRUE(ca_isupper<ca_encoding_t::CA_ENCODING_UTF32>(CHAR_GREEK_ALPHA));
}

TEST(CaCharTest, IsUpper_False) {
    EXPECT_FALSE(ca_isupper<ca_encoding_t::CA_ENCODING_UTF8>('f'));
    EXPECT_FALSE(ca_isupper<ca_encoding_t::CA_ENCODING_UTF32>(CHAR_ROMAN_NUMERAL_IV));
}

// ===============================
// ca_istitle
// ===============================
TEST(CaCharTest, IsTitle_True) {
    EXPECT_TRUE(ca_istitle<ca_encoding_t::CA_ENCODING_UTF8>(CHAR_TITLE_LJ));
    EXPECT_TRUE(ca_istitle<ca_encoding_t::CA_ENCODING_UTF32>(CHAR_TITLE_DZ));
}

TEST(CaCharTest, IsTitle_False) {
    EXPECT_FALSE(ca_istitle<ca_encoding_t::CA_ENCODING_UTF8>('g'));
    EXPECT_FALSE(ca_istitle<ca_encoding_t::CA_ENCODING_UTF32>('2'));
}

// ===============================
// ca_isnumeric
// ===============================
TEST(CaCharTest, IsNumeric_True) {
    EXPECT_TRUE(ca_isnumeric<ca_encoding_t::CA_ENCODING_ASCII>('7'));
    EXPECT_TRUE(ca_isnumeric<ca_encoding_t::CA_ENCODING_UTF8>('0'));
    EXPECT_TRUE(ca_isnumeric<ca_encoding_t::CA_ENCODING_UTF32>(CHAR_ROMAN_NUMERAL_IV));
}

TEST(CaCharTest, IsNumeric_False) {
    EXPECT_FALSE(ca_isnumeric<ca_encoding_t::CA_ENCODING_UTF8>(CHAR_HIRAGANA_A));
}

// ===============================
// ca_isdecimal
// ===============================
TEST(CaCharTest, IsDecimal_True) {
    EXPECT_TRUE(ca_isdecimal<ca_encoding_t::CA_ENCODING_ASCII>('1'));
    EXPECT_TRUE(ca_isdecimal<ca_encoding_t::CA_ENCODING_UTF8>('2'));
    EXPECT_TRUE(ca_isdecimal<ca_encoding_t::CA_ENCODING_UTF32>('9'));
}

TEST(CaCharTest, IsDecimal_False) {
    EXPECT_FALSE(ca_isdecimal<ca_encoding_t::CA_ENCODING_UTF32>(CHAR_ROMAN_NUMERAL_IV));
    EXPECT_FALSE(ca_isdecimal<ca_encoding_t::CA_ENCODING_UTF32>(CHAR_EN_DASH));
}

// ===============================
// ca_char_check dispatcher tests
// ===============================
TEST(CaCharTest, CharCheckDispatcher_ValidCases) {
    EXPECT_TRUE((ca_char_check<ca_encoding_t::CA_ENCODING_UTF8, ca_char_check_types::CA_ISALPHA>('A')));
    EXPECT_TRUE((ca_char_check<ca_encoding_t::CA_ENCODING_UTF32, ca_char_check_types::CA_ISDIGIT>('3')));
    EXPECT_FALSE((ca_char_check<ca_encoding_t::CA_ENCODING_UTF8, ca_char_check_types::CA_ISSPACE>('X')));
}