#include <set>
#include <string>
#include <boost/filesystem.hpp>
#include <gtest/gtest.h>

#include "../../file/file.hpp"
#include "../../gtest_ext/predicates.hpp"
#include "../../tmpdir/tmpdir.hpp"

namespace bfs = boost::filesystem;

class BFSRenameTest : public ::testing::Test
{
protected:
    BFSRenameTest()
      : m_tmp_dpath(ywen::tmpdir::mkdtemp()),
        m_tmp_dir_guard(m_tmp_dpath),
        m_f1(m_tmp_dpath / "file1.txt"),
        m_d1_empty(m_tmp_dpath / "dir1"),
        m_d2_empty(m_tmp_dpath / "dir2"),
        m_d3_filled(m_tmp_dpath / "dir3"),
        m_d4_filled(m_tmp_dpath / "dir4"),
        m_lnk_f1(m_tmp_dpath / "f1.lnk"),
        m_lnk_d1(m_tmp_dpath / "d1.lnk"),
        m_lnk_d3(m_tmp_dpath / "d3.lnk"),
        m_lnk_broken(m_tmp_dpath / "broken.lnk")
    {
        ywen::file::create_file(m_f1, "file1");

        bfs::create_directory(m_d1_empty);
        bfs::create_directory(m_d2_empty);

        // dir3 is not empty.
        bfs::create_directory(m_d3_filled);
        ywen::file::create_file(m_d3_filled / "file3.txt", "file3");

        // dir4 is not empty.
        bfs::create_directory(m_d4_filled);
        ywen::file::create_file(m_d4_filled / "file4.txt", "file4");

        bfs::create_symlink(m_f1, m_lnk_f1);
        bfs::create_symlink(m_d1_empty, m_lnk_d1);
        bfs::create_symlink(m_d3_filled, m_lnk_d3);
        bfs::create_symlink(m_tmp_dpath / "broken", m_lnk_broken);
    }

    void
    expect_hierarchy(std::set<std::string> const &hierarchy)
    {
        std::set<std::string> actual;
        for (bfs::directory_entry &e :
             bfs::recursive_directory_iterator(m_tmp_dpath))
        {
            actual.insert(e.path().string());
        }

        std::set<std::string> expected;
        for (std::string const &e : hierarchy)
        {
            expected.insert((m_tmp_dpath / e).string());
        }

        EXPECT_EQ(actual, expected);
    }

    template<class Func>
    void
    verify_exception_call(Func func, std::vector<std::string> const &substrings)
    {
        try
        {
            func();
            ADD_FAILURE()
                << "Expect an exception of type "
                   "\"boost::filesystem::filesystem_error\" to be thrown.";
        }
        catch (bfs::filesystem_error const &e)
        {
            std::string msg(e.what());
            for (std::string const &substr : substrings)
            {
                EXPECT_TRUE(ywen::gtest_ext::IsSubstr(msg, substr));
            }
        }

        // Nothing should happen.
        expect_hierarchy({ "file1.txt",
                           "dir1",
                           "dir2",
                           "dir3",
                           "dir3/file3.txt",
                           "dir4",
                           "dir4/file4.txt",
                           "f1.lnk",
                           "d1.lnk",
                           "d3.lnk",
                           "broken.lnk" });
    }

    bfs::path m_tmp_dpath;
    ywen::tmpdir::dtemp_guard m_tmp_dir_guard;

    // These are the files/directories/symlinks inside the temporary directory.
    bfs::path m_f1;
    bfs::path m_d1_empty;
    bfs::path m_d2_empty;
    bfs::path m_d3_filled;
    bfs::path m_d4_filled;
    bfs::path m_lnk_f1;
    bfs::path m_lnk_d1;
    bfs::path m_lnk_d3;
    bfs::path m_lnk_broken;
};

TEST_F(BFSRenameTest, test_renaming_empty_dir1_to_file1)
{
    auto func = [this]() { bfs::rename(this->m_d1_empty, this->m_f1); };
    verify_exception_call(
        func,
        { "Not a directory", m_d1_empty.string(), m_f1.string() });
}

