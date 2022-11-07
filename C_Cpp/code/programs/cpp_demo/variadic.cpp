#include <gtest/gtest.h>

#include <cstdarg>
#include <cstdio>

int message(std::string & output, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    const size_t BUFF_SIZE = 1024;
    char buffer[BUFF_SIZE] = {0};
    int ret = std::vsnprintf(buffer, BUFF_SIZE, fmt, args);
    if (ret < 0)
    {
        // vsnprintf() returns a negative value on error.
        return ret;
    }

    output.assign(buffer);
}

TEST(VariadicFunctionCallTest, Test)
{
    std::string msg;

    message(msg, "no argument");
    EXPECT_EQ(msg, "no argument");

    message(msg, "one argument: arg1=%d", 10);
    EXPECT_EQ(msg, "one argument: arg1=10");

    message(msg, "two arguments: arg1=%d, arg2=%s", 20, "21");
    EXPECT_EQ(msg, "two arguments: arg1=20, arg2=21");
}
