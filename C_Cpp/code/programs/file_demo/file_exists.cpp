/// This program verifies if I can detect whether a file exists or not by
/// simply opening it for reading and checking for `ENOENT`.

#include "../../tmpdir/tmpdir.hpp"

#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <gtest/gtest.h>

#include <cerrno>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace bfs = boost::filesystem;

class FileExistsTest : public ::testing::Test
{
protected:
    FileExistsTest()
      : m_tmpDir(ywen::tmpdir::mkdtemp("FileExistsTest")),
        m_tmpDirGuard(m_tmpDir),
        m_file(m_tmpDir / "file.txt")
    {
        // Empty
    }

    bfs::path m_tmpDir;
    ywen::tmpdir::dtemp_guard m_tmpDirGuard;
    bfs::path m_file;
};

TEST_F(FileExistsTest, test)
{
    bool exception_thrown = false;
    bool errno_is_ENOENT = false;

    try
    {
        std::ifstream fs;

        fs.exceptions(std::ios::badbit | std::ios::failbit);
        fs.open(m_file.native());
        fs.exceptions(std::ios::badbit);

        // We can do something about the file but that's not the point of this
        // program, so we just do nothing here.

        fs.close();

        // We should not reach here, so we make an assertion failure if we do.
        FAIL() << "We should not get here.";
    }
    catch (std::ios_base::failure &e)
    {
        // Because `m_file` doesn't exist, an exception should be thrown so we
        // should end up in the `catch` block.
        exception_thrown = true;

        // NOTE(ywen): Because `std::ios_base::failure` is derived from
        // `std::system_error`, it has the member function `code()`. But the
        // value of `code()` is 1, and I don't know what this value is about.
        // So it is better to check `errno`.
        if (ENOENT != errno)
        {
            throw std::runtime_error(
                str(boost::format("Fail to open file %1%: errno = %2%") %
                    m_file.native() % errno));
        }

        // Because `m_file` doesn't exist, `errno` should be `ENOENT` so we
        // should end up here. If anything else happened, the `runtime_error`
        // above would be thrown and we wouldn't arrive here.
        errno_is_ENOENT = true;
    }

    // We are supposed to reach here with the exception thrown but `errno`
    // being `ENOENT`.
    ASSERT_TRUE(exception_thrown);
    ASSERT_TRUE(errno_is_ENOENT);
}
