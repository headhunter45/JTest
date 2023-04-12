#ifndef JTEST__TESTFAILEDEXCEPTION_H__
#define JTEST__TESTFAILEDEXCEPTION_H__
namespace JTest{
    class TestFailedException;
}
#include <string>
#include <JTest/TestStatus.h>
#include <JTest/TestException.h>
namespace JTest{
    class TestFailedException: public TestException {
        public:
            TestFailedException(std::string label, std::string path, std::string reason);
            virtual ~TestFailedException();
            virtual TestStatus status() const;
            virtual std::string reason() const;
        private:
            std::string reason_;
    };
}
#endif
