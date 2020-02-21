#include "file.hpp"
#include <fstream>

namespace bfs = boost::filesystem;

namespace ywen
{
namespace file
{

void
create_file(bfs::path const &fpath, std::string const &content)
{
    std::ofstream f;
    f.open(fpath.string());
    f << content;
    f.close();
}

} // namespace file
} // namespace ywen
