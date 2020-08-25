#include <cstdlib>
#include <sstream>

#include <boost/filesystem.hpp>
#include <gtest/gtest.h>

#include "../../external/json.hpp"
#include "../../file/file.hpp"
#include "../../tmpdir/tmpdir.hpp"

namespace bfs = boost::filesystem;

class CopyDirectoryTest : public ::testing::Test
{
protected:
    CopyDirectoryTest()
      : m_tmpDir(ywen::tmpdir::mkdtemp()), m_tmpDirGuard(m_tmpDir)
    {
        std::stringstream ss;
        ss << R"({
            "file1.txt": {
                "type": "file",
                "content": "hello"
            },
            "dir1": {
                "type": "dir",
                "content" : {
                    "file2.txt" : {
                        "type": "file",
                        "content": "hello2"
                    },
                    "file3.txt" : {
                        "type": "file",
                        "content": "hello3"
                    },
                    "dir11" : {
                        "type" : "dir",
                        "content" : {}
                    }
                }
            }
        })";

        ::nlohmann::json hierarchy;
        ss >> hierarchy;

        m_srcDir = m_tmpDir / "src";
        bfs::create_directory(m_srcDir);
        m_dirA = m_srcDir / "A";
        bfs::create_directory(m_dirA);
        ywen::file::create_dir_hierarchy(m_dirA, hierarchy);

        m_dstDir = m_tmpDir / "dst";
        bfs::create_directory(m_dstDir);

        m_dirB = m_dstDir / "B";
        // NOTE(ywen): We don't create m_dirB because we want to test the case
        // of the target directory not existing.
    }

    bfs::path m_tmpDir;
    ywen::tmpdir::dtemp_guard m_tmpDirGuard;

    bfs::path m_srcDir;
    bfs::path m_dirA;
    bfs::path m_dstDir;
    bfs::path m_dirB;
};

TEST_F(CopyDirectoryTest, copy_merely_creates) {
    bfs::copy_directory(m_dirA, m_dirB);

    // As the documentation says, the name `copy_directory` is confusing because
    // what it actually does is:
    // 1). Creates `to`.
    // 2). Copies the attributes from `from`.
    // It does NOT recursively copy anything under the `from` directory, hence
    // `to` being empty.
    EXPECT_TRUE(bfs::is_empty(m_dirB));
}
