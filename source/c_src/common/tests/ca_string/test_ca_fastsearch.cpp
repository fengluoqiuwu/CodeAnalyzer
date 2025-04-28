// ================================
// CodeAnalyzer - source/c_src/common/tests/ca_string/test_ca_fastsearch.cpp
//
// @file
// @brief Tests ca fastsearch functions.
// ================================

#include "gtest/gtest.h"
#include "ca_string.h"

using namespace ca;
using namespace ca::ca_string;
using namespace ca::ca_string::fastsearch;

// ===============================
// Bloom Filter Tests
// ===============================

TEST(CaFastSearchTest, BloomFilter_CharTypeAddAndFind) {
    uint64_t mask = 0;
    constexpr ca_char_t ch = 'a';  // ASCII 97 → bit 33 (97 & 63)

    // Add character and verify it's found
    bloom_add(mask, ch);
    EXPECT_TRUE(bloom_find(mask, ch));

    // Check unrelated character is not found
    EXPECT_FALSE(bloom_find(mask, 'z'));  // ASCII 122 → bit 58
}

TEST(CaFastSearchTest, BloomFilter_Char4TypeAddAndFind) {
    uint64_t mask = 0;
    constexpr ca_char4_t ch = 0x12345678;  // Arbitrary 32-bit number → bit 56

    // Add char4-like value and verify it's found
    bloom_add(mask, ch);
    EXPECT_TRUE(bloom_find(mask, ch));

    // Check unrelated char4-like value is not found
    constexpr ca_char4_t other = 0x00000001;  // bit 1
    EXPECT_FALSE(bloom_find(mask, other));
}

TEST(CaFastSearchTest, BloomFilter_MultipleCharacters) {
    uint64_t mask = 0;
    bloom_add(mask, 'a');  // bit 33
    bloom_add(mask, 'b');  // bit 34
    bloom_add(mask, 'c');  // bit 35

    EXPECT_TRUE(bloom_find(mask, 'a'));
    EXPECT_TRUE(bloom_find(mask, 'b'));
    EXPECT_TRUE(bloom_find(mask, 'c'));
    EXPECT_FALSE(bloom_find(mask, 'z'));  // bit 58
}

TEST(CaFastSearchTest, BloomFilter_EdgeBits) {
    uint64_t mask = 0;
    bloom_add(mask, 0);              // bit 0
    bloom_add(mask, BLOOM_WIDTH - 1);  // bit 63

    EXPECT_TRUE(bloom_find(mask, 0));
    EXPECT_TRUE(bloom_find(mask, 63));
    EXPECT_FALSE(bloom_find(mask, 1));
}

// ===============================
// CheckedIndexer Tests
// ===============================

TEST(CaFastSearchTest, CheckedIndexer_DefaultConstructor) {
    const CheckedIndexer<ca_char_t, false> indexer1;
    EXPECT_FALSE(indexer1.is_reverse);
    EXPECT_EQ(indexer1.get_buffer(), nullptr);
    EXPECT_EQ(indexer1.get_length(), 0);


    const CheckedIndexer<ca_char_t, true> indexer2;
    EXPECT_TRUE(indexer2.is_reverse);
    EXPECT_EQ(indexer2.get_buffer(), nullptr);
    EXPECT_EQ(indexer2.get_length(), 0);
}

TEST(CaFastSearchTest, CheckedIndexer_Constructor) {
    auto* char_buf = new ca_char_t[10];
    auto* char4_buf = new ca_char4_t[10];

    const CheckedIndexer<ca_char_t, false> indexer1(char_buf, 10);
    EXPECT_EQ(indexer1.get_buffer(), char_buf);
    EXPECT_EQ(indexer1.get_length(), 10);

    const CheckedIndexer<ca_char4_t, false> indexer2(char4_buf, 10);
    EXPECT_EQ(indexer2.get_buffer(), char4_buf);
    EXPECT_EQ(indexer2.get_length(), 10);

    const CheckedIndexer<ca_char_t, true> indexer3(char_buf, 10);
    EXPECT_EQ(indexer3.get_buffer(), char_buf + 9);
    EXPECT_EQ(indexer3.get_length(), 10);

    const CheckedIndexer<ca_char4_t, true> indexer4(char4_buf, 10);
    EXPECT_EQ(indexer4.get_buffer(), char4_buf + 9);
    EXPECT_EQ(indexer4.get_length(), 10);

    delete[] char_buf;
    delete[] char4_buf;
}

TEST(CaFastSearchTest, CheckedIndexer_DereferenceOperator) {
    auto* char_buf = new ca_char_t[10];
    auto* char4_buf = new ca_char4_t[10];

    for (ca_size_t i = 0; i < 10; ++i) {
        char_buf[i] = static_cast<ca_char_t>(i);
        char4_buf[i] = static_cast<ca_char4_t>(i);
    }

    CheckedIndexer<ca_char_t, false> indexer1(char_buf, 10);
    CheckedIndexer<ca_char4_t, false> indexer2(char4_buf, 10);
    CheckedIndexer<ca_char_t, true> indexer3(char_buf, 10);
    CheckedIndexer<ca_char4_t, true> indexer4(char4_buf, 10);

    ASSERT_EQ(*indexer1, 0);
    ASSERT_EQ(*indexer2, 0);
    ASSERT_EQ(*indexer3, 9);
    ASSERT_EQ(*indexer4, 9);

    delete[] char_buf;
    delete[] char4_buf;
}


