#include <cerrno>
#include <cstdio>
#include <fstream>

#include <boost/filesystem.hpp>
#include <gtest/gtest.h>

#include "../../tmpdir/tmpdir.hpp"

namespace bfs = boost::filesystem;

/**
 * Test `int rename(const char *oldpath, const char *newpath)`
 *
 * The path can be one of the following:
 * - 1). nullptr
 * - 2). "" (empty string)
 * - 3). A non-existing path.
 *  - 3a). The path's parent path exists (e.g. `/tmp/a` where `/tmp` exists
 *      but `a` doesn't exist.)
 *  - 3b). The path's parent path doesn't exist (e.g. `/tmp/b/c/d/e` where
 *      `/tmp/b/c/d` doesn't exist.)
 * - 4). An existing path.
 *  - 4a). Regular file.
 *  - 4b). Directory (empty).
 *  - 4c). Directory (not empty).
 *  - 4d). Symbolic link to a regular file.
 *  - 4e). Symbolic link to a directory.
 *
 * We don't take other types of files into consideration.
 */

/// The fixture to test renaming-related APIs.
class Test_rename : public ::testing::Test
{
protected:
    Test_rename()
      : m_tmp_dpath(ywen::tmpdir::mkdtemp()),
        m_tmp_dir_guard(m_tmp_dpath),
        m_tmp_a(m_tmp_dpath / "a"),
        m_tmp_bcde(m_tmp_dpath / "b" / "c" / "d" / "e"),
        m_tmp_m(m_tmp_dpath / "m"),
        m_tmp_opq(m_tmp_dpath / "o" / "p" / "q"),
        m_fpath1(m_tmp_dpath / "file1.txt"),
        m_dpath1_e(m_tmp_dpath / "dir1_e"),
        m_dpath1_ne(m_tmp_dpath / "dir1_ne"),
        m_fpath3(m_dpath1_ne / "file3.txt"),
        m_lfpath1(m_tmp_dpath / "file1.lnk"),
        m_ldpath1_e(m_tmp_dpath / "dir1_e.lnk"),
        m_fpath2(m_tmp_dpath / "file2.txt"),
        m_dpath2_e(m_tmp_dpath / "dir2_e"),
        m_dpath2_ne(m_tmp_dpath / "dir2_ne"),
        m_fpath4(m_dpath2_ne / "file4.txt"),
        m_lfpath2(m_tmp_dpath / "file2.lnk"),
        m_ldpath2_e(m_tmp_dpath / "dir2_e.lnk")
    {
        createFile(m_fpath1, "file1");

        bfs::create_directory(m_dpath1_e);

        bfs::create_directory(m_dpath1_ne);
        createFile(m_fpath3, "file3");

        bfs::create_symlink(m_fpath1, m_lfpath1);
        bfs::create_symlink(m_dpath1_e, m_ldpath1_e);

        createFile(m_fpath2, "file2");

        bfs::create_directory(m_dpath2_e);

        bfs::create_directory(m_dpath2_ne);
        createFile(m_fpath4, "file4");

        bfs::create_symlink(m_fpath2, m_lfpath2);
        bfs::create_symlink(m_dpath2_e, m_ldpath2_e);

        m_paths.assign({ nullptr,
                         "",
                         m_tmp_a.string().c_str(),
                         m_tmp_bcde.string().c_str(),
                         m_fpath1.string().c_str(),
                         m_dpath1_e.string().c_str(),
                         m_dpath1_ne.string().c_str(),
                         m_lfpath1.string().c_str(),
                         m_ldpath1_e.string().c_str() });
    }

    static void
    createFile(bfs::path const &fpath, std::string const &content)
    {
        std::ofstream f(fpath.string(), std::ios::out);
        f << content;
        f.close();
    }

    static std::string
    readFileContent(bfs::path const &fpath)
    {
        std::string content;
        std::ifstream in(fpath.string(), std::ios::in);
        content.assign(
            std::istreambuf_iterator<char>(in),
            std::istreambuf_iterator<char>());
        return content;
    }

