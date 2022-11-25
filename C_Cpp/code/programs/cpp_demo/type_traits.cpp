#include <vector>
#include <type_traits>
#include <utility>

#include <gtest/gtest.h>

struct C_default
{
    // Empty. Everything uses the default implementation.
};

struct C_move_constructor_only
{
    C_move_constructor_only(C_move_constructor_only && other)
    : v(std::move(other.v))
    {
        // Empty
    }

    // Delete the move assignment operator to see how it affects the move
    // assignability.
    C_move_constructor_only & operator = (C_move_constructor_only &&) = delete;

    std::vector<int> v;
};

struct C_move_constructor_only_noexcept
{
    C_move_constructor_only_noexcept(C_move_constructor_only_noexcept && other) noexcept
    : v(std::move(other.v))
    {
        // Empty
    }

    // Delete the move assignment operator to see how it affects the move
    // assignability.
    C_move_constructor_only_noexcept & operator = (C_move_constructor_only_noexcept &&) = delete;

    std::vector<int> v;
};

struct C_move_assignment_only
{
    // Delete the move constructor to see how it affects the move assignability.
    C_move_assignment_only(C_move_assignment_only &&) = delete;

    C_move_assignment_only & operator = (C_move_assignment_only &&other)
    {
        this->v = std::move(other.v);
        return *this;
    }

    std::vector<int> v;
};

struct C_move_assignment_only_noexcept
{
    // Delete the move constructor to see how it affects the move assignability.
    C_move_assignment_only_noexcept(C_move_assignment_only_noexcept &&) = delete;

    C_move_assignment_only_noexcept & operator = (C_move_assignment_only_noexcept &&other) noexcept
    {
        this->v = std::move(other.v);
        return *this;
    }

    std::vector<int> v;
};

struct C_movable
{
    // Define the move constructor explicitly.
    C_movable(C_movable && other): v(std::move(other.v))
    {
        // Empty
    }

    C_movable & operator = (C_movable && other)
    {
        this->v = std::move(other.v);
        return *this;
    }

    std::vector<int> v;
};

struct C_movable_noexcept
{
    // Define the move constructor explicitly.
    C_movable_noexcept(C_movable_noexcept && other) noexcept
    : v(std::move(other.v))
    {
        // Empty
    }

    C_movable_noexcept & operator = (C_movable_noexcept && other) noexcept
    {
        this->v = std::move(other.v);
        return *this;
    }

    std::vector<int> v;
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

    {
        // C_move_constructor_only

        EXPECT_FALSE(std::is_move_assignable<C_move_constructor_only>::value);
    }

    {
        // C_move_assignment_only

        EXPECT_FALSE(std::is_move_assignable<C_move_constructor_only>::value);
    }

    {
        // C_movable

        EXPECT_TRUE(std::is_move_assignable<C_movable>::value);
    }
}

TEST(Test_type_traits, test_is_nothrow_move_assignable)
{
    // C_default
    EXPECT_TRUE(std::is_nothrow_move_assignable<C_default>::value);

    // C_move_constructor_only
    EXPECT_FALSE(std::is_nothrow_move_assignable<C_move_constructor_only>::value);

    // C_move_constructor_only_noexcept
    EXPECT_FALSE(std::is_nothrow_move_assignable<C_move_constructor_only_noexcept>::value);

    // C_move_assignment_only
    EXPECT_FALSE(std::is_nothrow_move_assignable<C_move_constructor_only>::value);

    // C_move_assignment_only_noexcept
    EXPECT_TRUE(std::is_nothrow_move_assignable<C_move_assignment_only_noexcept>::value);

    // C_movable
    EXPECT_FALSE(std::is_nothrow_move_assignable<C_movable>::value);

    // C_movable_noexcept
    EXPECT_TRUE(std::is_nothrow_move_assignable<C_movable_noexcept>::value);
}