TEST(CaFastSearchTest, CheckedIndexer_IndexAccessOperator) {
    auto* char_buf = new ca_char_t[10];
    auto* char4_buf = new ca_char4_t[10];

    for (ca_size_t i = 0; i < 10; ++i) {
        char_buf[i] = static_cast<ca_char_t>(i);
        char4_buf[i] = static_cast<ca_char4_t>(i);
    }

    CheckedIndexer<ca_char_t, false> indexer1(char_buf, 10);
    CheckedIndexer<ca_char4_t, false> indexer2(char4_buf, 10);
    CheckedIndexer<ca_char_t, true> indexer3(char_buf, 10);
    CheckedIndexer<ca_char4_t, true> indexer4(char4_buf, 10);

    for (ca_size_t i = 0; i < 10; ++i) {
        ASSERT_EQ(static_cast<ca_size_t>(indexer1[i]), i);
        ASSERT_EQ(static_cast<ca_size_t>(indexer2[i]), i);
        ASSERT_EQ(static_cast<ca_size_t>(indexer3[i]), 9 - i);
        ASSERT_EQ(static_cast<ca_size_t>(indexer4[i]), 9 - i);
    }

    ASSERT_EQ(indexer1[10], 0);
    ASSERT_EQ(indexer2[10], 0);
    ASSERT_EQ(indexer3[10], 0);
    ASSERT_EQ(indexer4[10], 0);

    delete[] char_buf;
    delete[] char4_buf;
}

TEST(CaFastSearchTest, CheckedIndexer_ArithmeticOperators) {
    auto* char_buf = new ca_char_t[10];

    for (ca_size_t i = 0; i < 10; ++i) {
        char_buf[i] = static_cast<ca_char_t>(i);
    }

    CheckedIndexer<ca_char_t, false> indexer1(char_buf, 10);
    CheckedIndexer<ca_char_t, true> indexer2(char_buf, 10);
    ASSERT_EQ(*indexer1, 0);
    ASSERT_EQ(*indexer2, 9);

    const auto indexer3 = indexer1 + 4;
    const auto indexer4 = indexer2 + 4;

    ASSERT_EQ(indexer3 - indexer1, 4);
    ASSERT_EQ(indexer4 - indexer2, 4);

    ASSERT_EQ(*indexer1, 0);
    ASSERT_EQ(*indexer2, 9);
    ASSERT_EQ(*indexer3, 4);
    ASSERT_EQ(*indexer4, 5);

    ASSERT_EQ((indexer3 + 100).get_length(), 0);
    ASSERT_EQ((indexer4 + 100).get_length(), 0);

    ASSERT_EQ(*(indexer3 - 4), 0);
    ASSERT_EQ(*(indexer4 - 4), 9);

    delete[] char_buf;
}

TEST(CaFastSearchTest, CheckedIndexer_CompoundAssignmentOperators) {
    auto* char_buf = new ca_char_t[10];

    for (ca_size_t i = 0; i < 10; ++i) {
        char_buf[i] = static_cast<ca_char_t>(i);
    }

    CheckedIndexer<ca_char_t, false> indexer1(char_buf, 10);
    CheckedIndexer<ca_char_t, true> indexer2(char_buf, 10);
    ASSERT_EQ(*indexer1, 0);
    ASSERT_EQ(*indexer2, 9);

    indexer1 += 3;
    indexer2 += 3;
    ASSERT_EQ(*indexer1, 3);
    ASSERT_EQ(*indexer2, 6);

    indexer1 += 100;
    indexer2 += 100;
    ASSERT_EQ(indexer1.get_length(), 0);
    ASSERT_EQ(indexer2.get_length(), 0);

    indexer1 -= 1;
    indexer2 -= 1;
    ASSERT_EQ(*indexer1, 9);
    ASSERT_EQ(*indexer2, 0);

    delete[] char_buf;
}

TEST(CaFastSearchTest, CheckedIndexer_IncrementAndDecrementOperators) {
    auto* char_buf = new ca_char_t[10];

    for (ca_size_t i = 0; i < 10; ++i) {
        char_buf[i] = static_cast<ca_char_t>(i);
    }

    CheckedIndexer<ca_char_t, false> indexer1(char_buf, 10);
    CheckedIndexer<ca_char_t, true> indexer2(char_buf, 10);
    ASSERT_EQ(*indexer1, 0);
    ASSERT_EQ(*indexer2, 9);

    auto& indexer3 = ++indexer1;
    auto& indexer4 = ++indexer2;
    ASSERT_EQ(*indexer1, 1);
    ASSERT_EQ(*indexer2, 8);
    ASSERT_EQ(&indexer3, &indexer1);
    ASSERT_EQ(&indexer4, &indexer2);

    indexer3 = --indexer1;
    indexer4 = --indexer2;
    ASSERT_EQ(*indexer1, 0);
    ASSERT_EQ(*indexer2, 9);
    ASSERT_EQ(&indexer3, &indexer1);
    ASSERT_EQ(&indexer4, &indexer2);

    auto indexer5 = indexer1++;
    auto indexer6 = indexer2++;
    ASSERT_EQ(*indexer1, 1);
    ASSERT_EQ(*indexer2, 8);
    ASSERT_EQ(*indexer5, 0);
    ASSERT_EQ(*indexer6, 9);

    indexer5 = indexer1--;
    indexer6 = indexer2--;
    ASSERT_EQ(*indexer1, 0);
    ASSERT_EQ(*indexer2, 9);
    ASSERT_EQ(*indexer5, 1);
    ASSERT_EQ(*indexer6, 8);

    delete[] char_buf;
}