    bfs::path m_tmp_dpath;
    ywen::tmpdir::dtemp_guard m_tmp_dir_guard;

    bfs::path m_tmp_a;
    bfs::path m_tmp_bcde;
    bfs::path m_tmp_m;
    bfs::path m_tmp_opq;

    bfs::path m_fpath1;
    bfs::path m_dpath1_e;
    bfs::path m_dpath1_ne;
    bfs::path m_fpath3;
    bfs::path m_lfpath1;
    bfs::path m_ldpath1_e;

    bfs::path m_fpath2;
    bfs::path m_dpath2_e;
    bfs::path m_dpath2_ne;
    bfs::path m_fpath4;
    bfs::path m_lfpath2;
    bfs::path m_ldpath2_e;

    /// `m_paths` is designed to enumerate all the above-mentioned cases of
    /// 1), 2), 3a), 3b), 4a), 4b), 4c), and 4d). Therefore, it doesn't need
    /// to include all the paths defined in the member variables because some
    /// of them represent the same case. For example, both `m_tmp_a` and
    /// `m_tmp_m` are for case 3a), so only one of them should be included.
    std::vector<const char *> m_paths;
};

TEST_F(Test_rename, case_1_vs_all)
{
    const char *lhs = nullptr;
    for (auto rhs : m_paths)
    {
        errno = 0;
        int ret = rename(lhs, rhs);
        EXPECT_EQ(ret, -1);
        EXPECT_EQ(errno, EFAULT) // Bad address
            << "lhs = '" << lhs << "'; rhs = '" << rhs << "'";
    }
}

TEST_F(Test_rename, case_2_vs_all)
{
    const char *lhs = "";
    for (auto rhs : m_paths)
    {
        errno = 0;
        int ret = rename(lhs, rhs);
        EXPECT_EQ(ret, -1);
        EXPECT_EQ(errno, ENOENT) // No such file or directory
            << "lhs = '" << lhs << "'; rhs = '" << rhs << "'";
    }
}

TEST_F(Test_rename, case_all_vs_1)
{
    const char *rhs = nullptr;

    std::vector<const char *> old_paths1({ nullptr,
                                           m_tmp_a.string().c_str(),
                                           m_fpath1.string().c_str(),
                                           m_dpath1_e.string().c_str(),
                                           m_dpath1_ne.string().c_str(),
                                           m_lfpath1.string().c_str(),
                                           m_ldpath1_e.string().c_str() });
    for (auto lhs : old_paths1)
    {
        errno = 0;
        int ret = rename(lhs, rhs);
        EXPECT_EQ(ret, -1);
        EXPECT_EQ(errno, EFAULT) // Bad address
            << "lhs = '" << lhs << "'; rhs = '" << rhs << "'";
    }

    std::vector<const char *> old_paths2({ "", m_tmp_bcde.string().c_str() });
    for (auto lhs : old_paths2)
    {
        errno = 0;
        int ret = rename(lhs, rhs);
        EXPECT_EQ(ret, -1);
        EXPECT_EQ(errno, ENOENT) // No such file or directory
            << "lhs = '" << lhs << "'; rhs = '" << rhs << "'";
    }
}

TEST_F(Test_rename, case_all_vs_2)
{
    const char *rhs = "";

    errno = 0;
    int ret = rename(nullptr, rhs);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EFAULT) // Bad address
        << "lhs = '" << (const char *)nullptr << "'; rhs = '" << rhs << "'";

    std::vector<const char *> old_paths2({ "",
                                           m_tmp_a.string().c_str(),
                                           m_tmp_bcde.string().c_str(),
                                           m_fpath1.string().c_str(),
                                           m_dpath1_e.string().c_str(),
                                           m_dpath1_ne.string().c_str(),
                                           m_lfpath1.string().c_str(),
                                           m_ldpath1_e.string().c_str() });
    for (auto lhs : old_paths2)
    {
        errno = 0;
        int ret = rename(lhs, rhs);
        EXPECT_EQ(ret, -1);
        EXPECT_EQ(errno, ENOENT) // No such file or directory
            << "lhs = '" << lhs << "'; rhs = '" << rhs << "'";
    }
}

