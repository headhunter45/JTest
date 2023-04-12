#ifndef JTEST__TEST_H__
#define JTEST__TEST_H__
namespace JTest {
    class Test;
}
#include <string>
#include <JTest/TestFunction.h>
namespace JTest {
    class Test {
        public:
            std::string label_;
            test_fn test_method_;
            bool disabled_;
        private:
    };
}
#endif
