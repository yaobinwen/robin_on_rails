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

void
create_dir_hierarchy(
    bfs::path const &base_path,
    ::nlohmann::json const &hierarcy)
{
    for (auto const &item : hierarcy.items())
    {
        std::string const &name = item.key();

        auto const &def = item.value();
        std::string type = def.at("type");

        if ("dir" == type)
        {
            bfs::create_directory(base_path / name);
            create_dir_hierarchy(base_path / name, def.at("content"));
        }
        else if ("file" == type)
        {
            std::string const &content = def.at("content");
            create_file(base_path / name, content);
        }
        else
        {
            // TODO(ywen): Throw the error, maybe.
        }
    }
}

} // namespace file
} // namespace ywen
