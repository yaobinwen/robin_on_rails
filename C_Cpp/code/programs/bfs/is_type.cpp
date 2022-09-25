#include <boost/filesystem.hpp>
#include <gtest/gtest.h>

#include "../../file/file.hpp"
#include "../../tmpdir/tmpdir.hpp"

namespace bfs = boost::filesystem;

TEST(is_regular_file, on_various_files)
{
    using ywen::file::create_file;
    using ywen::tmpdir::dtemp_guard;
    using ywen::tmpdir::mkdtemp;

    bfs::path tmp_path = mkdtemp();
    dtemp_guard __g(tmp_path);

    std::string content("hello");
    bfs::path fpath_good = tmp_path / "tmp.txt";
    bfs::path fpath_bad = tmp_path / "bad.txt";
    bfs::path lfpath_good = tmp_path / "tmp.txt.lnk";
    bfs::path lfpath_bad = tmp_path / "bad.txt.lnk";
    bfs::path ldpath = tmp_path / "dir.lnk";
    create_file(fpath_good, content);
    bfs::create_symlink(fpath_good, lfpath_good);
    bfs::create_symlink(fpath_bad, lfpath_bad);
    bfs::create_symlink(tmp_path, ldpath);

    EXPECT_TRUE(bfs::is_regular_file(fpath_good));
    EXPECT_FALSE(bfs::is_regular_file(fpath_bad));
    EXPECT_TRUE(bfs::is_regular_file(lfpath_good));
    EXPECT_FALSE(bfs::is_regular_file(lfpath_bad));
    EXPECT_FALSE(bfs::is_regular_file(ldpath));
}
