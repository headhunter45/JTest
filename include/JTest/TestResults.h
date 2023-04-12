#ifndef JTEST_TESTRESULTS_H__
#define  JTEST_TESTRESULTS_H__
namespace JTest {
    class TestResults;
}
#include <cstdint>
#include <ostream>
namespace JTest {
    // TODO: Consider hiding the vectors behind accessor methods void add(...), T get(), vector<T> get(uint32_t index)
    class TestResults {
        public:
            TestResults();
            TestResults(uint32_t total, uint32_t skipped, uint32_t passed, uint32_t failed);
            TestResults operator+(const TestResults& right) const;
            TestResults& operator+=(const TestResults& right);
            friend std::ostream& operator<<(std::ostream& os, const TestResults& results);
            // TODO: Make these private, make operator++ increment total, add methods pass, fail, and skip to increment passed, failed, and skipped
            uint32_t total;
            uint32_t skipped;
            uint32_t passed;
            uint32_t failed;
        private:
            // vector<TestError> errors;
            // vector<TestFailure> failures;
            // vector<TestSkip> skipped;
    };
}
#endif
