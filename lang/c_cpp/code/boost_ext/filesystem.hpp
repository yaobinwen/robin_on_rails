#pragma once

#include <string>
#include <boost/filesystem.hpp>

namespace ywen
{
namespace boost_ext
{

std::string
file_type_name(boost::filesystem::file_type ft);

std::string
str(boost::filesystem::file_status const &st);

} // namespace boost_ext
} // namespace ywen
