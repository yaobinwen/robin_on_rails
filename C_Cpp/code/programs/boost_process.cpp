#include <boost/filesystem.hpp>
#include <boost/process.hpp>
#include <gtest/gtest.h>

namespace bfs = boost::filesystem;
namespace bp = boost::process;

TEST(Test_boost_process, test_search_path)
{
    bfs::path p;

    p = bp::search_path("wyb");
    EXPECT_TRUE(p.empty());

    p = bp::search_path("sudo");
    EXPECT_FALSE(p.empty());
}
