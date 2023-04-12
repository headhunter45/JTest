#include <JTest/TestException.h>
#include <string>

namespace JTest {
    using std::string;

    TestException::TestException(const string& label, const string& path)
    : runtime_error("Exception in test: " + path + "::" + label)
    , test_label_(label)
    , fully_qualified_test_label_(path + "::" + label)
    , function_("")
    , file_("")
    , line_(-1)
    , reason_("") {}

    TestException::TestException(const string& label, const string& path, const string& what)
    : runtime_error(what)
    , test_label_(label)
    , fully_qualified_test_label_(path + "::" + label)
    , function_("")
    , file_("")
    , line_(-1)
    , reason_("") {}

    TestException::TestException(const std::string& function, const std::string& file, int line, const std::string& reason)
    : runtime_error(reason)
    , test_label_("")
    , fully_qualified_test_label_("")
    , function_(function)
    , file_(file)
    , line_(line)
    , reason_(reason) {}

    TestException::~TestException() {}

    string TestException::test_label() const {
        return test_label_;
    }

    string TestException::fully_qualified_test_label() const {
        return fully_qualified_test_label_;
    }

    TestStatus TestException::status() const {
        return TestStatus::Unknown;
    }
}
