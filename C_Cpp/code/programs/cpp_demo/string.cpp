#include <stdexcept>
#include <string>

#include <gtest/gtest.h>

TEST(Test_string, test_constructor_NULL_logic_error)
{
    try
    {
        const char *null = nullptr;

        // Constructing an std::string from nullptr (for const CharT *) is
        // considered as a programming bug.
        std::string s(null);
    }
    catch (std::logic_error const &ex)
    {
        std::string msg(ex.what());
        EXPECT_TRUE(msg.find("null not valid") != std::string::npos);
    }
}
