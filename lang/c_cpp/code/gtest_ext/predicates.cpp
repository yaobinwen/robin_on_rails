#include "predicates.hpp"

#include <boost/format.hpp>

namespace ywen::gtest_ext
{

::testing::AssertionResult
IsSubstr(std::string const &str, std::string const &substr)
{
    boost::format fmt("str = \"%1%\"; substr = \"%2%\"");
    std::string dbg_msg(boost::str(fmt % str % substr));

    ::testing::AssertionResult r(
        str.find(substr) != std::string::npos ? ::testing::AssertionSuccess()
                                              : ::testing::AssertionFailure());
    return r << dbg_msg;
}

} // namespace ywen::gtest_ext