TEST_F(Test_rename, case_3a_vs_all)
{
    const char *lhs = m_tmp_a.string().c_str();

    // rhs being nullptr or an empty string has been tested earlier, so we
    // don't test them again here.
    std::vector<const char *> paths({ m_tmp_m.string().c_str(),
                                      m_tmp_opq.string().c_str(),
                                      m_fpath1.string().c_str(),
                                      m_dpath1_e.string().c_str(),
                                      m_dpath1_ne.string().c_str(),
                                      m_lfpath1.string().c_str(),
                                      m_ldpath1_e.string().c_str() });
    for (auto rhs : paths)
    {
        errno = 0;
        int ret = rename(lhs, rhs);
        EXPECT_EQ(ret, -1);
        EXPECT_EQ(errno, ENOENT) // No such file or directory
            << "lhs = '" << lhs << "'; rhs = '" << rhs << "'";
    }
}

TEST_F(Test_rename, case_3b_vs_all)
{
    const char *lhs = m_tmp_bcde.string().c_str();

    // rhs being nullptr or an empty string has been tested earlier, so we
    // don't test them again here.
    std::vector<const char *> paths({ m_tmp_m.string().c_str(),
                                      m_tmp_opq.string().c_str(),
                                      m_fpath1.string().c_str(),
                                      m_dpath1_e.string().c_str(),
                                      m_dpath1_ne.string().c_str(),
                                      m_lfpath1.string().c_str(),
                                      m_ldpath1_e.string().c_str() });
    for (auto rhs : paths)
    {
        errno = 0;
        int ret = rename(lhs, rhs);
        EXPECT_EQ(ret, -1);
        EXPECT_EQ(errno, ENOENT) // No such file or directory
            << "lhs = '" << lhs << "'; rhs = '" << rhs << "'";
    }
}

TEST_F(Test_rename, case_4a_vs_3a)
{
    const char *lhs = m_fpath1.string().c_str();
    const char *rhs = m_tmp_a.string().c_str();

    EXPECT_TRUE(bfs::is_regular_file(m_fpath1));
    EXPECT_FALSE(bfs::exists(m_tmp_a));

    errno = 0;
    int ret = rename(lhs, rhs);

    EXPECT_EQ(ret, 0);
    EXPECT_EQ(errno, 0);
    EXPECT_FALSE(bfs::exists(m_fpath1));
    EXPECT_TRUE(bfs::is_regular_file(m_tmp_a));
}

TEST_F(Test_rename, case_4a_vs_3b)
{
    const char *lhs = m_fpath1.string().c_str();
    const char *rhs = m_tmp_bcde.string().c_str();

    EXPECT_TRUE(bfs::is_regular_file(m_fpath1));
    EXPECT_FALSE(bfs::exists(m_tmp_bcde));

    errno = 0;
    int ret = rename(lhs, rhs);

    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, ENOENT);
    EXPECT_TRUE(bfs::is_regular_file(m_fpath1));
    EXPECT_FALSE(bfs::exists(m_tmp_bcde));
}

// old path: regular file
// new path: another regular file
TEST_F(Test_rename, case_4a_vs_4a)
{
    const char *lhs = m_fpath1.string().c_str();
    const char *rhs = m_fpath2.string().c_str();

    EXPECT_TRUE(bfs::is_regular_file(m_fpath1));
    EXPECT_TRUE(bfs::is_regular_file(m_fpath2));
    EXPECT_EQ(readFileContent(m_fpath1), "file1");

    errno = 0;
    int ret = rename(lhs, rhs);

    EXPECT_EQ(ret, 0);
    EXPECT_EQ(errno, 0);
    EXPECT_FALSE(bfs::exists(m_fpath1));
    EXPECT_TRUE(bfs::is_regular_file(m_fpath2));
    EXPECT_EQ(readFileContent(m_fpath2), "file1");
}