TEST(CaFastSearchTest, CheckedIndexer_CompareOperators) {
    auto* char_buf = new ca_char_t[10];

    for (ca_size_t i = 0; i < 10; ++i) {
        char_buf[i] = static_cast<ca_char_t>(i);
    }

    CheckedIndexer<ca_char_t, false> indexer1(char_buf, 10);
    CheckedIndexer<ca_char_t, true> indexer2(char_buf, 10);
    ASSERT_EQ(*indexer1, 0);
    ASSERT_EQ(*indexer2, 9);

    const auto indexer1_3 = indexer1 + 3;
    const auto indexer2_3 = indexer2 + 3;
    const auto indexer1_6 = indexer1 + 6;
    const auto indexer2_6 = indexer2 + 6;

    ASSERT_TRUE(indexer1_3 > indexer1);
    ASSERT_TRUE(indexer2_3 > indexer2);
    ASSERT_FALSE(indexer1_3 > indexer1_3);
    ASSERT_FALSE(indexer2_3 > indexer2_3);
    ASSERT_FALSE(indexer1_3 > indexer1_6);
    ASSERT_FALSE(indexer2_3 > indexer2_6);

    ASSERT_TRUE(indexer1_3 >= indexer1);
    ASSERT_TRUE(indexer2_3 >= indexer2);
    ASSERT_TRUE(indexer1_3 >= indexer1_3);
    ASSERT_TRUE(indexer2_3 >= indexer2_3);
    ASSERT_FALSE(indexer1_3 >= indexer1_6);
    ASSERT_FALSE(indexer2_3 >= indexer2_6);

    ASSERT_TRUE(indexer1_3 < indexer1_6);
    ASSERT_TRUE(indexer2_3 < indexer2_6);
    ASSERT_FALSE(indexer1_3 < indexer1_3);
    ASSERT_FALSE(indexer2_3 < indexer2_3);
    ASSERT_FALSE(indexer1_3 < indexer1);
    ASSERT_FALSE(indexer2_3 < indexer2);

    ASSERT_TRUE(indexer1_3 <= indexer1_6);
    ASSERT_TRUE(indexer2_3 <= indexer2_6);
    ASSERT_TRUE(indexer1_3 <= indexer1_3);
    ASSERT_TRUE(indexer2_3 <= indexer2_3);
    ASSERT_FALSE(indexer1_3 <= indexer1);
    ASSERT_FALSE(indexer2_3 <= indexer2);

    delete[] char_buf;
}

TEST(CaFastSearchTest, CheckedIndexer_Cmp) {
    auto* char_buf = new ca_char_t[10];

    for (ca_size_t i = 0; i < 10; ++i) {
        char_buf[i] = static_cast<ca_char_t>(i);
    }

    CheckedIndexer<ca_char_t, false> indexer1(char_buf, 10);
    CheckedIndexer<ca_char_t, true> indexer2(char_buf, 10);
    ASSERT_EQ(*indexer1, 0);
    ASSERT_EQ(*indexer2, 9);

    const auto indexer1_3 = indexer1 + 3;
    const auto indexer2_3 = indexer2 + 3;
    const auto indexer1_6 = indexer1 + 6;
    const auto indexer2_6 = indexer2 + 6;

    CheckedIndexer<ca_char_t, false> tmp1(char_buf, 5);
    CheckedIndexer<ca_char_t, true> tmp2(char_buf + 5, 5);
    ASSERT_TRUE(indexer1 == tmp1);
    ASSERT_TRUE(indexer2 == tmp2);
    ASSERT_FALSE(indexer1 == indexer1_3);
    ASSERT_FALSE(indexer2 == indexer2_3);

    ASSERT_TRUE(indexer1 != indexer1_6);
    ASSERT_TRUE(indexer2 != indexer2_6);
    ASSERT_FALSE(indexer1 != tmp1);
    ASSERT_FALSE(indexer2 != tmp2);

    ASSERT_EQ(indexer1_3.cmp(indexer1, 7), 1);
    ASSERT_EQ(indexer1_3.cmp(indexer1_3, 7), 0);
    ASSERT_EQ(indexer1_3.cmp(indexer1_6, 4), -1);
    ASSERT_EQ(indexer1.cmp(tmp1, 5), 0);
    ASSERT_EQ(indexer2_3.cmp(indexer2, 7), -1);
    ASSERT_EQ(indexer2_3.cmp(indexer2_3, 7), 0);
    ASSERT_EQ(indexer2_3.cmp(indexer2_6, 4), 1);
    ASSERT_EQ(indexer2.cmp(tmp2, 5), 0);

    delete[] char_buf;
}

// ===============================
// Find Char Tests
// ===============================

