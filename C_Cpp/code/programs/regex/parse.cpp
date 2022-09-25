#include <regex>
#include <string>

#include <boost/format.hpp>
#include <gtest/gtest.h>

// See https://en.cppreference.com/w/cpp/regex/regex_match

TEST(Test_regex_parse, test_find_key_value)
{
    std::string key("machine");
    std::string value("yaobin.mvs.io");
    std::string line(str(boost::format("%1% %2%") % key % value));

    std::regex re("^([a-zA-Z]+) (.+)$");
    std::smatch m;
    bool matching = std::regex_match(line, m, re);

    // Sub-match m[0] is the whole string. m[1] is the first capture group.
    EXPECT_TRUE(matching);
    EXPECT_EQ(m[1], key);
    EXPECT_EQ(m[2], value);
}

TEST(Test_regex_parse, test_mismatching)
{
    std::string key("machine");
    std::string line(str(boost::format("%1% ") % key));

    std::regex re("^machine (.+)$");
    std::smatch m;
    bool matching = std::regex_match(line, m, re);

    // Sub-match m[0] is the whole string. m[1] is the first capture group.
    EXPECT_FALSE(matching);
}
