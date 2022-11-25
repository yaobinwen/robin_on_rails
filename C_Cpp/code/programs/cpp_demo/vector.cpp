#include <stdexcept>
#include <vector>

#include <gtest/gtest.h>

TEST(Test_vector, test_front)
{
    std::vector<int> empty_v;
    if (!empty_v.empty())
    {
        // `empty_v` is empty. If calling `front()` directly, the behavior is
        // undefined, possibly segmentation fault.
        int i = empty_v.front();
        EXPECT_EQ(i, *empty_v.begin());
    }

    std::vector<int> v{1};
    if (!v.empty())
    {
        int i = v.front();
        EXPECT_EQ(i, *v.begin());
    }
}
