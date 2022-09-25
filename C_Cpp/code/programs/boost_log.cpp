#include <boost/algorithm/string.hpp>
#include <boost/core/null_deleter.hpp>
#include <boost/format.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/attributes/scoped_attribute.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/smart_ptr.hpp>
#include <gtest/gtest.h>

#include <sstream>
#include <string>
#include <vector>
#include <utility>

namespace attrs = boost::log::attributes;
namespace expr = boost::log::expressions;
namespace logging = boost::log;
namespace sinks = boost::log::sinks;
namespace src = boost::log::sources;

// We define our own severity levels
enum severity_level
{
    info,
    warning,
    error
};

const char *SEVERITY_NAME[] = { "info", "warning", "error" };

// https://www.boost.org/doc/libs/1_65_0/libs/log/example/doc/tutorial_attributes.cpp
// The operator puts a human-friendly representation of the severity level to
// the stream.
std::ostream &
operator<<(std::ostream &strm, severity_level level)
{
    if (static_cast<std::size_t>(level) <
        sizeof(SEVERITY_NAME) / sizeof(*SEVERITY_NAME))
        strm << SEVERITY_NAME[level];
    else
        strm << static_cast<int>(level);

    return strm;
}

BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", severity_level)
BOOST_LOG_ATTRIBUTE_KEYWORD(tag_attr, "Tag", std::string)
BOOST_LOG_ATTRIBUTE_KEYWORD(scope, "Scope", attrs::named_scope::value_type)

struct BoostLogTextFacilities
{
    std::ostringstream oss;

    boost::shared_ptr<std::ostream> oss_ptr;
    boost::shared_ptr<sinks::text_ostream_backend> backend;

    typedef sinks::synchronous_sink<sinks::text_ostream_backend> text_sink_t;
    boost::shared_ptr<text_sink_t> sink;
};

class Test_boost_log : public ::testing::Test
{
protected:
    void
    SetUp() override
    {
        // NOTE(ywen): `init_logging()` modifies the global object
        // `logging::core` by adding a new sink which will become a global
        // object, too. Therefore, we must make sure `init_logging()` is only
        // run once. Otherwise, multiple sinks would be added. To achieve this,
        // we have several options:
        // - 1). Implement a global method that is only run once during the
        //      entire lifecycle of the program.
        // - 2). Run `init_logging()` in `SetUpTestSuite()` which **usually**
        //      is run just once. However, `googletest` document has clearly
        //      said that "Note that `SetUpTestSuite()` may be called multiple
        //      times for a test fixture class that has derived classes". So
        //      this option doesn't look as appealing as it does at first
        //      glance. If we want to go this way, we'd still need to implement
        //      an "uninit_logging()" method in case `SetUpTestSuite()` is
        //      called multiple times.
        // - 3). Run `init_logging()` in `SetUp()` and `uninit_logging()` in
        //      `TearDown()`, as what is implemented here. Note that googletest
        //      creates a new test fixture object for each test that's defined
        //      with `TEST_F`. Therefore, we must make sure that resources
        //      that are added to the global `logging::core` do not outlive the
        //      life span of a particular test fixture object.
        this->init_logging();
        this->clear_logs();
    }

    void
    TearDown() override
    {
        this->uninit_logging();
    }

    std::string
    current() const
    {
        return m_log_facilities.oss.str();
    }

    std::vector<std::string>
    current_split() const
    {
        std::vector<std::string> lines;
        std::string logs(m_log_facilities.oss.str());
        boost::algorithm::split(lines, logs, boost::is_any_of("\n"));
        return lines;
    }

    void
    clear_logs()
    {
        // NOTE(ywen): The method `clear()` sets the stream error state flags.
        // It doesn't clear the content.
        m_log_facilities.oss.str("");
    }

private:
    void
    init_logging()
    {
        logging::add_common_attributes();

        boost::shared_ptr<logging::core> core = logging::core::get();

        // Create a backend.
        m_log_facilities.backend =
            boost::make_shared<sinks::text_ostream_backend>();

        // Create an output stream which will be attached to the backend.
        m_log_facilities.oss_ptr = boost::shared_ptr<std::ostream>(
            &m_log_facilities.oss, boost::null_deleter());

        // Attach the output stream to the backend.
        m_log_facilities.backend->add_stream(m_log_facilities.oss_ptr);

        // Enable auto-flushing after each log record written
        m_log_facilities.backend->auto_flush(true);

        // Wrap it into the frontend and register in the core.
        // The backend requires synchronization in the frontend.
        m_log_facilities.sink =
            boost::make_shared<BoostLogTextFacilities::text_sink_t>(
                m_log_facilities.backend);

        // Set the formatter.
        m_log_facilities.sink->set_formatter(
            expr::stream
            << expr::if_(expr::has_attr(
                   severity))[expr::stream << "<" << severity << ">"]
            << expr::if_(
                   expr::has_attr(scope))[expr::stream << "(" << scope << ")"]
            << expr::if_(expr::has_attr(
                   tag_attr))[expr::stream << "[" << tag_attr << "]"]
            << expr::smessage);

        core->add_sink(m_log_facilities.sink);
    }

