#include "JTest.h"
#include <stdexcept>

namespace JTest {
    using std::wostream;
    using std::endl;
    using std::vector;
    using std::wstring;
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

    void print_test_results(const testresults_t& results, wostream& out) {
        out << L"Tests: " << results.total << endl;
        out << L"Failed: " << results.failed << ", Passed: " << results.passed << ", Skipped: " << results.skipped << endl; 
    }

    testresults_t operator+(const testresults_t& left, const testresults_t& right) {
        return add(left, right);
    }

    testresults_t& operator+=(testresults_t& left, const testresults_t& right) {
        left = left + right;
        return left;
    }

    testbundle_t make_testbundle(const vector<testbundle_t>&, const describeoptions_t& options) {
        throw unimplemented_function_error("make_testbundle(const vector<testbundle_t>&, const describeoptions_t&)");
    }

    describeoptions_t& describeoptions_t::beforeEach(configure_fn) {
        throw unimplemented_method_error("describeoptions_t", "beforeEach(configure_fn)");
    }

    describeoptions_t& describeoptions_t::afterEach(configure_fn) {
        throw unimplemented_method_error("describeoptions_t", "afterEach(configure_fn)");
    }
    describeoptions_t& describeoptions_t::beforeAll(configure_fn) {
        throw unimplemented_method_error("describeoptions_t", "beforeAll(configure_fn)");
    }
    describeoptions_t& describeoptions_t::afterAll(configure_fn) {
        throw unimplemented_method_error("describeoptions_t", "afterAll(configure_fn)");
    }
    describeoptions_t make_describeoptions() {
        return {};
    }

    testbundle_t it(const wstring& label, const make_test_fn& test_method, optional<testoptions_t> options) {
        throw unimplemented_function_error("it(const wstring&, const make_test_fn&, optional<testoptions_t>)");
    }

    testresults_t execute(testbundle_t tests) {
        throw unimplemented_function_error("execute(testbundle_t)");
    }

    testbundle_t describe(const wstring& label, const make_testbundle_fn& make_tests, optional<describeoptions_t> options) {
        throw unimplemented_function_error("describe(const wstring&, const make_testbundle_fn&, optional<describeoptions_t>)");
    }

    // TODO: Use these to make the unimplemented_* errors simpler to call.
    // For this function
    // testbundle_t describe(const std::wstring& label, const make_testbundle_fn& make_tests, std::optional<describeoptions_t> options)
    // __PRETTY_FUNCTION__
    // Unimplemented function: JTest::testbundle_t JTest::describe(const std::wstring &, const JTest::make_testbundle_fn &, std::optional<describeoptions_t>)
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
