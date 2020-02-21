#pragma once

#include <string>
#include <boost/filesystem.hpp>

namespace ywen
{
namespace file
{

/// Create a text file with the given content.
void
create_file(
    boost::filesystem::path const &fpath,
    std::string const &content = "");

} // namespace file
} // namespace ywen
