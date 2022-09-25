#include <string_view>

#include <gtest/gtest.h>

TEST(Test_string_view, test_constructor)
{
    {
        // Default constructor.

        std::string_view v;
        EXPECT_TRUE(v.empty());
        EXPECT_EQ(v.data(), nullptr);
        EXPECT_EQ(v.size(), 0);
    }

    {
        // Copy constructor.

        std::string s("hello, world!");
        std::string_view v1(s);
        std::string_view v2(v1);
        EXPECT_FALSE(v2.empty());
        EXPECT_EQ(v2.data(), v1.data());
        EXPECT_EQ(v2.size(), v1.size());
        EXPECT_EQ(v2.size(), s.size());
    }

    {
        // Constructor from the first N chars of the string.

        std::string s("hello, world!");
        std::string_view v(s.data(), 5);
        EXPECT_EQ(v.size(), 5);
        EXPECT_EQ(v.compare("hello"), 0);   // `0` means `equal`
    }

    {
        // Constructor from a null-terminating string.

        const char s[] = "hello, world!";
        std::string_view v(s);
        EXPECT_EQ(v.size(), sizeof(s)-1);
        EXPECT_EQ(v.compare(s), 0);
    }
}

TEST(Test_string_view, test_life_time)
{
    // [1] says: "It is the programmer's responsibility to ensure that
    // std::string_view does not outlive the pointed-to character array."
    // [1]: https://en.cppreference.com/w/cpp/string/basic_string_view
    //
    // The following code shows that if an `std::string_view` object outlives
    // the string it points to, the content of `std::string_view` becomes no
    // longer reliable and any operation on it may cause unexpected results.

    std::string_view v;

    {
        std::string s("hello");

        // We let `v` points to `s`.
        v = s;
        // It is expected that `v` has the same size and content as `s`.
        EXPECT_EQ(v.size(), s.size());
        EXPECT_EQ(v.compare("hello"), 0);
    }

    // Now `s` has gone out of its lifecycle so it's destructed.

    // Create a new string object on the stack without modifying `v`.
    std::string s2("world!!!");

    // Now, `v` points at `s2` (partially). Because `v` was constructed from
    // `s` which had only 5 characters, `v`'s size is also 5, so it only
    // contains the first 5 characters of `s2`.
    EXPECT_EQ(v.size(), 5);
    EXPECT_EQ(v.compare("world"), 0);
}
