#include "tmpdir.hpp"

#include <boost/filesystem.hpp>
#include <boost/format.hpp>

namespace bfs = boost::filesystem;

namespace ywen
{
namespace tmpdir
{

bfs::path
mkdtemp(std::string const &prefix)
{
    using boost::format;
    using boost::str;

    // boost::format resolves "%%" into one '%' character, so "%%%%" is
    // resolved into two '%' characters.
    bfs::path tmp_model =
        bfs::temp_directory_path() / str(format("%1%-%%%%") % prefix);
    bfs::path dpath = bfs::unique_path(tmp_model);
    bfs::create_directory(dpath);
    return dpath;
}

dtemp_guard::dtemp_guard(
    bfs::path const &path,
    bool auto_clean,
    std::ostream &os)
  : m_tmp_path(path), m_auto_clean(auto_clean), m_os(os)
{
}

bfs::path
dtemp_guard::path() const
{
    return m_tmp_path;
}

dtemp_guard::~dtemp_guard()
{
    try
    {
        if (m_auto_clean)
        {
            bfs::remove_all(m_tmp_path);
        }
    }
    catch (boost::filesystem::filesystem_error const &e)
    {
        m_os << "dtemp_guard fails to remove " << m_tmp_path << std::endl;
    }
}

} // namespace tmpdir
} // namespace ywen