constexpr char article[6337] =
"Title: Bridging the Future: How Technology is Transforming Education\n"
"\n"
"In the 21st century, technology has become an inseparable part of everyday life, and its integration into education\n"
"has marked a profound shift in how knowledge is delivered, consumed, and understood. From smart classrooms to\n"
"AI-powered learning tools, the digital revolution is reshaping the educational landscape in ways that were unimaginable\n"
"just a few decades ago.\n"
"\n"
"The Digital Classroom: Learning Without Borders\n"
"\n"
"One of the most significant impacts of technology on education is the removal of physical and geographical barriers.\n"
"Virtual learning environments, online courses, and video conferencing tools have enabled students to learn from\n"
"anywhere in the world. During the COVID-19 pandemic, remote learning became a necessity, highlighting the crucial\n"
"role of technology in ensuring educational continuity.\n"
"\n"
"Platforms like Zoom, Microsoft Teams, and Google Classroom allowed schools to maintain a sense of normalcy, while\n"
"Learning Management Systems (LMS) such as Moodle and Canvas provided structured content delivery, grading, and\n"
"student feedback. These tools have not only become staples of modern education but have also opened up new models\n"
"of hybrid and asynchronous learning that give students greater flexibility and control over their learning experiences.\n"
"\n"
"Personalized Learning Through Artificial Intelligence\n"
"\n"
"Artificial Intelligence (AI) is revolutionizing education by enabling personalized learning experiences tailored to\n"
"individual students’ needs. AI-powered platforms can analyze student performance in real-time, identify strengths\n"
"and weaknesses, and provide customized resources to support improvement.\n"
"\n"
"For example, platforms like Khan Academy and Duolingo use adaptive learning algorithms that adjust difficulty levels\n"
"based on a student's progress. Similarly, intelligent tutoring systems can simulate one-on-one instruction, offering\n"
"hints, explanations, and feedback akin to a human tutor. This level of personalization was previously impossible in\n"
"traditional classrooms where one teacher had to cater to the needs of 20 to 30 students simultaneously.\n"
"\n"
"Gamification and Engagement\n"
"\n"
"Technology has also made learning more interactive and engaging through gamification—applying game design elements\n"
"in non-game contexts. Educational apps, simulations, and interactive quizzes turn learning into a more dynamic process,\n"
"making students active participants rather than passive receivers.\n"
"\n"
"For younger learners especially, platforms like Kahoot!, Quizizz, and Classcraft turn assessments into exciting challenges.\n"
"Virtual and Augmented Reality (VR/AR) technologies are also gaining traction. For example, VR can transport students to\n"
"historical events, the solar system, or inside the human body, providing immersive experiences that textbooks simply\n"
"cannot offer.\n"
"\n"
"Bridging Educational Inequality\n"
"\n"
"While technology can widen the digital divide, it also has the potential to bridge gaps in educational access. In underserved\n"
"regions, mobile learning and low-cost tablets are bringing educational resources to students who previously had none.\n"
"Organizations like One Laptop per Child and UNESCO have made strides in deploying educational technologies in developing countries.\n"
"\n"
"Open Educational Resources (OERs) also play a vital role. These are freely accessible, openly licensed text, media, and other\n"
"digital assets used for teaching, learning, and research. MIT OpenCourseWare and Khan Academy are prime examples of how free\n"
"content can democratize education globally.\n"
"\n"
"Teacher Empowerment and Professional Development\n"
"\n"
"Technology is not only transforming student experiences but also empowering educators. Teachers now have access to vast resources,\n"
"online communities, and professional development tools. Webinars, MOOCs (Massive Open Online Courses), and forums like Edmodo allow\n"
"teachers to continuously upgrade their skills and adopt best practices from around the world.\n"
"\n"
"Moreover, classroom management software enables teachers to monitor student activity, track attendance, and assess engagement,\n"
"allowing for data-driven decisions and timely interventions.\n"
"\n"
"Challenges and Considerations\n"
"\n"
"Despite its many benefits, the integration of technology in education is not without challenges. The digital divide remains a\n"
"major concern, as not all students have access to reliable internet or devices. Data privacy and cybersecurity are also pressing\n"
"issues, especially when dealing with minors' information.\n"
"\n"
"Additionally, the overuse of technology can lead to screen fatigue, reduced physical activity, and potential declines in social\n"
"interaction. Educators must strike a balance between leveraging technology and maintaining healthy learning environments.\n"
"\n"
"Another consideration is the risk of dehumanizing education. While AI and digital platforms are excellent tools, they cannot replace\n"
"the empathy, intuition, and mentorship that human teachers provide. Thus, technology should be viewed as a complement to, rather than\n"
"a replacement for, traditional teaching methods.\n"
"\n"
"The Road Ahead: A Hybrid Future\n"
"\n"
"Looking ahead, the future of education lies in hybrid models that combine the best of both digital and physical learning environments.\n"
"The goal is not to replace teachers or classrooms, but to enhance them—making education more inclusive, efficient, and adaptable.\n"
"\n"
"Emerging technologies like blockchain for credential verification, learning analytics for performance tracking, and natural language\n"
"processing for language learning will continue to evolve. Schools and policymakers must stay agile and proactive in integrating these\n"
"tools while ensuring ethical and equitable practices.\n"
"\n"
"Conclusion\n"
"\n"
"The convergence of technology and education is not just a trend—it is a paradigm shift that will define the future of learning.\n"
"As digital tools become more advanced and accessible, the potential to revolutionize education on a global scale grows exponentially.\n"
"\n"
"However, the true success of this transformation will depend on thoughtful implementation, ongoing support for educators, and a\n"
"commitment to equity. By embracing technology while maintaining the core human values of teaching and mentorship, we can build an\n"
"educational system that is not only smarter but also more compassionate and inclusive.\n";

constexpr int len = sizeof(article);
ca_char_t article_char[len];
ca_char2_t article_char2[len];
ca_char4_t article_char4[len];

int init_article(const char c[], ca_char_t* char_p, ca_char2_t* char2_p, ca_char4_t* char4_p) {
    for (int i = 0; i < len; ++i) {
        char_p[i] = c[i];
        char2_p[i] = static_cast<ca_char2_t>(char_p[i]);
        char4_p[i] = static_cast<ca_char4_t>(char_p[i]);
    }
    return 0;
}

int tmp = init_article(article, article_char, article_char2, article_char4);

// #define LOG_RUNNING_TIME

#ifndef LOG_RUNNING_TIME
#define LOG_RUNNING_TIME_UTILS(code, times)
#define TEST_RUNNING_TIME(test_suite_name, test_name) void ignore_test_##test_suite_name##_##test_name()
#else
#include <chrono>
#include <iostream>

