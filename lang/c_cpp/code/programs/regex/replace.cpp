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
