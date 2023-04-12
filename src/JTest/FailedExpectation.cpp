#include <JTest/FailedExpectation.h>
namespace JTest{
    FailedExpectation::~FailedExpectation() {}

    FailedExpectation::FailedExpectation(const std::string& reason)
    : TestFailedException("", "", reason) {}

    // TODO: Figure out if we can get label and path from here without passing them in.
    FailedExpectation::FailedExpectation(const std::string& label, const std::string& path, const std::string& reason)
    : TestFailedException(label, path, reason) {}
}