#define LOG_RUNNING_TIME_UTILS(code, times) { \
    auto start = std::chrono::high_resolution_clock::now(); \
    for (int i = 0; i < times; ++i) { \
        code; \
        asm volatile("" ::"r"(tmp) : "memory"); \
    } \
    auto end = std::chrono::high_resolution_clock::now(); \
    std::chrono::duration<double, std::milli> duration = end - start; \
    std::cout << times << " times running time: " << duration.count() << " ms" << std::endl; \
}

#define TEST_RUNNING_TIME(test_suite_name, test_name) TEST(test_suite_name, test_name)
#endif

TEST(CaFastSearchTest, FindChar_ReturnValue) {
    const CheckedIndexer<ca_char_t, false> indexer1(article_char, len);
    const CheckedIndexer<ca_char2_t, false> indexer2(article_char2, len);
    const CheckedIndexer<ca_char4_t, false> indexer3(article_char4, len);
    ca_size_t tmp;

    constexpr int num_of_test = 5;
    constexpr char chars[] = {'c', 'a', '.', '\0', '\t'};
    constexpr ca_size_t indexes[] = {34, 48, 269, len-1, 0};
    constexpr bool results[] = {true, true, true, true, false};

    for (int i = 0; i < num_of_test; ++i) {
        bool found = find_char(indexer1, len, static_cast<ca_char_t>(chars[i]), &tmp);
        if (found) {EXPECT_EQ(tmp, indexes[i]);}
        EXPECT_EQ(found, results[i]);
        found = find_char(indexer2, len, static_cast<ca_char2_t>(chars[i]), &tmp);
        if (found) {EXPECT_EQ(tmp, indexes[i]);}
        EXPECT_EQ(found, results[i]);
        found = find_char(indexer3, len, static_cast<ca_char4_t>(chars[i]), &tmp);
        if (found) {EXPECT_EQ(tmp, indexes[i]);}
        EXPECT_EQ(found, results[i]);
    }
}

TEST_RUNNING_TIME(CaFastSearchTest, FindChar_RunningTime) {
    const CheckedIndexer<ca_char_t, false> indexer1(article_char, len);
    const CheckedIndexer<ca_char2_t, false> indexer2(article_char2, len);
    const CheckedIndexer<ca_char4_t, false> indexer3(article_char4, len);
    ca_size_t tmp = 0;

    std::cout << "####################RunningTime-FindChar####################" << std::endl;
    LOG_RUNNING_TIME_UTILS(find_char(indexer1, len, static_cast<ca_char_t>('c'), &tmp);, 100000)
    LOG_RUNNING_TIME_UTILS(find_char(indexer2, len, static_cast<ca_char2_t>('c'), &tmp);, 100000)
    LOG_RUNNING_TIME_UTILS(find_char(indexer3, len, static_cast<ca_char4_t>('c'), &tmp);, 100000)
    std::cout << "############################################################" << std::endl;
}

TEST(CaFastSearchTest, FindChar_AssertFailure) {
    const CheckedIndexer<ca_char_t, false> indexer1(article_char, len);

    EXPECT_DEATH({
        find_char(indexer1, len, static_cast<ca_char_t>('c'), nullptr);
    }, ".*");
}

TEST(CaFastSearchTest, RFindChar_ReturnValue) {
    const CheckedIndexer<ca_char_t, false> indexer1(article_char, len);
    const CheckedIndexer<ca_char2_t, false> indexer2(article_char2, len);
    const CheckedIndexer<ca_char4_t, false> indexer3(article_char4, len);
    ca_size_t tmp;

    constexpr int num_of_test = 5;
    constexpr char chars[] = {'j', 'a', '.', '\0', '\t'};
    constexpr ca_size_t indexes[] = {5777, len - 16, len-3, len-1, 0};
    constexpr bool results[] = {true, true, true, true, false};

    for (int i = 0; i < num_of_test; ++i) {
        bool found = rfind_char(indexer1, len, static_cast<ca_char_t>(chars[i]), &tmp);
        if (found) {EXPECT_EQ(tmp, indexes[i]);}
        EXPECT_EQ(found, results[i]);
        found = rfind_char(indexer2, len, static_cast<ca_char2_t>(chars[i]), &tmp);
        if (found) {EXPECT_EQ(tmp, indexes[i]);}
        EXPECT_EQ(found, results[i]);
        found = rfind_char(indexer3, len, static_cast<ca_char4_t>(chars[i]), &tmp);
        if (found) {EXPECT_EQ(tmp, indexes[i]);}
        EXPECT_EQ(found, results[i]);
    }
}

TEST_RUNNING_TIME(CaFastSearchTest, RFindChar_RunningTime) {
    const CheckedIndexer<ca_char_t, false> indexer1(article_char, len);
    const CheckedIndexer<ca_char2_t, false> indexer2(article_char2, len);
    const CheckedIndexer<ca_char4_t, false> indexer3(article_char4, len);
    ca_size_t tmp = 0;

    std::cout << "####################RunningTime-FindChar####################" << std::endl;
    LOG_RUNNING_TIME_UTILS(rfind_char(indexer1, len, static_cast<ca_char_t>('j'), &tmp);, 100000)
    LOG_RUNNING_TIME_UTILS(rfind_char(indexer2, len, static_cast<ca_char2_t>('j'), &tmp);, 100000)
    LOG_RUNNING_TIME_UTILS(rfind_char(indexer3, len, static_cast<ca_char4_t>('j'), &tmp);, 100000)
    std::cout << "############################################################" << std::endl;
}

