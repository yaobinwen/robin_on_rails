#include <boost/filesystem.hpp>
#include <gtest/gtest.h>

#include "../../tmpdir/tmpdir.hpp"

namespace bfs = boost::filesystem;

class DirectoryIteratorTest : public ::testing::Test
{
protected:
    DirectoryIteratorTest()
      : m_tmpDir(ywen::tmpdir::mkdtemp()), m_tmpDirGuard(m_tmpDir)
    {
        bfs::create_directory(m_tmpDir / "a");
        bfs::create_directory(m_tmpDir / "b");
        bfs::create_directory(m_tmpDir / "c");
    }

    bfs::path m_tmpDir;
    ywen::tmpdir::dtemp_guard m_tmpDirGuard;
};

TEST_F(DirectoryIteratorTest, path_is_absolute)
{
    size_t count = 0;
    for (bfs::directory_iterator d(m_tmpDir); d != bfs::directory_iterator();
         ++d)
    {
        count++;
        EXPECT_TRUE(d->path().is_absolute());
    }
    EXPECT_EQ(count, 3U);
}
