#pragma once

#include "SemVer.hpp"

/// Demo how Debian packaging works.
class Demo
{
public:
    /// Return the name of the demo.
    std::string name() const;

    /// Return the version of the demo.
    SemVer version() const;
};
