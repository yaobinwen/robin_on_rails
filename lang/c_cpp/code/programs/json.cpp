#include <map>
#include <string>

#include <gtest/gtest.h>

#include "../external/json.hpp"

class JSONVerifier : public ::testing::Test
{
};

TEST_F(JSONVerifier, test_initialization_with_different_types)
{
    using ::nlohmann::json;

    EXPECT_EQ(json(10).dump(), "10");
    EXPECT_EQ(json("10").dump(), "\"10\"");
    EXPECT_EQ(json(10.01).dump(), "10.01");
    EXPECT_EQ(json({ 1, 2, 3 }).dump(), "[1,2,3]"); // array
    EXPECT_EQ(
        json({ { 1, 10 }, { 2, 20 }, { 3, 30 } }).dump(),
        "[[1,10],[2,20],[3,30]]"); // array of arrays

    std::map<std::string, char> obj = { { "1", 'a' }, { "2", 'b' } };
    EXPECT_EQ(json(obj).dump(), "{\"1\":97,\"2\":98}");
}
