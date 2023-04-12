#ifndef JTEST__TESTPENDINGEXCEPTION_H__
#define JTEST__TESTPENDINGEXCEPTION_H__
namespace JTest {
    class TestPendingException;
}
#include <JTest/TestException.h>
#include <JTest/TestStatus.h>

namespace JTest {
    class TestPendingException: public TestException {
        public:
            TestPendingException(const std::string& label, const std::string& path, const std::string& reason);
            virtual ~TestPendingException();
            virtual TestStatus status() const;
    };
}
#endif
