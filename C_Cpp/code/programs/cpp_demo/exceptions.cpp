#include <boost/algorithm/string/join.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/process.hpp>
#include <gtest/gtest.h>

#include <algorithm>
#include <sstream>
#include <stdexcept>
#include <string>

namespace bfs = boost::filesystem;

/// Thrown when the given program is not found on the system.
class ProgramNotFound : public std::runtime_error
{
public:
    /// Constructor
    /// @param[in] name
    ///     The name of the program that is not found on the system.
    /// @param[in] search_paths
    ///     The paths in which the program is searched.
    ProgramNotFound(
        std::string const &name,
        std::vector<bfs::path> const &search_paths);

    /// Return the explanatory message.
    virtual const char *
    what() const noexcept override;

    /// Return the name of the program.
    std::string
    name() const;

    /// Return the search paths.
    std::vector<bfs::path>
    search_paths() const;

private:
    std::string m_name;
    std::vector<bfs::path> m_search_paths;
    std::string m_msg;
};

ProgramNotFound::ProgramNotFound(
    std::string const &name,
    std::vector<bfs::path> const &search_paths)
    // Because we are going to override `what()`, we don't have to provide
    // a string to `std::runtime_error`.
  : std::runtime_error(""), m_name(name), m_search_paths(search_paths)
{
    std::vector<std::string> paths;
    paths.resize(m_search_paths.size());

    std::transform(
        m_search_paths.begin(),
        m_search_paths.end(),
        paths.begin(),
        [](bfs::path const &p) { return p.string(); });

    std::string s = boost::algorithm::join(paths, ", ");

    m_msg =
        str(boost::format("program %1% is not found in the path(s) %2%") %
            m_name % s);
}

std::string
ProgramNotFound::name() const
{
    return m_name;
}

std::vector<bfs::path>
ProgramNotFound::search_paths() const
{
    return m_search_paths;
}

const char *
ProgramNotFound::what() const noexcept
{
    return m_msg.c_str();
}

TEST(ExceptionTest, runtime_error_subclass)
{
    try
    {
        std::vector<bfs::path> search_paths = { "path1", "path2", "path3" };

        throw ProgramNotFound("my_program", search_paths);
    }
    catch (std::runtime_error const &ex)
    {
        std::string msg(ex.what());
        EXPECT_TRUE(msg.find("my_program") != std::string::npos);
        EXPECT_TRUE(msg.find("path1") != std::string::npos);
        EXPECT_TRUE(msg.find("path2") != std::string::npos);
        EXPECT_TRUE(msg.find("path3") != std::string::npos);
        EXPECT_TRUE(
            msg.find("is not found in the path(s)") != std::string::npos);
    }
}
