#include <boost/filesystem.hpp>
#include <gtest/gtest.h>

namespace bfs = boost::filesystem;

TEST(bfs_path, gtest_ok_if_equal)
{
    bfs::path p1("path/to/file1");
    bfs::path p1_copy(p1);

    EXPECT_EQ(p1, p1_copy); // OK
}

TEST(bfs_path, gtest_crash_if_not_equal)
{
    bfs::path p1("path/to/file1");
    bfs::path p2("path/to/file2");

    // Segmentation fault due to call stack overflow.
    // Boost.Filesystem: 1.65.1
    // gtest: 1.7.0
    EXPECT_EQ(p1, p2);
}
