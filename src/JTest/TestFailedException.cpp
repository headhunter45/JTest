#include <JTest/TestFailedException.h>
#include <string>
#include <JTest/TestException.h>
#include <JTest/TestStatus.h>

namespace JTest {
    using std::string;

    TestFailedException::TestFailedException(string label, string path, string reason)
    : TestException(label, path, "Failed Test: " +fully_qualified_test_label())
    , reason_(reason) {}

    TestFailedException::~TestFailedException() {}

    TestStatus TestFailedException::status() const {
        return TestStatus::Failed;
    }

    string TestFailedException::reason() const {
        return reason_;
    }
}
