#ifndef JTEST__FAILEDEXPECTATION_H__
#define JTEST__FAILEDEXPECTATION_H__
namespace JTest {
    class FailedExpectation;
}
#include <string>
#include <JTest/TestFailedException.h>
namespace JTest {
    class FailedExpectation: public TestFailedException {
        public:
            FailedExpectation(const std::string& label);
            FailedExpectation(const std::string& label, const std::string& path, const std::string& reason);
            virtual ~FailedExpectation();
    };
}
#endif
