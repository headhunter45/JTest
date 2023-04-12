#ifndef JEST__TESTBUNDLE_H__
#define JEST__TESTBUNDLE_H__
#include <functional>
namespace JTest {
    class TestBundle;
    typedef std::function<TestBundle()> make_testbundle_fn;
}
#include <vector>
#include <optional>
#include <string>
#include <JTest/ConfigureFunction.h>
#include <JTest/DescribeOptions.h>
#include <JTest/TestBundle.h>
#include <JTest/Test.h>
namespace JTest {
    class TestBundle {
        public:
            TestBundle(const std::vector<TestBundle>& bundles, const DescribeOptions& options);
            TestBundle(const std::string& label, const std::vector<Test>& tests);
            std::string fully_qualified_label() const;
            std::string label_;
            std::string fully_qualified_path_;
            std::vector<Test> tests_;
            std::vector<TestBundle> children_;
            maybe_configure_fn afterAll_;
            maybe_configure_fn afterEach_;
            maybe_configure_fn beforeAll_;
            maybe_configure_fn beforeEach_;
            bool disabled_;
        private:
    };
}
#endif
