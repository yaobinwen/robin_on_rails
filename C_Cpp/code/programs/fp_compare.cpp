// Ref: https://www.boost.org/doc/libs/1_65_0/libs/math/doc/html/math_toolkit/float_comparison.html

#include <boost/math/special_functions/relative_difference.hpp>
#include <gtest/gtest.h>

using boost::math::epsilon_difference;

bool exactly_equal(double a, double b)
{
    return (epsilon_difference(a, b) == 0.0);
}

bool almost_equal(double a, double b)
{
    return (epsilon_difference(a, b) < 1.0);
}

TEST(fp_compare, test)
{
    EXPECT_TRUE(almost_equal(0.1 + 0.2, 0.3));
    EXPECT_TRUE(exactly_equal(0.1 + 0.2, 0.1 + 0.2));
    EXPECT_TRUE(exactly_equal(0.3, 0.3));
}
