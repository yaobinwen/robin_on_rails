#include <boost/filesystem.hpp>
#include <gtest/gtest.h>

#include "../../file/file.hpp"
#include "../../gtest_ext/predicates.hpp"
#include "../../tmpdir/tmpdir.hpp"

namespace bfs = boost::filesystem;

class BFSCreateDirectoryTest : public ::testing::Test
{
protected:
    BFSCreateDirectoryTest()
      : m_tmp_dpath(ywen::tmpdir::mkdtemp()), m_tmp_dir_guard(m_tmp_dpath)
    {
    }

    bfs::path m_tmp_dpath;
    ywen::tmpdir::dtemp_guard m_tmp_dir_guard;
};

TEST_F(BFSCreateDirectoryTest, create_same_empty_directory_twice)
{
    EXPECT_TRUE(bfs::create_directory(m_tmp_dpath / "dir"));

    // This won't throw any exception but the return value indicates if the
    // creation succeeds.
    EXPECT_FALSE(bfs::create_directory(m_tmp_dpath / "dir"));
}

TEST_F(BFSCreateDirectoryTest, create_directory_twice_but_not_empty)
{
    bfs::path dir_path = m_tmp_dpath / "dir";

    EXPECT_TRUE(bfs::create_directory(dir_path));
    ywen::file::create_file(dir_path / "file.txt");

    // This won't throw any exception but the return value indicates if the
    // creation succeeds.
    EXPECT_FALSE(bfs::create_directory(dir_path));
}
