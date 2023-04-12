#ifndef JTEST__TESTFUNCTION_H__
#define JTEST__TESTFUNCTION_H__
#include <functional>
namespace JTest {
    typedef std::function<void()> test_fn;
}
#endif
