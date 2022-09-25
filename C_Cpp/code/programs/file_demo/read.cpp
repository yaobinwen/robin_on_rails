// This program demonstrates how to read a file.

#include "../../file/file.hpp"
#include "../../tmpdir/tmpdir.hpp"

#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <gtest/gtest.h>

#include <cstdio> // for std::fopen
#include <fstream>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <string>

namespace bfs = boost::filesystem;

class BinaryFileTest : public ::testing::Test
{
protected:
    BinaryFileTest()
      : m_tmpDir(ywen::tmpdir::mkdtemp("FileTest")),
        m_tmpDirGuard(m_tmpDir),
        m_content("hello\nworld"),
        m_file(m_tmpDir / "file.txt")
    {
        std::ofstream f;
        f.open(m_file.string(), std::ios::out | std::ios::binary);
        f << m_content; // Do not insert a newline.
        f.close();
    }

    bfs::path m_tmpDir;
    ywen::tmpdir::dtemp_guard m_tmpDirGuard;
    std::string m_content;
    bfs::path m_file;
};

TEST_F(BinaryFileTest, use_C_API)
{
    std::string content;
    const char *filename = m_file.string().c_str();

    std::FILE *fp = std::fopen(filename, "rb");
    ASSERT_NE(fp, nullptr);

    int ret = std::fseek(fp, 0, SEEK_END);
    ASSERT_EQ(ret, 0);

    long pos = std::ftell(fp);
    ASSERT_NE(pos, -1L);

    content.resize(static_cast<size_t>(pos));

    std::rewind(fp);
    size_t actualSize = std::fread(&content[0], 1, content.size(), fp);
    EXPECT_EQ(actualSize, content.size());

    // "Whether or not the operation succeeds, the stream is no longer
    // associated with a file"
    // https://en.cppreference.com/w/cpp/io/c/fclose
    ret = std::fclose(fp);
    EXPECT_EQ(ret, 0);

    EXPECT_EQ(content, m_content);
}

TEST_F(BinaryFileTest, use_Cpp_fstream)
{
    std::string content;
    std::string filename = m_file.string();

    std::ifstream in(filename, std::ios::in | std::ios::binary);
    ASSERT_TRUE(in);

    in.seekg(0, std::ios::end);
    ASSERT_TRUE(in);

    std::ifstream::pos_type pos = in.tellg();
    ASSERT_NE(pos, std::ifstream::pos_type(-1));
    content.resize(pos);

    in.seekg(0, std::ios::beg);
    ASSERT_TRUE(in);

    in.read(&content[0], content.size());
    ASSERT_TRUE(in);
    EXPECT_EQ(in.gcount(), content.size());

    in.close();
    EXPECT_TRUE(in);

    EXPECT_EQ(content, m_content);
}

TEST_F(BinaryFileTest, use_Cpp_istreambuf_iterator)
{
    std::string content;
    std::string filename = m_file.string();

    std::ifstream in(filename, std::ios::in | std::ios::binary);
    ASSERT_TRUE(in);

    content.assign(
        std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
    EXPECT_TRUE(in);

    EXPECT_EQ(content, m_content);
}

TEST_F(BinaryFileTest, use_Cpp_rdbuf)
{
    std::ostringstream content;
    std::string filename = m_file.string();

    std::ifstream in(filename, std::ios::in | std::ios::binary);
    ASSERT_TRUE(in);

    // `rdbuf()` returns an `std::basic_streambuf`. `std::basic_ostream` has
    // the `operator <<` that takes an `std::basic_streambuf` as the input.
    content << in.rdbuf();
    ASSERT_TRUE(in);

    in.close();
    EXPECT_TRUE(in);

    EXPECT_EQ(content.str(), m_content);
}
