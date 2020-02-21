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

dtemp_guard::dtemp_guard(bfs::path const &path, bool auto_clean)
  : m_tmp_path(path), m_auto_clean(auto_clean)
{
}

dtemp_guard::dtemp_guard(std::string const &prefix, bool auto_clean)
  : m_auto_clean(auto_clean)
{
    m_tmp_path = mkdtemp(prefix);
}

bfs::path
dtemp_guard::path() const
{
    return m_tmp_path;
}

dtemp_guard::~dtemp_guard() {}

} // namespace tmpdir
} // namespace ywen
