#include <gtest/gtest.h>

#include <chrono>

#include <iostream>

TEST(ChronoTest, test_milliseconds)
{
    using namespace std::chrono;

    milliseconds ms = duration_cast< milliseconds >(
        system_clock::now().time_since_epoch()
    );

    EXPECT_EQ(ms.count(), 0);
}