// old path: regular file
// new path: existing directory (empty)
// Renaming fails with errno `EISDIR`; nothing is changed.
TEST_F(Test_rename, case_4a_vs_4b)
{
    const char *lhs = m_fpath1.string().c_str();
    const char *rhs = m_dpath2_e.string().c_str();

    EXPECT_TRUE(bfs::is_regular_file(m_fpath1));
    EXPECT_TRUE(bfs::is_directory(m_dpath2_e));
    EXPECT_EQ(readFileContent(m_fpath1), "file1");

    errno = 0;
    int ret = rename(lhs, rhs);

    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EISDIR); // Is a directory
    EXPECT_TRUE(bfs::is_regular_file(m_fpath1));
    EXPECT_TRUE(bfs::is_directory(m_dpath2_e));
    EXPECT_EQ(readFileContent(m_fpath1), "file1");
}

// old path: regular file
// new path: existing directory (not empty)
// Renaming fails with errno `EISDIR`; nothing is changed.
TEST_F(Test_rename, case_4a_vs_4c)
{
    const char *lhs = m_fpath1.string().c_str();
    const char *rhs = m_dpath2_ne.string().c_str();

    EXPECT_TRUE(bfs::is_regular_file(m_fpath1));
    EXPECT_TRUE(bfs::is_directory(m_dpath2_ne));
    EXPECT_EQ(readFileContent(m_fpath1), "file1");

    errno = 0;
    int ret = rename(lhs, rhs);

    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EISDIR); // Is a directory
    EXPECT_TRUE(bfs::is_regular_file(m_fpath1));
    EXPECT_TRUE(bfs::is_directory(m_dpath2_ne));
    EXPECT_EQ(readFileContent(m_fpath1), "file1");
}

// old path: regular file
// new path: symlink to a regular file
TEST_F(Test_rename, case_4a_vs_4d)
{
    const char *lhs = m_fpath1.string().c_str();
    const char *rhs = m_lfpath2.string().c_str();

    EXPECT_TRUE(bfs::is_regular_file(m_fpath1));
    EXPECT_EQ(readFileContent(m_fpath1), "file1");
    EXPECT_TRUE(bfs::is_symlink(m_lfpath2));
    EXPECT_EQ(readFileContent(m_lfpath2), "file2");

    errno = 0;
    int ret = rename(lhs, rhs);

    EXPECT_EQ(ret, 0);
    EXPECT_EQ(errno, 0);
    EXPECT_FALSE(bfs::exists(m_fpath1));
    EXPECT_TRUE(bfs::is_regular_file(m_lfpath2));
    EXPECT_EQ(readFileContent(m_lfpath2), "file1");
}

// old path: regular file
// new path: symlink to a directory
TEST_F(Test_rename, case_4a_vs_4e)
{
    const char *lhs = m_fpath1.string().c_str();
    const char *rhs = m_ldpath2_e.string().c_str();

    EXPECT_TRUE(bfs::is_regular_file(m_fpath1));
    EXPECT_TRUE(bfs::is_symlink(m_ldpath2_e));
    EXPECT_EQ(readFileContent(m_fpath1), "file1");

    errno = 0;
    int ret = rename(lhs, rhs);

    EXPECT_EQ(ret, 0);
    EXPECT_EQ(errno, 0);
    EXPECT_FALSE(bfs::exists(m_fpath1));
    EXPECT_TRUE(bfs::is_regular_file(m_ldpath2_e));
    EXPECT_EQ(readFileContent(m_ldpath2_e), "file1");
}

