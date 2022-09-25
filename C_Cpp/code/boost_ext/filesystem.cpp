#include "filesystem.hpp"
#include <map>
#include <string>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>

namespace bfs = boost::filesystem;

namespace
{
const std::map<bfs::file_type, std::string> FILE_TYPE_NAMES = {
    { bfs::status_error, "status_error" },
    { bfs::file_not_found, "file_not_found" },
    { bfs::regular_file, "regular_file" },
    { bfs::directory_file, "directory_file" },
    { bfs::symlink_file, "symlink_file" },
    { bfs::block_file, "block_file" },
    { bfs::character_file, "character_file" },
    { bfs::fifo_file, "fifo_file" },
    { bfs::socket_file, "socket_file" },
    { bfs::type_unknown, "type_unknown" },
};
} // anonymous namespace

namespace ywen
{
namespace boost_ext
{

std::string
file_type_name(bfs::file_type ft)
{
    return FILE_TYPE_NAMES.at(ft);
}

std::string
str(bfs::file_status const &st)
{
    boost::format fmt("file_status<type=%1% perms=%2%>");
    return boost::str(
        fmt % boost_ext::file_type_name(st.type()) % st.permissions());
}

} // namespace boost_ext
} // namespace ywen
