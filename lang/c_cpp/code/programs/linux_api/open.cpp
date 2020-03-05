#include <cerrno>
#include <fcntl.h>

#include <boost/filesystem.hpp>
#include <gtest/gtest.h>

#include "../../tmpdir/tmpdir.hpp"

namespace bfs = boost::filesystem;

// http://man7.org/linux/man-pages/man2/open.2.html
class Test_open : public ::testing::Test
{
protected:
    Test_open()
      : m_tmp_dpath(ywen::tmpdir::mkdtemp()), m_tmp_dir_guard(m_tmp_dpath)
    {
    }

    bfs::path m_tmp_dpath;
    ywen::tmpdir::dtemp_guard m_tmp_dir_guard;
};

TEST_F(Test_open, open)
{
    const char *pathname = nullptr;
    int flags = -1;
    int fd = -1;
    int ret = -1;

    // --------------------------------------------------
    // When given file path does not exist...

    // Without O_CREAT, the open fails.
    pathname = (m_tmp_dpath / "not_existing_1.txt").string().c_str();
    flags = O_RDONLY;
    fd = ::open(pathname, flags);
    EXPECT_EQ(fd, -1);
    EXPECT_EQ(errno, ENOENT);
    EXPECT_EQ(std::string(std::strerror(errno)), "No such file or directory");

    // With O_CREAT, the open succeeds.
    pathname = (m_tmp_dpath / "not_existing_2.txt").string().c_str();
    flags = (O_RDONLY | O_CREAT);
    fd = ::open(pathname, flags);
    EXPECT_GE(fd, 0);
    ret = ::close(fd);
    EXPECT_EQ(ret, 0);

    // O_APPEND alone doesn't create the file if it doesn't exist. It assumes
    // the file already exists.
    pathname = (m_tmp_dpath / "not_existing_3.txt").string().c_str();
    flags = (O_WRONLY | O_APPEND);
    fd = ::open(pathname, flags);
    EXPECT_EQ(fd, -1);
    EXPECT_EQ(errno, ENOENT);
    EXPECT_EQ(std::string(std::strerror(errno)), "No such file or directory");

    // O_CREAT actually creates the file.
    pathname = (m_tmp_dpath / "not_existing_4.txt").string().c_str();
    flags = (O_RDONLY | O_CREAT);
    fd = ::open(pathname, flags);
    // TODO(ywen): Why do I get "permission denied" error? Is it because of
    // "file sealing"? http://man7.org/linux/man-pages/man2/fcntl.2.html
    EXPECT_GE(fd, 0);
    std::cout << std::strerror(errno) << std::endl;
}
