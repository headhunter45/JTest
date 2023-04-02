#include "JTest.h"

namespace JTest {
    using std::wostream;
    using std::endl;

    // typedef struct {
    //     uint32_t total;
    //     uint32_t skipped;
    //     uint32_t passed;
    //     uint32_t failed;
    //     // vector<error_t> errors;
    //     // vector<testfailure_t> failures;
    //     // vector<testmethod_t> skipped;
    // } testresults_t;

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
}
