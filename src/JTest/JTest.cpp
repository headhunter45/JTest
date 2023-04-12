#include <JTest/JTest.h>
#include <stdexcept>
#include <string>
#include <iostream>
#include <algorithm>
#include <JTest/TestFunction.h>
#include <JTest/TestBundle.h>
#include <JTest/Test.h>
#include <JTest/TestResults.h>
#include <JTest/TestOptions.h>
#include <JTest/DescribeOptions.h>
#include <JTest/TestPendingException.h>
#include <JTest/TestFailedException.h>

namespace JTest {
    using std::ostream;
    using std::endl;
    using std::vector;
    using std::string;
    using std::runtime_error;
    using std::string;
    using std::optional;
    using std::cout;
    using std::exception;
    using std::for_each;

    void throw_unimplemented_function_error(string method_name) {
        throw runtime_error("Unimplemented function: " + method_name);
    }

    void throw_unimplemented_method_error(string class_name, string method_name) {
        throw runtime_error("Unimplemented method in class " + class_name + ": " + method_name);
    }

    void throw_unimplemented_feature_error(string feature_name) {
        throw runtime_error("Unimplemented feature: " + feature_name);
    }

    void print_current_function(ostream & out, string function_name = __builtin_FUNCTION()) {
        out << "\033[32m" << function_name << "\033[39m" << endl;
    }

    TestBundle it(const string& label, const test_fn& test_method, optional<TestOptions> options) {
        // TODO: Stop ignoring options.
        // TODO: Return a Test instead of a TestBundle.
        Test test;
        test.disabled_ = false;
        test.label_ = label;
        test.test_method_ = test_method;
        return TestBundle("", {test});
    }

    TestBundle xit(const string& label, const test_fn& test_method, optional<TestOptions> options) {
        TestBundle bundle = it(label, test_method, options);
        bundle.tests_.at(0).disabled_ = true;
        return bundle;
    }

    TestResults execute(TestBundle bundle, const string& path) {
        TestResults results;
        // TODO: Set this during bundle creation or parsing.
        // bundle.fully_qualified_path_ = path.length() > 0 ? path + "::" : "";
        // cout << "executing TestBundle: " << bundle.fully_qualified_label() << endl;
        
        if (bundle.disabled_) { 
            // TODO: Recursively report all tests as skipped.
        } else {
            try {
                // TODO: Try to do this in parallel where possible
                if (bundle.beforeAll_.has_value()) {
                    bundle.beforeAll_.value()();
                }
                for_each(bundle.children_.begin(), bundle.children_.end(), [&results, &bundle](TestBundle child) {
                    // TODO: Find a way to make child tests get our beforeEach and afterEach callbacks without modifying child.
                    child.beforeEach_ = combine(bundle.beforeEach_, child.beforeEach_);
                    child.afterEach_ = combine(bundle.afterEach_, child.afterEach_);
                    child.fully_qualified_path_ = bundle.fully_qualified_label();
                    results += execute(child);
                });
                // TODO: Consider capturing these callbacks differently. Without the bundle? By value?
                for_each(bundle.tests_.begin(), bundle.tests_.end(), [&results, &bundle](Test test) {
                    if (bundle.beforeEach_.has_value()) {
                        bundle.beforeEach_.value()();
                    }
                    results += execute(test, bundle.fully_qualified_label());
                    if (bundle.afterEach_.has_value()) {
                        bundle.afterEach_.value()();
                    }
                });
                if (bundle.afterAll_.has_value()) {
                    bundle.afterAll_.value()();
                }
            } catch(const exception& ex) {
                // TODO: This should include the fully qualified test name.
                cout << "🔥 Unhandled exception running test bundle: " << bundle.label_ << " what: " << ex.what() << endl;
                results.failed++;
            } catch(...) {
                // TODO: This should include the fully qualified test name.
                cout << "🔥 Unhandled exception running test bundle: " << bundle.label_ << endl;
                results.failed++;
            }
        }

        return results;
    }