TEST(CaFastSearchTest, RFindChar_AssertFailure) {
    const CheckedIndexer<ca_char_t, false> indexer1(article_char, len);

    EXPECT_DEATH({
        find_char(indexer1, len, static_cast<ca_char_t>('j'), nullptr);
    }, ".*");
}

TEST(CaFastSearchTest, CountChar_ReturnValue) {
    const CheckedIndexer<ca_char_t, false> indexer1(article_char, len);
    const CheckedIndexer<ca_char2_t, false> indexer2(article_char2, len);
    const CheckedIndexer<ca_char4_t, false> indexer3(article_char4, len);
    const CheckedIndexer<ca_char_t, true> indexer4(article_char, len);
    const CheckedIndexer<ca_char2_t, true> indexer5(article_char2, len);
    const CheckedIndexer<ca_char4_t, true> indexer6(article_char4, len);

    constexpr int num_of_test = 5;
    constexpr char chars[] = {'j', 'a', '.', '\0', '\t'};
    constexpr ca_size_t nums[] = {4, 100, 43, 1, 0};

    for (int i = 0; i < num_of_test; ++i) {
        ca_size_t tmp = count_char(indexer1, len, static_cast<ca_char_t>(chars[i]), 100);
        EXPECT_EQ(tmp, nums[i]);
        tmp = count_char(indexer2, len, static_cast<ca_char2_t>(chars[i]), 100);
        EXPECT_EQ(tmp, nums[i]);
        tmp = count_char(indexer3, len, static_cast<ca_char4_t>(chars[i]), 100);
        EXPECT_EQ(tmp, nums[i]);
        tmp = count_char(indexer4, len, static_cast<ca_char_t>(chars[i]), 100);
        EXPECT_EQ(tmp, nums[i]);
        tmp = count_char(indexer5, len, static_cast<ca_char2_t>(chars[i]), 100);
        EXPECT_EQ(tmp, nums[i]);
        tmp = count_char(indexer6, len, static_cast<ca_char4_t>(chars[i]), 100);
        EXPECT_EQ(tmp, nums[i]);
    }
}

TEST(CaFastSearchTest, LexSearch_ReturnValue) {
    char pattern1[] = "Bridging";
    char pattern2[] = "abcdabcabc";

    const CheckedIndexer<ca_char_t, false> indexer1(
        reinterpret_cast<ca_char_t*>(&pattern1[0]), 8
    );
    const CheckedIndexer<ca_char_t, false> indexer2(
        reinterpret_cast<ca_char_t*>(&pattern2[0]), 10
    );
    const CheckedIndexer<ca_char_t, true> indexer3(
        reinterpret_cast<ca_char_t*>(&pattern1[0]), 8
    );
    const CheckedIndexer<ca_char_t, true> indexer4(
        reinterpret_cast<ca_char_t*>(&pattern2[0]), 10
    );

    ca_size_t index = 0;
    ca_size_t period = 0;

    index = lex_search(indexer1, indexer1.get_length(), &period, false);
    EXPECT_EQ(index, 1);
    EXPECT_EQ(period, 7);
    index = lex_search(indexer1, indexer1.get_length(), &period, true);
    EXPECT_EQ(index, 0);
    EXPECT_EQ(period, 8);
    index = lex_search(indexer2, indexer2.get_length(), &period, false);
    EXPECT_EQ(index, 3);
    EXPECT_EQ(period, 7);
    index = lex_search(indexer2, indexer2.get_length(), &period, true);
    EXPECT_EQ(index, 4);
    EXPECT_EQ(period, 3);
    index = lex_search(indexer3, indexer3.get_length(), &period, false);
    EXPECT_EQ(index, 6);
    EXPECT_EQ(period, 2);
    index = lex_search(indexer3, indexer3.get_length(), &period, true);
    EXPECT_EQ(index, 7);
    EXPECT_EQ(period, 1);
    index = lex_search(indexer4, indexer4.get_length(), &period, false);
    EXPECT_EQ(index, 6);
    EXPECT_EQ(period, 4);
    index = lex_search(indexer4, indexer4.get_length(), &period, true);
    EXPECT_EQ(index, 2);
    EXPECT_EQ(period, 7);
}

TEST(CaFastSearchTest, Factorize_ReturnValue) {
    char pattern1[] = "Bridging";
    char pattern2[] = "abcdabcabc";

    const CheckedIndexer<ca_char_t, false> indexer1(
        reinterpret_cast<ca_char_t*>(&pattern1[0]), 8
    );
    const CheckedIndexer<ca_char_t, false> indexer2(
        reinterpret_cast<ca_char_t*>(&pattern2[0]), 10
    );
    const CheckedIndexer<ca_char_t, true> indexer3(
        reinterpret_cast<ca_char_t*>(&pattern1[0]), 8
    );
    const CheckedIndexer<ca_char_t, true> indexer4(
        reinterpret_cast<ca_char_t*>(&pattern2[0]), 10
    );

    ca_size_t index = 0;
    ca_size_t period = 0;

    index = factorize(indexer1, indexer1.get_length(), &period);
    EXPECT_EQ(index, 1);
    EXPECT_EQ(period, 7);
    index = factorize(indexer2, indexer2.get_length(), &period);
    EXPECT_EQ(index, 4);
    EXPECT_EQ(period, 3);
    index = factorize(indexer3, indexer3.get_length(), &period);
    EXPECT_EQ(index, 7);
    EXPECT_EQ(period, 1);
    index = factorize(indexer4, indexer4.get_length(), &period);
    EXPECT_EQ(index, 6);
    EXPECT_EQ(period, 4);
}

