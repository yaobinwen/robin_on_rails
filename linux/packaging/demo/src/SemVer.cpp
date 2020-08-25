#include "SemVer.hpp"
#include <sstream>

std::string SemVer::to_str() const
{
    std::ostringstream oss;
    oss << this->major << "." << this->minor << "." << this->patch;
    return oss.str();
}
