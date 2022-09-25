#pragma once

#include <string>

#include <gtest/gtest.h>

namespace ywen::gtest_ext
{

::testing::AssertionResult
IsSubstr(std::string const &str, std::string const &substr);

} // namespace ywen::gtest_ext