char pattern1[] = "Title";
char pattern2[] = ".";
char pattern3[] = "\n";
char pattern4[] = "For example, platforms like Khan Academy and Duolingo use adaptive learning algorithms";
char pattern5[] = "ment";
char pattern6[] = "nKSJjfiCnkjbKjkf";

char* patterns[] = {
    &pattern1[0], &pattern2[0], &pattern3[0],
    &pattern4[0], &pattern5[0], &pattern6[0]
};

constexpr ca_size_t lens[] = {
    5, 1, 1, sizeof("For example, platforms like Khan Academy and Duolingo use adaptive learning algorithms") - 1,
    4, sizeof("nKSJjfiCnkjbKjkf") - 1
};

constexpr ca_size_t indexes[] = {0, 269, 68, 1710, 631, 0};
constexpr ca_size_t reverse_index[] = {0, 6334, 6335, 1710, 6221, 0};
constexpr ca_size_t count[] = {1, 43, 90, 1, 20, 0};
constexpr bool result[] = {true, true, true, true, true, false};

TEST(CaFastSearchTest, TwoWayFind_ReturnValue) {
    const CheckedIndexer<ca_char_t, false> indexer(article_char, len - 1);
    const CheckedIndexer<ca_char_t, true> reverse_indexer(article_char, len);
    CheckedIndexer<ca_char_t, false> *pattern_indexers[6];
    CheckedIndexer<ca_char_t, true> *reverse_pattern_indexers[6];

    for (int i = 0; i < 6; i++) {
        pattern_indexers[i] = new CheckedIndexer<ca_char_t, false>(reinterpret_cast<ca_char_t*>(patterns[i]), lens[i]);
        reverse_pattern_indexers[i] = new CheckedIndexer<ca_char_t, true>(reinterpret_cast<ca_char_t*>(patterns[i]), lens[i]);
    }

    for (int i = 0; i < 6; ++i) {
        ca_size_t tmp = -1;
        bool found = two_way_find(indexer, indexer.get_length(), *pattern_indexers[i], pattern_indexers[i]->get_length(), &tmp);
        EXPECT_EQ(found, result[i]) << "no_reversed with i: " << i ;;
        if (found) {
            EXPECT_EQ(tmp, indexes[i]) << "no_reversed with i: " << i ;
        }

        found = two_way_find(reverse_indexer, reverse_indexer.get_length(), *reverse_pattern_indexers[i], reverse_pattern_indexers[i]->get_length(), &tmp);
        EXPECT_EQ(found, result[i]) << "reversed with i: " << i ;;
        if (found) {
            EXPECT_EQ(tmp, reverse_index[i]) << "reversed with i: " << i ;
        }
    }

    for (int i = 0; i < 6; i++) {
        delete[] pattern_indexers[i];
        delete[] reverse_pattern_indexers[i];
    }
}

TEST(CaFastSearchTest, TwoWayCount_ReturnValue) {
    const CheckedIndexer<ca_char_t, false> indexer(article_char, len - 1);
    const CheckedIndexer<ca_char_t, true> reverse_indexer(article_char, len);
    CheckedIndexer<ca_char_t, false> *pattern_indexers[6];
    CheckedIndexer<ca_char_t, true> *reverse_pattern_indexers[6];

    for (int i = 0; i < 6; i++) {
        pattern_indexers[i] = new CheckedIndexer<ca_char_t, false>(reinterpret_cast<ca_char_t*>(patterns[i]), lens[i]);
        reverse_pattern_indexers[i] = new CheckedIndexer<ca_char_t, true>(reinterpret_cast<ca_char_t*>(patterns[i]), lens[i]);
    }

    for (int i = 0; i < 6; ++i) {
        ca_size_t tmp = -1;
        tmp = two_way_count(indexer, indexer.get_length(), *pattern_indexers[i], pattern_indexers[i]->get_length(), 100);
        ASSERT_EQ(tmp, count[i]) << "no_reversed with i: " << i ;

        tmp = -1;
        tmp = two_way_count(reverse_indexer, reverse_indexer.get_length(), *reverse_pattern_indexers[i], reverse_pattern_indexers[i]->get_length(), 100);
        ASSERT_EQ(tmp, count[i]) << "reversed with i: " << i ;
    }

    for (int i = 0; i < 6; i++) {
        delete[] pattern_indexers[i];
        delete[] reverse_pattern_indexers[i];
    }
}

TEST(CaFastSearchTest, DefaultFind_ReturnValue) {
    const CheckedIndexer<ca_char_t, false> indexer(article_char, len - 1);
    const CheckedIndexer<ca_char_t, true> reverse_indexer(article_char, len);
    CheckedIndexer<ca_char_t, false> *pattern_indexers[6];
    CheckedIndexer<ca_char_t, true> *reverse_pattern_indexers[6];

    for (int i = 0; i < 6; i++) {
        pattern_indexers[i] = new CheckedIndexer<ca_char_t, false>(reinterpret_cast<ca_char_t*>(patterns[i]), lens[i]);
        reverse_pattern_indexers[i] = new CheckedIndexer<ca_char_t, true>(reinterpret_cast<ca_char_t*>(patterns[i]), lens[i]);
    }

    for (int i = 0; i < 6; ++i) {
        ca_size_t tmp = -1;
        bool found = default_find(indexer, indexer.get_length(), *pattern_indexers[i], pattern_indexers[i]->get_length(), &tmp);
        EXPECT_EQ(found, result[i]) << "no_reversed with i: " << i ;;
        if (found) {
            EXPECT_EQ(tmp, indexes[i]) << "no_reversed with i: " << i ;
        }

        found = default_find(reverse_indexer, reverse_indexer.get_length(), *reverse_pattern_indexers[i], reverse_pattern_indexers[i]->get_length(), &tmp);
        EXPECT_EQ(found, result[i]) << "reversed with i: " << i ;;
        if (found) {
            EXPECT_EQ(tmp, reverse_index[i]) << "reversed with i: " << i ;
        }
    }

    for (int i = 0; i < 6; i++) {
        delete[] pattern_indexers[i];
        delete[] reverse_pattern_indexers[i];
    }
}

