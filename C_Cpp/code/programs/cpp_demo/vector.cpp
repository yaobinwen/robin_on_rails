#include <stdexcept>
#include <vector>

#include <gtest/gtest.h>

TEST(Test_vector, test_constructor)
{
    {
        std::vector<int> empty_v;
        EXPECT_TRUE(empty_v.empty());
        EXPECT_TRUE(0U == empty_v.size());
        EXPECT_TRUE(0U == empty_v.capacity());
    }

    {
        std::vector<int> v(3U, 99);
        EXPECT_EQ(3U, v.size());

        for (size_t i = 0; i < v.size(); ++i)
        {
            EXPECT_EQ(99, v.at(i));
        }
    }

    {
        constexpr int default_int = int();
        std::vector<int> v(4U);
        EXPECT_EQ(4U, v.size());

        for (size_t i = 0; i < v.size(); ++i)
        {
            EXPECT_EQ(default_int, v.at(i));
        }
    }

    {
        std::vector<int> v1{1, 3, 5};
        std::vector<int> v2(v1.begin(), v1.end());

        EXPECT_EQ(v1.size(), v2.size());

        for (size_t i = 0; i < v1.size(); ++i)
        {
            EXPECT_EQ(v1.at(i), v2.at(i));
        }
    }
}

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