    TestResults execute(Test test, const string& bundle_label) {
        // cout << "executing Test:" << endl
        //      << "    bundle_label = " << bundle_label << endl
        //      << "    test.label = " << test.label_ << endl;
        TestResults results;
        results.total++;
        int status = 0;
        if (test.disabled_) {
            results.skipped++;
        } else {
            try {
                test.test_method_();
                results.passed++;
            } catch (const TestPendingException& ex) {
                results.skipped++;
                // TODO: This should be ex.fully_qualified_test_label() but that isn't set yet.
                cout << "🚧 Pending Test: " << bundle_label + "::" + test.label_ << endl;
            } catch (const TestFailedException& ex) {
                results.failed++;
                // TODO: This should be ex.fully_qualified_test_label() but that isn't set yet.
                cout << "❌ Failed Test: " << bundle_label + "::" + test.label_ << " reason: " << ex.reason() << endl;
            } catch(const exception& ex) {
                results.failed++;
                // TODO: This should include the fully qualified test name.
                cout << "🔥 Unhandled exception running test: " << bundle_label + "::" + test.label_ << " what: " << ex.what() << endl;
            } catch(...) {
                results.failed++;
                // TODO: This should include the fully qualified test name.
                cout << "🔥 Unhandled exception running test: " << bundle_label + "::" + test.label_ << endl;
            }
            // throw; can still escape these catch clauses because it calls std::terminate and we can't catch/interrupt that.
        }
        return results;
    }

    TestBundle describe(const string& label, const make_testbundle_fn& maketests_fn, optional<DescribeOptions> options) {
        TestBundle bundle = maketests_fn();
        // vector<TestBundle> newChildren;
        // for_each(bundle.children_.begin(), bundle.children_.end(), [&bundle, &newChildren](TestBundle& child) {
        //     if (child.label_.length() == 0 && !child.afterAll_.has_value() && !child.afterEach_.has_value()
        //         && !child.beforeAll_.has_value() && !child.beforeEach_.has_value()) {
        //             for_each(child.tests_.begin(), child.tests_.end(), [&bundle](Test test) {
        //                 bundle.tests_.push_back(test);
        //             });
        //     } else {
        //         newChildren.push_back(bundle);
        //     }
        // });
        // bundle.children_ = newChildren;

        bundle.label_ = label;
        bundle.disabled_ = false;
        if (options.has_value()) {
            DescribeOptions options_v = options.value();
            bundle.afterAll_ = combine(options_v.getAfterAll(), bundle.afterAll_);
            bundle.afterEach_ = combine(options_v.getAfterEach(), bundle.afterEach_);
            bundle.beforeAll_ = combine(options_v.getBeforeAll(), bundle.beforeAll_);
            bundle.beforeEach_ = combine(options_v.getBeforeEach(), bundle.beforeEach_);
        }
        // TODO: go through all child bundles and tests to update the fully qualified path/label
        // cout << "describe: bundle_label = " << bundle.label_
        //      << ", bundle.tests.size() = " << bundle.tests_.size()
        //      << ", bundle.children.size() = " << bundle.children_.size() << endl;
        return bundle;
    }

    void fail(const std::string& reason) {
        // TODO: Figure out how to get label, and path
        string label = "";
        string path = "";
        throw TestFailedException(label, path, reason);
    }

    void pending(const std::string& reason) {
        // TODO: Figure out how to get label, and path
        string label = "";
        string path = "";
        throw TestPendingException(label, path, reason);
    }

    // TODO: Use these to make the unimplemented_* errors simpler to call.
    // For this function
    // TestBundle describe(const std::string& label, const make_testbundle_fn& maketests_, std::optional<DescribeOptions> options)
    // __PRETTY_FUNCTION__
    // Unimplemented function: JTest::TestBundle JTest::describe(const std::string &, const JTest::make_testbundle_fn &, std::optional<DescribeOptions>)
    // __FUNCSIG__ is not defined on clang++
    // __func__
    // describe
    // __LINE__ is an integer
    // 
    // __FILE__
    // examples/JSTest.cpp
    // __FUNCTION__
    // describe
}
