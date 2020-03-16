#include <map>
#include <string>

#include <gtest/gtest.h>

#include "../external/json.hpp"

TEST(nlohmann_json, test_initialization_with_different_types)
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

TEST(nlohmann_json, test_modifiers)
{
    using ::nlohmann::json;

    json j = json::parse("{\"a\":10, \"b\":20}");
    j += json::object_t::value_type("c", 30);
    EXPECT_EQ(j.dump(), "{\"a\":10,\"b\":20,\"c\":30}");
}

TEST(nlohmann_json, test_iterator)
{
    using ::nlohmann::json;

    std::map<std::string, int> items = { { "a", 10 }, { "b", 20 } };

    json j;
    j += json::object_t::value_type("a", 10);
    j += json::object_t::value_type("b", 20);

    for (auto const &[key, value] : j.items())
    {
        EXPECT_EQ(items.at(key), value);
    }
}
