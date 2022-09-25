#include <boost/filesystem.hpp>
#include <gtest/gtest.h>

namespace bfs = boost::filesystem;

TEST(absolute, test)
{
    bfs::path cwd = bfs::current_path();

    bfs::path p;

    p = "relative/path/to/something";
    EXPECT_EQ(bfs::absolute(p).string(), (cwd / p).string());

    p = "/absolute/path/to/something";
    EXPECT_EQ(bfs::absolute(p).string(), p.string());
}