// old path: directory (empty)
// new path: non-exisitng directory (3a)
TEST_F(Test_rename, case_4b_vs_3a)
{
    const char *lhs = m_dpath1_e.string().c_str();
    const char *rhs = m_tmp_a.string().c_str();

    EXPECT_TRUE(bfs::is_directory(m_dpath1_e));
    EXPECT_FALSE(bfs::exists(m_tmp_a));

    errno = 0;
    int ret = rename(lhs, rhs);

    EXPECT_EQ(ret, 0);
    EXPECT_EQ(errno, 0);
    EXPECT_FALSE(bfs::exists(m_dpath1_e));
    EXPECT_TRUE(bfs::is_directory(m_tmp_a));
}

// old path: directory (empty)
// new path: non-exisitng directory (3b)
TEST_F(Test_rename, case_4b_vs_3b)
{
    const char *lhs = m_dpath1_e.string().c_str();
    const char *rhs = m_tmp_bcde.string().c_str();

    EXPECT_TRUE(bfs::is_directory(m_dpath1_e));
    EXPECT_FALSE(bfs::exists(m_tmp_bcde));

    errno = 0;
    int ret = rename(lhs, rhs);

    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, ENOENT);
    EXPECT_TRUE(bfs::is_directory(m_dpath1_e));
    EXPECT_FALSE(bfs::exists(m_tmp_bcde));
}

// old path: directory (empty)
// new path: regular file
TEST_F(Test_rename, case_4b_vs_4a)
{
    const char *lhs = m_dpath1_e.string().c_str();
    const char *rhs = m_fpath2.string().c_str();

    EXPECT_TRUE(bfs::is_directory(m_dpath1_e));
    EXPECT_TRUE(bfs::is_regular_file(m_fpath2));

    errno = 0;
    int ret = rename(lhs, rhs);

    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, ENOTDIR); // Not a directory
    EXPECT_TRUE(bfs::is_directory(m_dpath1_e));
    EXPECT_TRUE(bfs::is_regular_file(m_fpath2));
}

// old path: directory (empty)
// new path: another directory (empty)
// This is esentially the same as removing the old path.
TEST_F(Test_rename, case_4b_vs_4b)
{
    const char *lhs = m_dpath1_e.string().c_str();
    const char *rhs = m_dpath2_e.string().c_str();

    EXPECT_TRUE(bfs::is_directory(m_dpath1_e));
    EXPECT_TRUE(bfs::is_directory(m_dpath2_e));
    EXPECT_TRUE(bfs::is_empty(m_dpath2_e));

    errno = 0;
    int ret = rename(lhs, rhs);

    EXPECT_EQ(ret, 0);
    EXPECT_EQ(errno, 0);
    EXPECT_FALSE(bfs::exists(m_dpath1_e));
    EXPECT_TRUE(bfs::is_directory(m_dpath2_e));
    EXPECT_TRUE(bfs::is_empty(m_dpath2_e));
}

// old path: directory (empty)
// new path: another directory (not empty)
// This is esentially the same as removing the old path.
TEST_F(Test_rename, case_4b_vs_4c)
{
    const char *lhs = m_dpath1_e.string().c_str();
    const char *rhs = m_dpath2_ne.string().c_str();

    EXPECT_TRUE(bfs::is_directory(m_dpath1_e));
    EXPECT_TRUE(bfs::is_directory(m_dpath2_ne));
    EXPECT_FALSE(bfs::is_empty(m_dpath2_ne));

    errno = 0;
    int ret = rename(lhs, rhs);

    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, ENOTEMPTY); // Directory not empty
    EXPECT_TRUE(bfs::is_directory(m_dpath1_e));
    EXPECT_TRUE(bfs::is_directory(m_dpath2_ne));
    EXPECT_FALSE(bfs::is_empty(m_dpath2_ne));
}

