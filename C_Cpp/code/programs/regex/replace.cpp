#include <regex>
#include <string>

#include <gtest/gtest.h>

// See https://en.cppreference.com/w/cpp/regex/regex_replace

TEST(Test_regex_replace, overload_4)
{
    std::string str("  a b  c12`~!@#$%^&*()3d -=_+--__[]{}\\|  ,./<>?;':\"  ");

    std::regex re("[^a-zA-Z0-9-_]+");
    std::string ret = std::regex_replace(str, re, "-");
    EXPECT_EQ(ret, "-a-b-c12-3d---_---__-");

    re = "-+";
    ret = std::regex_replace(ret, re, "-");
    EXPECT_EQ(ret, "-a-b-c12-3d-_-__-");
}

TEST(Test_regex_match, capturing_group)
{
    std::string str("image000016.aux");

    std::regex re("^image([0-9]+)\\.aux$");
    std::smatch m;
    bool matching = std::regex_match(str, m, re);
    EXPECT_TRUE(matching);
    EXPECT_EQ(m.size(), 2U);

    // The first sub_match is the whole string.
    EXPECT_EQ(m[0], str);
    // The next sub_match is the first parenthesized expression.
    EXPECT_EQ(m[1], std::string("000016"));
}