TEST_F(BFSRenameTest, test_renaming_empty_dir1_to_file_symlink)
{
    auto func = [this]() { bfs::rename(this->m_d1_empty, this->m_lnk_f1); };
    verify_exception_call(
        func,
        { "Not a directory", m_d1_empty.string(), m_lnk_f1.string() });
}

TEST_F(BFSRenameTest, test_renaming_empty_dir2_to_empty_dir_symlink)
{
    auto func = [this]() { bfs::rename(this->m_d2_empty, this->m_lnk_d1); };
    verify_exception_call(
        func,
        { "Not a directory", m_d2_empty.string(), m_lnk_d1.string() });
}

TEST_F(BFSRenameTest, test_renaming_empty_dir2_to_filled_dir_symlink)
{
    auto func = [this]() { bfs::rename(this->m_d2_empty, this->m_lnk_d3); };
    verify_exception_call(
        func,
        { "Not a directory", m_d2_empty.string(), m_lnk_d3.string() });
}

TEST_F(BFSRenameTest, test_renaming_empty_dir2_to_broken_symlink)
{
    auto func = [this]() { bfs::rename(this->m_d2_empty, this->m_lnk_broken); };
    verify_exception_call(
        func,
        { "Not a directory", m_d2_empty.string(), m_lnk_broken.string() });
}

TEST_F(BFSRenameTest, test_renaming_empty_dir1_to_itself)
{
    bfs::rename(m_d1_empty, m_d1_empty);

    // Nothing will happen.
    expect_hierarchy({ "file1.txt",
                       "dir1",
                       "dir2",
                       "dir3",
                       "dir3/file3.txt",
                       "dir4",
                       "dir4/file4.txt",
                       "f1.lnk",
                       "d1.lnk",
                       "d3.lnk",
                       "broken.lnk" });
}

TEST_F(BFSRenameTest, test_renaming_empty_d1_to_empty_d2)
{
    // Rename dir1 to dir2 which is empty.
    bfs::rename(m_d1_empty, m_d2_empty);

    // dir1 will disappear.
    expect_hierarchy({ "file1.txt",
                       "dir2",
                       "dir3",
                       "dir3/file3.txt",
                       "dir4",
                       "dir4/file4.txt",
                       "f1.lnk",
                       "d1.lnk",
                       "d3.lnk",
                       "broken.lnk" });
}

TEST_F(BFSRenameTest, test_renaming_empty_d1_to_filled_d3)
{
    auto func = [this]() { bfs::rename(this->m_d1_empty, this->m_d3_filled); };
    verify_exception_call(
        func,
        { "Directory not empty", m_d1_empty.string(), m_d3_filled.string() });
}

TEST_F(BFSRenameTest, test_renaming_filled_d3_to_itself)
{
    // Rename dir3 to itself.
    bfs::rename(m_d3_filled, m_d3_filled);

    // Nothing will happen.
    expect_hierarchy({ "file1.txt",
                       "dir1",
                       "dir2",
                       "dir3",
                       "dir3/file3.txt",
                       "dir4",
                       "dir4/file4.txt",
                       "f1.lnk",
                       "d1.lnk",
                       "d3.lnk",
                       "broken.lnk" });
}

TEST_F(BFSRenameTest, test_renaming_filled_d3_to_empty_d1)
{
    // Rename dir3 to dir1 which is empty.
    bfs::rename(m_d3_filled, m_d1_empty);

    // dir3 will disappear.
    expect_hierarchy({ "file1.txt",
                       "dir1",
                       "dir1/file3.txt",
                       "dir2",
                       "dir4",
                       "dir4/file4.txt",
                       "f1.lnk",
                       "d1.lnk",
                       "d3.lnk",
                       "broken.lnk" });
}

TEST_F(BFSRenameTest, test_renaming_filled_d3_to_filled_d4)
{
    auto func = [this]() { bfs::rename(this->m_d3_filled, this->m_d4_filled); };
    verify_exception_call(
        func,
        { "Directory not empty", m_d3_filled.string(), m_d4_filled.string() });
}
