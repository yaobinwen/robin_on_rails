#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/process.hpp>
#include <gtest/gtest.h>
#include <stdexcept>
#include <string>
#include <vector>

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

bfs::path
search_program_path(
    std::string const &prog_name,
    std::vector<bfs::path> const &search_paths = boost::this_process::path())
{
    bfs::path p = bp::search_path(prog_name, search_paths);
    if (p.empty())
    {
        throw std::runtime_error(
            str(boost::format("Cannot find program %1%") % prog_name));
    }

    return p;
}

TEST(Test_boost_process, test_get_os_codename)
{
    bfs::path prog_lsb_release = search_program_path("lsb_release");

    bp::ipstream is_codename;
    bp::child lsb_release(
        prog_lsb_release, "--codename", "--short", bp::std_out > is_codename);
    if (lsb_release.exit_code() != 0)
    {
        throw std::runtime_error(
            str(boost::format("%1% failed with exit code %2%") %
                prog_lsb_release.string() %
                lsb_release.exit_code()));
    }
    std::string os_codename;
    std::getline(is_codename, os_codename);
}

TEST(Test_boost_process, test_ignore_output)
{
    bfs::path prog_ping = search_program_path("ping");

    bp::child ping(
        prog_ping,
        "-c",
        "1",
        "127.0.0.1",
        bp::std_out > bp::null,
        bp::std_err > bp::null
    );
    if (ping.exit_code() != 0)
    {
        throw std::runtime_error(
            str(boost::format("%1% failed with exit code %2%") %
                prog_ping.string() %
                ping.exit_code()));
    }
}
