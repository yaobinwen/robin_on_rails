#include <type_traits>

#include <gtest/gtest.h>

struct C_default
{
    // Empty. Everything uses the default implementation.
};

TEST(Test_type_traits, test_is_move_assignable)
{
    {
        // C_default

        // Use the class static member `value` directly.
        EXPECT_TRUE(std::is_move_assignable<C_default>::value);

        // Create an object to test the trait.
        std::is_move_assignable<C_default> ima;
        EXPECT_TRUE(ima.value);
        EXPECT_TRUE(static_cast<bool>(ima));
        EXPECT_TRUE(ima());
    }
}
