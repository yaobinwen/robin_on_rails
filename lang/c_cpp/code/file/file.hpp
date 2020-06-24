#pragma once

#include <string>
#include <boost/filesystem.hpp>

#include "../external/json.hpp"

namespace ywen
{
namespace file
{

/// Create a text file with the given content.
void
create_file(
    boost::filesystem::path const &fpath,
    std::string const &content = "");

/// Create a whole hierarchy of directory under `base_path`.
void
create_dir_hierarchy(
    boost::filesystem::path const &base_path,
    ::nlohmann::json const &hierarcy);

} // namespace file
} // namespace ywen
