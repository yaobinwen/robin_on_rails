#pragma once

#include <cstddef>
#include <string>

/// Simple semantic version.
/// For a better implementation, see: https://github.com/Neargye/semver
struct SemVer
{
    std::size_t major;
    std::size_t minor;
    std::size_t patch;

    /// Constructor.
    SemVer(std::size_t major = 0, std::size_t minor = 1, std::size_t patch = 0)
        : major{major}, minor{minor}, patch{patch}
    {
        // Empty
    }

    /// Convert to a string representation.
    std::string to_str() const;
};
