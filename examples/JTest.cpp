#include "JTest.h"
#include <stdexcept>

namespace JTest {
    using std::ostream;
    using std::endl;
    using std::vector;
    using std::string;
    using std::runtime_error;
    using std::string;
    using std::optional;

    runtime_error unimplemented_function_error(string method_name) {
        return runtime_error("Unimplemented function: " + method_name);
    }

    runtime_error unimplemented_method_error(string class_name, string method_name) {
        return runtime_error("Unimplemented method in class " + class_name + ": " + method_name);
    }

    runtime_error unimplemented_feature_error(string feature_name) {
        return runtime_error("Unimplemented feature: " + feature_name);
    }

    testresults_t make_testresults() {
        return {0, 0, 0, 0};
    }

    testresults_t make_testresults(uint32_t total, uint32_t skipped, uint32_t passed, uint32_t failed) {
        return {total, skipped, passed, failed};
    }

    testresults_t add(const testresults_t& l, const testresults_t& r) {
        return {l.total + r.total, l.skipped + r.skipped, l.passed + r.passed, l.failed + r.failed};
    }

    void print_test_results(const testresults_t& results, ostream& out) {
        out << "Tests: " << results.total << endl;
        out << "Failed: " << results.failed << ", Passed: " << results.passed << ", Skipped: " << results.skipped << endl; 
    }

    testresults_t operator+(const testresults_t& left, const testresults_t& right) {
        return add(left, right);
    }

    testresults_t& operator+=(testresults_t& left, const testresults_t& right) {
        left = left + right;
        return left;
    }

    testbundle_t make_testbundle(const vector<testbundle_t>& bundles, const describeoptions_t& options) {
        testbundle_t b;

        for_each(bundles.begin(), bundles.end(), [&b](testbundle_t bundle){
            if (bundle._tests.size() == 0 && bundle._children.size() == 1 && bundle._label.size() == 0) {
                b._tests.push_back(bundle._tests.at(0));
            } else {
                b._children.push_back(bundle);
            }
        });

        b._beforeEach = options.getBeforeEach();
        b._afterEach = options.getAfterEach();
        b._beforeAll = options.getBeforeAll();
        b._afterAll = options.getAfterAll();
        return b;
    }

    describeoptions_t& describeoptions_t::beforeEach(configure_fn fn) {
        this->_beforeEach = fn;
        return *this;
    }

    describeoptions_t& describeoptions_t::afterEach(configure_fn fn) {
        this->_afterEach = fn;
        return *this;
    }
    describeoptions_t& describeoptions_t::beforeAll(configure_fn fn) {
        this->_beforeAll = fn;
        return *this;
    }
    describeoptions_t& describeoptions_t::afterAll(configure_fn fn) {
        this->_afterAll = fn;
        return *this;
    }
    describeoptions_t make_describeoptions() {
        return {};
    }

    // TODO test_method is the actual test method. We should save it to the test instead of executing it.
    testbundle_t it(const string& label, const test_fn& test_method, optional<testoptions_t> options) {
        // TODO: Stop ignoring options.
        test_t test;
            test._disabled = false;
            test._label = label;
            test._test_method = test_method;
        return make_testbundle("", {test});
    }

    testresults_t execute(testbundle_t bundle) {
/*
        Try to do this in parallel where possible

        If the bundle is marked as disabled (xdescribe) report all the tests in it and child bundles as SKIPPED.
        testresults_t results;
        try {
            bundle.beforeAll();
            for each child of bundle.children
                results += execute(child)
            for each test of bundle.tests
                results += execute(test)
            bundle.afterAll();
        } catch (...) {
            Report as much info as possible. This likely means something happened in beforeAll() or afterAll().
        }
*/
        throw unimplemented_function_error("execute(testbundle_t)");
    }

    testresults_t execute(test_t test) {
/*
        status = PASSED (PASSED, FAILED, SKIPPED (PENDING | DISABLED), QUEUED, RUNNING) QUEUED and RUNNING might not make sense.
        If the test is marked as disabled (xit) then record it as a skipped test and return.
        try {
            test.beforeEach();
            run the test function
                // Successful completion of the test method is recorded as a passed test.
                // Pending tests throw a test_pending exception - These are recorded as skipped tests.
                // Failed expects throw a test_failed exception - These are recorded as failed tests.
                // Runtime errors and catchall should be caught last of all. - These are recorded as failed tests.
            test.afterEach();
            
        } catch (test_pending_exception ex) {
            status = SKIPPED;
        } catch (test_failed_exception ex) {
            status = FAILED
        } catch (exception ex) {
            status = FAILED
        } catch (...) {
            status = FAILED
        } finally {
            maybe some kind of cleanup
        }

        return make_testresults(...) // based on the status.
*/
        throw unimplemented_function_error("execute(test_t)");
    }

    optional<configure_fn> combine(optional<configure_fn> first, optional<configure_fn> second) {
        if (first.has_value()) {
            auto first_v = first.value();
            if (second.has_value()) {
                auto second_v = second.value();
                return [first_v, second_v]() {
                    first_v();
                    second_v();
                };
            } else {
                return first;
            }
        } else {
            return second;
        }
    }

    testbundle_t describe(const wstring& label, const make_testbundle_fn& make_tests, optional<describeoptions_t> options) {
        testbundle_t bundle = make_tests();
        bundle._label = label;
        if (options.has_value()) {
            describeoptions_t options_v = options.value();
            bundle._afterAll = combine(options_v.getAfterAll(), bundle._afterAll);
            bundle._afterEach = combine(options_v.getAfterEach(), bundle._afterEach);
            bundle._beforeAll = combine(options_v.getBeforeAll(), bundle._beforeAll);
            bundle._beforeEach = combine(options_v.getBeforeEach(), bundle._beforeEach);
        }
        return bundle;
    }

    testbundle_t make_testbundle(const string& label, const vector<test_t>& tests) {
        testbundle_t bundle;
        bundle._label = label;
        bundle._tests.clear();
        for_each(tests.begin(), tests.end(), [&bundle](test_t test){
            bundle._tests.push_back(test);
        });
        return bundle;
    }

    optional<configure_fn> describeoptions_t::getAfterAll() const {
        return this->_afterAll;
    }

    optional<configure_fn> describeoptions_t::getAfterEach() const {
        return this->_afterEach;
    }

    optional<configure_fn> describeoptions_t::getBeforeAll() const {
        return this->_beforeAll;
    }

    optional<configure_fn> describeoptions_t::getBeforeEach() const {
        return this->_beforeEach;
    }

    // TODO: Use these to make the unimplemented_* errors simpler to call.
    // For this function
    // testbundle_t describe(const std::string& label, const make_testbundle_fn& make_tests, std::optional<describeoptions_t> options)
    // __PRETTY_FUNCTION__
    // Unimplemented function: JTest::testbundle_t JTest::describe(const std::string &, const JTest::make_testbundle_fn &, std::optional<describeoptions_t>)
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