// old path: directory (empty)
// new path: symlink to regular file
TEST_F(Test_rename, case_4b_vs_4d)
{
    const char *lhs = m_dpath1_e.string().c_str();
    const char *rhs = m_lfpath2.string().c_str();

    EXPECT_TRUE(bfs::is_directory(m_dpath1_e));
    EXPECT_TRUE(bfs::is_symlink(m_lfpath2));

    errno = 0;
    int ret = rename(lhs, rhs);

    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, ENOTDIR); // Not a directory
    EXPECT_TRUE(bfs::is_directory(m_dpath1_e));
    EXPECT_TRUE(bfs::is_symlink(m_lfpath2));
}

// old path: directory (empty)
// new path: symlink to directory
TEST_F(Test_rename, case_4b_vs_4e)
{
    const char *lhs = m_dpath1_e.string().c_str();
    const char *rhs = m_ldpath2_e.string().c_str();

    EXPECT_TRUE(bfs::is_directory(m_dpath1_e));
    EXPECT_TRUE(bfs::is_symlink(m_ldpath2_e));

    errno = 0;
    int ret = rename(lhs, rhs);

    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, ENOTDIR); // Not a directory
    EXPECT_TRUE(bfs::is_directory(m_dpath1_e));
    EXPECT_TRUE(bfs::is_symlink(m_ldpath2_e));
}

// =========================================================================

// old path: directory (not empty)
// new path: non-exisitng directory (3a)
TEST_F(Test_rename, case_4c_vs_3a)
{
    const char *lhs = m_dpath1_ne.string().c_str();
    const char *rhs = m_tmp_a.string().c_str();

    EXPECT_TRUE(bfs::is_directory(m_dpath1_ne));
    EXPECT_FALSE(bfs::exists(m_tmp_a));

    errno = 0;
    int ret = rename(lhs, rhs);

    EXPECT_EQ(ret, 0);
    EXPECT_EQ(errno, 0);
    EXPECT_FALSE(bfs::exists(m_dpath1_ne));
    EXPECT_TRUE(bfs::is_directory(m_tmp_a));

    bfs::path new_fpath3 = m_tmp_a / m_fpath3.filename();
    EXPECT_TRUE(bfs::is_regular_file(new_fpath3));
    EXPECT_EQ(readFileContent(new_fpath3), "file3");
}

// old path: directory (not empty)
// new path: non-exisitng directory (3b)
TEST_F(Test_rename, case_4c_vs_3b)
{
    const char *lhs = m_dpath1_ne.string().c_str();
    const char *rhs = m_tmp_bcde.string().c_str();

    EXPECT_TRUE(bfs::is_directory(m_dpath1_ne));
    EXPECT_EQ(readFileContent(m_fpath3), "file3");
    EXPECT_FALSE(bfs::exists(m_tmp_bcde));

    errno = 0;
    int ret = rename(lhs, rhs);

    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, ENOENT);
    EXPECT_TRUE(bfs::is_directory(m_dpath1_ne));
    EXPECT_EQ(readFileContent(m_fpath3), "file3");
    EXPECT_FALSE(bfs::exists(m_tmp_bcde));
}

// old path: directory (not empty)
// new path: regular file
TEST_F(Test_rename, case_4c_vs_4a)
{
    const char *lhs = m_dpath1_ne.string().c_str();
    const char *rhs = m_fpath2.string().c_str();

    EXPECT_TRUE(bfs::is_directory(m_dpath1_ne));
    EXPECT_EQ(readFileContent(m_fpath3), "file3");
    EXPECT_TRUE(bfs::is_regular_file(m_fpath2));

    errno = 0;
    int ret = rename(lhs, rhs);

    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, ENOTDIR); // Not a directory
    EXPECT_TRUE(bfs::is_directory(m_dpath1_e));
    EXPECT_EQ(readFileContent(m_fpath3), "file3");
    EXPECT_TRUE(bfs::is_regular_file(m_fpath2));
}

