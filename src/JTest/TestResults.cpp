#include <JTest/TestResults.h>

namespace JTest {
    TestResults::TestResults()
    : total(0)
    , skipped(0)
    , passed(0)
    , failed(0) {}

    TestResults::TestResults(uint32_t total, uint32_t skipped, uint32_t passed, uint32_t failed)
    : total(total)
    , skipped(skipped)
    , passed(passed)
    , failed(failed) {}

    TestResults TestResults::operator+(const TestResults& right) const {
        return TestResults(
            total + right.total, skipped + right.skipped,
            passed + right.passed, failed + right.failed);
    }

    TestResults& TestResults::operator+=(const TestResults& right) {
        total += right.total;
        skipped += right.skipped;
        passed += right.passed;
        failed += right.failed;
        return *this;
    }

    std::ostream& operator<<(std::ostream& os, const TestResults& results) {
        os << "Total: " << results.total << ", Passed: " << results.passed
           << ", Failed: " << results.failed << ", Skipped: " << results.skipped;
        return os;
    }
}
