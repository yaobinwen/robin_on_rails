#include <sstream>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <gtest/gtest.h>

#include "../../boost_ext/filesystem.hpp"
#include "../../file/file.hpp"
#include "../../tmpdir/tmpdir.hpp"

namespace bfs = boost::filesystem;

TEST(BoostFileSystemVerifier, test_path)
{
    // What paths are considered empty?
    // 1). A default constructed path is empty.
    EXPECT_TRUE(bfs::path().empty());
    // 2). A path initialized with an empty string is empty.
    EXPECT_TRUE(bfs::path("").empty());
    // 3). A path initialized with a non-empty string is not empty.
    EXPECT_FALSE(bfs::path(".").empty());

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

TEST(BoostFileSystemVerifier, test_symlink_functions)
{
    using ywen::file::create_file;
    using ywen::tmpdir::dtemp_guard;
    using ywen::tmpdir::mkdtemp;

    bfs::path tmp_path = mkdtemp();
    dtemp_guard __g(tmp_path);

    // Create a symlink that points to an existing file.
    bfs::path fpath = tmp_path / "tmp.txt";
    bfs::path lpath = tmp_path / "tmp.txt.lnk";
    create_file(fpath, "hello");
    bfs::create_symlink(fpath, lpath);

    EXPECT_TRUE(bfs::exists(lpath));

    // For a regular file, status() and symlink_status() return the same type.
    EXPECT_EQ(bfs::status(fpath).type(), bfs::regular_file);
    EXPECT_EQ(bfs::symlink_status(fpath).type(), bfs::status(fpath).type());

    // For a symlink, status() dereferences the link while symlink_status()
    // checks the link itself.
    EXPECT_EQ(bfs::status(lpath).type(), bfs::status(fpath).type());
    EXPECT_EQ(bfs::symlink_status(lpath).type(), bfs::symlink_file);

    // Remove the file so the symlink is broken.
    bfs::remove(fpath);

    // For a regular file, status() and symlink_status() return the same type.
    EXPECT_EQ(bfs::status(fpath).type(), bfs::file_not_found);
    EXPECT_EQ(bfs::symlink_status(fpath).type(), bfs::status(fpath).type());

    // For a symlink, status() dereferences the link while symlink_status()
    // checks the link itself.
    EXPECT_EQ(bfs::status(lpath).type(), bfs::status(fpath).type());
    EXPECT_EQ(bfs::symlink_status(lpath).type(), bfs::symlink_file);
}
