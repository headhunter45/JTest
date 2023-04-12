#ifndef JTEST_H__
#define JTEST_H__
namespace JTest {
}
#include <string>
#include <optional>
#include <JTest/TestResults.h>
#include <JTest/TestBundle.h>
#include <JTest/Test.h>
#include <JTest/Expectable.h>
#include <JTest/TestOptions.h>
#include <JTest/DescribeOptions.h>
#include <JTest/TestFunction.h>
#include <JTest/TestBundle.h>

namespace JTest {
    // Jasmine methods

    // Executes the tests in tests. Possibly in parallel. Will block until all async tests have completed.
    TestResults execute(TestBundle tests, const std::string& path="");
    TestResults execute(Test test, const std::string& bundle_label);

    // 
    TestBundle describe(const std::string& label, const make_testbundle_fn& make_tests, std::optional<DescribeOptions> options = std::nullopt);
    TestBundle xdescribe(const std::string& label, const make_testbundle_fn& make_tests, std::optional<DescribeOptions> options = std::nullopt);
    
    // TODO: Make this return a Test instead.
    TestBundle it(const std::string& label, const test_fn& test_method, std::optional<TestOptions> options = std::nullopt);
    TestBundle xit(const std::string& label, const test_fn& test_method, std::optional<TestOptions> options = std::nullopt);
    
    void fail(const std::string& reason);
    void pending(const std::string& reason);

    template<typename T>
    Expectable<T> expect(const T& actual) {
        return Expectable(actual);
    }
}
#endif