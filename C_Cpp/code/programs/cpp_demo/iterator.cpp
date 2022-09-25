#include <gtest/gtest.h>

#include <iterator>

TEST(IteratorTest, distance) {
    std::map<int, int> dict;
    for (int i = 0; i < 10; ++i)
    {
        dict[i] = i;
    }

    auto iter1 = dict.find(6);
    auto iter2 = dict.find(7);
    EXPECT_FALSE(iter1 == dict.end());
    EXPECT_FALSE(iter2 == dict.end());

    std::size_t d1 = std::distance(iter1, iter2);

    std::size_t d2 = 0;
    for (auto iter = iter1; iter != iter2; ++iter, ++d2);

    EXPECT_EQ(d1, 1);
    EXPECT_EQ(d1, d2);

    std::size_t d3 = std::distance(iter2, dict.end());

    std::size_t d4 = 0;
    for (auto iter = iter2; iter != dict.end(); ++iter, ++d4);

    EXPECT_EQ(d3, 3);
    EXPECT_EQ(d3, d4);
}