TEST(CaFastSearchTest, DefaultCount_ReturnValue) {
    const CheckedIndexer<ca_char_t, false> indexer(article_char, len - 1);
    const CheckedIndexer<ca_char_t, true> reverse_indexer(article_char, len);
    CheckedIndexer<ca_char_t, false> *pattern_indexers[6];
    CheckedIndexer<ca_char_t, true> *reverse_pattern_indexers[6];

    for (int i = 0; i < 6; i++) {
        pattern_indexers[i] = new CheckedIndexer<ca_char_t, false>(reinterpret_cast<ca_char_t*>(patterns[i]), lens[i]);
        reverse_pattern_indexers[i] = new CheckedIndexer<ca_char_t, true>(reinterpret_cast<ca_char_t*>(patterns[i]), lens[i]);
    }

    for (int i = 0; i < 6; ++i) {
        ca_size_t tmp = -1;
        tmp = default_count(indexer, indexer.get_length(), *pattern_indexers[i], pattern_indexers[i]->get_length(), 100);
        ASSERT_EQ(tmp, count[i]) << "no_reversed with i: " << i ;

        tmp = -1;
        tmp = default_count(reverse_indexer, reverse_indexer.get_length(), *reverse_pattern_indexers[i], reverse_pattern_indexers[i]->get_length(), 100);
        ASSERT_EQ(tmp, count[i]) << "reversed with i: " << i ;
    }

    for (int i = 0; i < 6; i++) {
        delete[] pattern_indexers[i];
        delete[] reverse_pattern_indexers[i];
    }
}

TEST(CaFastSearchTest, AdaptiveFind_ReturnValue) {
    const CheckedIndexer<ca_char_t, false> indexer(article_char, len - 1);
    const CheckedIndexer<ca_char_t, true> reverse_indexer(article_char, len);
    CheckedIndexer<ca_char_t, false> *pattern_indexers[6];
    CheckedIndexer<ca_char_t, true> *reverse_pattern_indexers[6];

    for (int i = 0; i < 6; i++) {
        pattern_indexers[i] = new CheckedIndexer<ca_char_t, false>(reinterpret_cast<ca_char_t*>(patterns[i]), lens[i]);
        reverse_pattern_indexers[i] = new CheckedIndexer<ca_char_t, true>(reinterpret_cast<ca_char_t*>(patterns[i]), lens[i]);
    }

    for (int i = 0; i < 6; ++i) {
        ca_size_t tmp = -1;
        bool found = adaptive_find(indexer, indexer.get_length(), *pattern_indexers[i], pattern_indexers[i]->get_length(), &tmp);
        EXPECT_EQ(found, result[i]) << "no_reversed with i: " << i ;;
        if (found) {
            EXPECT_EQ(tmp, indexes[i]) << "no_reversed with i: " << i ;
        }

        found = adaptive_find(reverse_indexer, reverse_indexer.get_length(), *reverse_pattern_indexers[i], reverse_pattern_indexers[i]->get_length(), &tmp);
        EXPECT_EQ(found, result[i]) << "reversed with i: " << i ;;
        if (found) {
            EXPECT_EQ(tmp, reverse_index[i]) << "reversed with i: " << i ;
        }
    }

    for (int i = 0; i < 6; i++) {
        delete[] pattern_indexers[i];
        delete[] reverse_pattern_indexers[i];
    }
}

TEST(CaFastSearchTest, AdaptiveCount_ReturnValue) {
    const CheckedIndexer<ca_char_t, false> indexer(article_char, len - 1);
    const CheckedIndexer<ca_char_t, true> reverse_indexer(article_char, len);
    CheckedIndexer<ca_char_t, false> *pattern_indexers[6];
    CheckedIndexer<ca_char_t, true> *reverse_pattern_indexers[6];

    for (int i = 0; i < 6; i++) {
        pattern_indexers[i] = new CheckedIndexer<ca_char_t, false>(reinterpret_cast<ca_char_t*>(patterns[i]), lens[i]);
        reverse_pattern_indexers[i] = new CheckedIndexer<ca_char_t, true>(reinterpret_cast<ca_char_t*>(patterns[i]), lens[i]);
    }

    for (int i = 0; i < 6; ++i) {
        ca_size_t tmp = -1;
        tmp = adaptive_count(indexer, indexer.get_length(), *pattern_indexers[i], pattern_indexers[i]->get_length(), 100);
        ASSERT_EQ(tmp, count[i]) << "no_reversed with i: " << i ;

        tmp = -1;
        tmp = adaptive_count(reverse_indexer, reverse_indexer.get_length(), *reverse_pattern_indexers[i], reverse_pattern_indexers[i]->get_length(), 100);
        ASSERT_EQ(tmp, count[i]) << "reversed with i: " << i ;
    }

    for (int i = 0; i < 6; i++) {
        delete[] pattern_indexers[i];
        delete[] reverse_pattern_indexers[i];
    }
}
