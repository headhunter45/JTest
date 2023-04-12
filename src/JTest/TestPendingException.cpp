#include <JTest/TestPendingException.h>
#include <string>
#include <JTest/TestStatus.h>
#include <JTest/TestException.h>

namespace JTest {
    using std::string;

    TestPendingException::TestPendingException(const string& label, const string& path, const string& reason)
    : TestException(label, path, "Pending Test: " + fully_qualified_test_label()) {}
    
    TestPendingException::~TestPendingException() {}

    TestStatus TestPendingException::status() const {
        return TestStatus::Skipped;
    }
}
