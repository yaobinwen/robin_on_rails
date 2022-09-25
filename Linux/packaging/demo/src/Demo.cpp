#include "Demo.hpp"

std::string Demo::name() const
{
    return "deb-packaging-demo";
}

SemVer Demo::version() const
{
    return SemVer(1, 0, 0);
}