// old path: directory (not empty)
// new path: another directory (empty)
TEST_F(Test_rename, case_4c_vs_4b)
{
    const char *lhs = m_dpath1_ne.string().c_str();
    const char *rhs = m_dpath2_e.string().c_str();

    EXPECT_TRUE(bfs::is_directory(m_dpath1_ne));
    EXPECT_EQ(readFileContent(m_fpath3), "file3");
    EXPECT_TRUE(bfs::is_directory(m_dpath2_e));
    EXPECT_TRUE(bfs::is_empty(m_dpath2_e));

    errno = 0;
    int ret = rename(lhs, rhs);

    EXPECT_EQ(ret, 0);
    EXPECT_EQ(errno, 0);
    EXPECT_FALSE(bfs::exists(m_dpath1_ne));
    EXPECT_TRUE(bfs::is_directory(m_dpath2_e));
    // This directory, although named as "empty", is no longer empty.
    EXPECT_FALSE(bfs::is_empty(m_dpath2_e));

    bfs::path new_fpath3 = m_dpath2_e / m_fpath3.filename();
    EXPECT_EQ(readFileContent(new_fpath3), "file3");
}

// old path: directory (not empty)
// new path: another directory (not empty)
TEST_F(Test_rename, case_4c_vs_4c)
{
    const char *lhs = m_dpath1_ne.string().c_str();
    const char *rhs = m_dpath2_ne.string().c_str();

    EXPECT_TRUE(bfs::is_directory(m_dpath1_ne));
    EXPECT_FALSE(bfs::is_empty(m_dpath1_ne));
    EXPECT_TRUE(bfs::is_directory(m_dpath2_ne));
    EXPECT_FALSE(bfs::is_empty(m_dpath2_ne));

    errno = 0;
    int ret = rename(lhs, rhs);

    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, ENOTEMPTY); // Directory not empty
    EXPECT_TRUE(bfs::is_directory(m_dpath1_ne));
    EXPECT_FALSE(bfs::is_empty(m_dpath1_ne));
    EXPECT_TRUE(bfs::is_directory(m_dpath2_ne));
    EXPECT_FALSE(bfs::is_empty(m_dpath2_ne));
}

// old path: directory (not empty)
// new path: symlink to regular file
TEST_F(Test_rename, case_4c_vs_4d)
{
    const char *lhs = m_dpath1_ne.string().c_str();
    const char *rhs = m_lfpath2.string().c_str();

    EXPECT_TRUE(bfs::is_directory(m_dpath1_ne));
    EXPECT_FALSE(bfs::is_empty(m_dpath1_ne));
    EXPECT_TRUE(bfs::is_symlink(m_lfpath2));

    errno = 0;
    int ret = rename(lhs, rhs);

    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, ENOTDIR); // Not a directory
    EXPECT_TRUE(bfs::is_directory(m_dpath1_ne));
    EXPECT_FALSE(bfs::is_empty(m_dpath1_ne));
    EXPECT_TRUE(bfs::is_symlink(m_lfpath2));
}

// old path: directory (not empty)
// new path: symlink to directory
TEST_F(Test_rename, case_4c_vs_4e)
{
    const char *lhs = m_dpath1_ne.string().c_str();
    const char *rhs = m_ldpath2_e.string().c_str();

    EXPECT_TRUE(bfs::is_directory(m_dpath1_ne));
    EXPECT_FALSE(bfs::is_empty(m_dpath1_ne));
    EXPECT_TRUE(bfs::is_symlink(m_ldpath2_e));

    errno = 0;
    int ret = rename(lhs, rhs);

    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, ENOTDIR); // Not a directory
    EXPECT_TRUE(bfs::is_directory(m_dpath1_ne));
    EXPECT_FALSE(bfs::is_empty(m_dpath1_ne));
    EXPECT_TRUE(bfs::is_symlink(m_ldpath2_e));
}

// TODO(ywen): 4d) and 4e).
