#include <sstream>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <gtest/gtest.h>

namespace bfs = boost::filesystem;

TEST(path, test_empty)
{
    // What paths are considered empty?
    // 1). A default constructed path is empty.
    EXPECT_TRUE(bfs::path().empty());
    // 2). A path initialized with an empty string is empty.
    EXPECT_TRUE(bfs::path("").empty());
    // 3). A path initialized with a non-empty string is not empty.
    EXPECT_FALSE(bfs::path(".").empty());
}

TEST(path, test_ostream_inserter)
{
    // Can paths be inserted into an output stream directly?
    std::ostringstream oss;
    oss << bfs::path("/tmp");
    // Note the additional double-quotation marks.
    EXPECT_EQ(oss.str(), "\"/tmp\"");
    // It also works for boost::format. With string() called, the output does
    // not have the additional double-quotation marks.
    EXPECT_EQ(
        boost::str(boost::format("%1%") % bfs::path("/tmp").string()), "/tmp");
}

TEST(path, test_appends)
{
    bfs::path p("path/to/dir");

    bfs::path p2(p);
    p2 /= "file";
    EXPECT_EQ(p2.string(), "path/to/dir/file");

    bfs::path p3(p);
    p3.append("file");
    EXPECT_EQ(p3.string(), "path/to/dir/file");
}

TEST(path, test_concatenation)
{
    bfs::path p("path/to/dir");

    bfs::path p2(p);
    p2 += "name";
    EXPECT_EQ(p2.string(), "path/to/dirname");

    bfs::path p3(p);
    p3.concat("name");
    EXPECT_EQ(p3.string(), "path/to/dirname");
}