    void
    uninit_logging()
    {
        boost::shared_ptr<logging::core> core = logging::core::get();
        core->remove_sink(m_log_facilities.sink);
    }

    BoostLogTextFacilities m_log_facilities;
};

TEST_F(Test_boost_log, test_trivial_logging_to_string)
{
    std::vector<std::string> messages;

    std::string msg_trace("trace message");
    BOOST_LOG_TRIVIAL(trace) << msg_trace;

    messages = this->current_split();
    // BOOST_LOG_XXX macros append '\n' at the end of the log messages, so
    // `logs` can be split into two parts with the first part being equal
    // to `msg` while the second part is an empty string.
    EXPECT_EQ(messages.size(), 2U);
    EXPECT_EQ(messages[0], msg_trace);
    EXPECT_EQ(messages[1], "");

    std::string msg_info("info message");
    BOOST_LOG_TRIVIAL(info) << msg_info;

    messages = this->current_split();
    EXPECT_EQ(messages.size(), 3U);
    EXPECT_EQ(messages[0], msg_trace);
    EXPECT_EQ(messages[1], msg_info);
    EXPECT_EQ(messages[2], "");

    this->clear_logs();

    std::string msg_error("error message");
    BOOST_LOG_TRIVIAL(error) << msg_error;

    messages = this->current_split();
    EXPECT_EQ(messages.size(), 2U);
    EXPECT_EQ(messages[0], msg_error);
    EXPECT_EQ(messages[1], "");
}

TEST_F(Test_boost_log, test_logger_logging_to_string)
{
    std::vector<std::string> messages;

    src::logger_mt lg;

    std::string msg_hello("Hello");
    BOOST_LOG(lg) << msg_hello;

    messages = this->current_split();
    EXPECT_EQ(messages.size(), 2U);
    EXPECT_EQ(messages[0], msg_hello);
    EXPECT_EQ(messages[1], "");

    std::string msg_world("world");
    BOOST_LOG(lg) << msg_world;

    messages = this->current_split();
    EXPECT_EQ(messages.size(), 3U);
    EXPECT_EQ(messages[0], msg_hello);
    EXPECT_EQ(messages[1], msg_world);
    EXPECT_EQ(messages[2], "");

    this->clear_logs();

    std::string msg_greeting("ola mundo");
    BOOST_LOG(lg) << msg_greeting;

    messages = this->current_split();
    EXPECT_EQ(messages.size(), 2U);
    EXPECT_EQ(messages[0], msg_greeting);
    EXPECT_EQ(messages[1], "");
}

TEST_F(Test_boost_log, test_attribute_scope)
{
    // We don't add the "Scope" attribute until we are running this test. If we
    // had added it in `init_logging()`, the log messages in all the tests
    // would have the "scope" attribute and I would need to adjust the
    // assertions accordingly.
    std::pair<logging::attribute_set::iterator, bool> pos =
        logging::core::get()->add_global_attribute(
            "Scope", attrs::named_scope());

    BOOST_LOG_NAMED_SCOPE("test_attribute_scope");

    std::vector<std::string> messages;

    src::logger lg;
    std::string msg_hello("Hello");
    BOOST_LOG(lg) << msg_hello;

    messages = this->current_split();
    EXPECT_EQ(messages.size(), 2U);
    EXPECT_EQ(messages[0], "(test_attribute_scope)Hello");
    EXPECT_EQ(messages[1], "");

    // NOTE(ywen): Need to remove the attribute "Scope" because it is added as
    // a global one, otherwise all the subsequently run tests would have the
    // "scope" part in their log messages.
    logging::core::get()->remove_global_attribute(pos.first);
}

TEST_F(Test_boost_log, test_attribute_severity)
{
    std::vector<std::string> messages;

    // NOTE(ywen): Somehow, `src::severity_logger slg` can also pass the
    // compilation but doesn't produce the expected results, i.e., that the
    // log messages don't have the severity part.
    src::severity_logger<severity_level> slg;
    std::string msg_warning("Low fuel");
    BOOST_LOG_SEV(slg, warning) << msg_warning;

    messages = this->current_split();
    EXPECT_EQ(messages.size(), 2U);
    EXPECT_EQ(
        messages[0],
        boost::str(
            boost::format("<%1%>%2%") % SEVERITY_NAME[warning] % msg_warning));
    EXPECT_EQ(messages[1], "");
}

TEST_F(Test_boost_log, test_attribute_tag)
{
    std::vector<std::string> messages;

    src::logger lg;
    std::string tag("test_attribute_tag");
    lg.add_attribute("Tag", attrs::constant<std::string>(tag));

    std::string msg_hello("Hello");
    BOOST_LOG(lg) << msg_hello;

    messages = this->current_split();
    EXPECT_EQ(messages.size(), 2U);
    EXPECT_EQ(
        messages[0], boost::str(boost::format("[%1%]%2%") % tag % msg_hello));
    EXPECT_EQ(messages[1], "");
}
