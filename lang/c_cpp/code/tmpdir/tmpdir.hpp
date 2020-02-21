#pragma once

#include <string>
#include <boost/filesystem.hpp>

namespace ywen
{
namespace tmpdir
{

const std::string DEFAULT_TMP_DIR_PREFIX = "ywen";

/// Create a temporary directory with the given prefix.
boost::filesystem::path
mkdtemp(std::string const &prefix = DEFAULT_TMP_DIR_PREFIX);

/// Automatically remove the temporary directory on destruction.
class dtemp_guard
{
public:
    /// Manage a temporary directory that is created by someone else.
    /// "auto_clean" being false is usually for debugging purpose.
    explicit dtemp_guard(
        boost::filesystem::path const &path,
        bool auto_clean = true);

    /// Create a temporary directory and manage it. The path of the created
    /// directory can be obtained via `path()`.
    explicit dtemp_guard(
        std::string const &prefix = DEFAULT_TMP_DIR_PREFIX,
        bool auto_clean = true);

    /// Return the path of the managed temporary directory.
    boost::filesystem::path
    path() const;

    ~dtemp_guard();

protected:
    boost::filesystem::path m_tmp_path;
    bool m_auto_clean;
};

} // namespace tmpdir
} // namespace ywen
