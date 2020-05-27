#include "../../file/file.hpp"
#include "../../tmpdir/tmpdir.hpp"

#include <boost/filesystem.hpp>
#include <gtest/gtest.h>

#include <cstring>

namespace bfs = boost::filesystem;

class FileFlagBitsTest : public ::testing::Test
{
protected:
    FileFlagBitsTest()
      : m_tmpDir(ywen::tmpdir::mkdtemp("FileFlagBitsTest")),
        m_tmpDirGuard(m_tmpDir),
        m_content("line"),
        m_file(m_tmpDir / "file.txt")
    {
        std::ofstream f;
        f.open(m_file.string(), std::ios::app);
        f << "line"; // Do not insert a newline.
        f.close();
    }

    bfs::path m_tmpDir;
    ywen::tmpdir::dtemp_guard m_tmpDirGuard;
    std::string m_content;
    bfs::path m_file;
};

TEST_F(FileFlagBitsTest, eofbit)
{
    char buf[8] = {0};

    std::ifstream f;
    f.open(m_file.string());

    f.getline(buf, 8);
    EXPECT_FALSE(f.bad());
    EXPECT_TRUE(f.eof());
    EXPECT_FALSE(f.fail());
    EXPECT_FALSE(f.good());

    f.close();
}

TEST_F(FileFlagBitsTest, failbit)
{
    char buf[8] = {0};

    std::ifstream f;
    f.open(m_file.string());

    // Get the size of the file.
    f.seekg(0, std::ios::end);
    EXPECT_EQ(f.tellg(), m_content.size());
    f.seekg(0, std::ios::beg);

    // ------------------------------
    // Read (m_content.size() - 1) characters. By doing so, the file pointer is
    // pointing at the last but one character and has not reached EOF.

    // Pre-conditions
    EXPECT_EQ(f.tellg(), 0);

    std::memset(buf, 0, sizeof(buf));
    // `read` tries to read the number of characters as specified exactly.
    f.read(buf, m_content.size() - 1);

    // Post-conditions
    EXPECT_EQ(std::string(buf), m_content.substr(0, m_content.size() - 1));
    EXPECT_EQ(f.tellg(), m_content.size() - 1);
    EXPECT_FALSE(f.bad());
    EXPECT_FALSE(f.eof());
    EXPECT_FALSE(f.fail());
    EXPECT_TRUE(f.good());

    // ------------------------------
    // Try to get a whole line.

    // Pre-conditions
    EXPECT_EQ(f.tellg(), m_content.size() - 1);

    std::memset(buf, 0, sizeof(buf));
    f.getline(buf, 8);

    // Post-conditions
    // NOTE: Because now the file pointer is at EOF, calling tellg() results in
    // internal failure and failbit be set.
    EXPECT_EQ(std::string(buf), m_content.substr(m_content.size() - 1));
    EXPECT_FALSE(f.bad());
    EXPECT_TRUE(f.eof());
    EXPECT_FALSE(f.fail());
    EXPECT_FALSE(f.good());

    // ------------------------------
    // Try to operate on a stream that has reached EOF.

    // Pre-condition.
    EXPECT_TRUE(f.eof());
    EXPECT_FALSE(f.fail());

    std::memset(buf, 0, sizeof(buf));
    f.read(buf, 1);

    // Post-conditions.
    EXPECT_EQ(std::string(buf), "");
    EXPECT_FALSE(f.bad());
    EXPECT_TRUE(f.eof());
    EXPECT_TRUE(f.fail());
    EXPECT_FALSE(f.good());

    f.close();
}
