#include <gtest/gtest.h>

#include <memory>
#include <vector>

class Foo
{
};

TEST(SharedPtrTest, compare_raw_pointer_with_shared_ptr)
{
    std::vector<std::shared_ptr<Foo>> foos;
    for (int i = 0; i < 3; ++i)
    {
        foos.push_back(std::make_shared<Foo>());
    }
    EXPECT_EQ(foos.size(), 3U);

    Foo *p = foos.at(0).get();

    bool removed = false;
    for (auto iter = foos.begin(); iter != foos.end(); ++iter)
    {
        if (iter->get() == p)
        {
            foos.erase(iter);
            removed = true;
            break;
        }
    }
    EXPECT_TRUE(removed);
    EXPECT_EQ(foos.size(), 2U);
}
