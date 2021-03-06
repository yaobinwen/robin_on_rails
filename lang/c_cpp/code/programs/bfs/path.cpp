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

    bfs::path p4(p / "///");
    EXPECT_EQ(p4.string(), "path/to/dir///");
    EXPECT_EQ(bfs::weakly_canonical(p4).string(), "path/to/dir/.");
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

TEST(path, weak_canonical)
{
    bfs::path p;

    p = bfs::weakly_canonical("");
    EXPECT_EQ(p.string(), "");
    EXPECT_EQ(p.filename().string(), "");

    p = bfs::weakly_canonical("relative/path/to/dir");
    EXPECT_EQ(p.string(), "relative/path/to/dir");
    EXPECT_EQ(p.filename().string(), "dir");

    p = bfs::weakly_canonical("relative///path/../path/././to/dir");
    EXPECT_EQ(p.string(), "relative/path/to/dir");
    EXPECT_EQ(p.filename().string(), "dir");

    // NOTE: A path that ends with '/' does NOT have the ending '/' removed.
    // Instead, it appends a dot ('.') to indicate that current level of
    // directory.
    p = bfs::weakly_canonical("relative/path/to/dir///");
    EXPECT_EQ(p.string(), "relative/path/to/dir/.");
    EXPECT_EQ(p.filename().string(), ".");

    p = bfs::weakly_canonical("/absolute/path/to/dir");
    EXPECT_EQ(p.string(), "/absolute/path/to/dir");
    EXPECT_EQ(p.filename().string(), "dir");

    p = bfs::weakly_canonical("/absolute///path/../path/././to/dir");
    EXPECT_EQ(p.string(), "/absolute/path/to/dir");
    EXPECT_EQ(p.filename().string(), "dir");

    // NOTE: A path that ends with '/' does NOT have the ending '/' removed.
    // Instead, it appends a dot ('.') to indicate that current level of
    // directory.
    p = bfs::weakly_canonical("/absolute/path/to/dir///");
    EXPECT_EQ(p.string(), "/absolute/path/to/dir/.");
    EXPECT_EQ(p.filename().string(), ".");
}

TEST(path, remove_ending_slashes)
{
    bfs::path p = bfs::weakly_canonical("relative/path/to/dir///");
    EXPECT_TRUE(p.filename_is_dot());
    EXPECT_EQ(p.parent_path().string(), "relative/path/to/dir");
}

TEST(path_decomposition, root_name)
{
    bfs::path p;

    p = "relative/path";
    // According to the documentation, if the path doesn't have `root-name`,
    // `path()` is returned. Technically, we don't know what value `path()`
    // is constructed to, so instead of comparing an empty string directly,
    // we compare against `path().string()`.
    EXPECT_EQ(p.root_name().string(), bfs::path().string());

    p = "/absolute/path/to/somewhere";
    EXPECT_EQ(p.root_name().string(), bfs::path().string());
}

TEST(path_decomposition, root_directory)
{
    bfs::path p;

    p = "relative/path";
    // According to the documentation, if the path doesn't have
    // `root-directory`, `path()` is returned. Technically, we don't know what
    // value `path()` is constructed to, so instead of comparing an empty string
    // directly, we compare against `path().string()`.
    EXPECT_EQ(p.root_directory().string(), bfs::path().string());

    p = "/absolute/path/to/somewhere";
    EXPECT_EQ(p.root_directory().string(), "/");

    p = "////absolute/path/to/somewhere";
    EXPECT_EQ(p.root_directory().string(), "/");
}

TEST(path_decomposition, root_path)
{
    bfs::path p;

    p = "relative/path";
    EXPECT_EQ(p.root_path().string(), bfs::path().string());

    p = "/absolute/path/to/somewhere";
    EXPECT_EQ(p.root_path().string(), "/");

    p = "////absolute/path/to/somewhere";
    EXPECT_EQ(p.root_path().string(), "/");
}
