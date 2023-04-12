#ifndef JTEST__TESTEXCEPTION_H__
#define JTEST__TESTEXCEPTION_H__
namespace JTest {
    class TestException;
}
#include <string>
#include <stdexcept>
#include <JTest/TestStatus.h>
namespace JTest {
    class TestException: public std::runtime_error {
        public:
            TestException(const std::string& label, const std::string& path);
            TestException(const std::string& label, const std::string& path, const std::string& what);
            TestException(const std::string& function, const std::string& file, int line, const std::string& reason);
            virtual ~TestException();
            virtual std::string test_label() const;
            virtual std::string fully_qualified_test_label() const;
            virtual TestStatus status() const;
        private:
            const std::string test_label_;
            const std::string fully_qualified_test_label_;
            const std::string function_;
            const std::string file_;
            const int line_;
            const std::string reason_;
    };
}
#endif
