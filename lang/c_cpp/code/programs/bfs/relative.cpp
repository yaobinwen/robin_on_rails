#include <boost/filesystem.hpp>
#include <gtest/gtest.h>

#include "../../file/file.hpp"
#include "../../gtest_ext/predicates.hpp"
#include "../../tmpdir/tmpdir.hpp"

namespace bfs = boost::filesystem;

class BFSRelativeTest : public ::testing::Test
{
protected:
    BFSRelativeTest()
      : m_tmp_dpath(ywen::tmpdir::mkdtemp()),
        m_tmp_dir_guard(m_tmp_dpath),
        m_dirA(m_tmp_dpath / "A"),
        m_dirABC(m_dirA / "B" / "C"),
        m_dirM(m_tmp_dpath / "M"),
        m_dirXY(m_tmp_dpath / "X" / "Y"),
        m_lnkZ(m_dirXY / "Z")
    {
        bfs::create_directories(m_dirABC);
        bfs::create_directories(m_dirM);
        bfs::create_directories(m_dirXY);

        bfs::create_symlink(m_dirM, m_lnkZ);
    }

    bfs::path m_tmp_dpath;
    ywen::tmpdir::dtemp_guard m_tmp_dir_guard;

    bfs::path m_dirA;
    bfs::path m_dirABC;
    bfs::path m_dirM;
    bfs::path m_dirXY;
    bfs::path m_lnkZ;
};

TEST_F(BFSRelativeTest, test)
{
    bfs::path lnkC = m_dirA / "lnkC";
    bfs::path rel_path = bfs::relative(lnkC, m_dirABC);
    EXPECT_EQ(rel_path.string(), "../../lnkC");
}
