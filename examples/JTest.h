#include <iostream> // TODO: Maybe just ostream.

namespace JTest {
    using std::wostream;
    // TODO: Consider making testresults_t a class so we can hide the vectors behind accessor methods void add(...), T get(), vector<T> get(uint32_t index)

    typedef struct {
        uint32_t total;
        uint32_t skipped;
        uint32_t passed;
        uint32_t failed;
        // vector<error_t> errors;
        // vector<testfailure_t> failures;
        // vector<testmethod_t> skipped;
    } testresults_t;

    testresults_t make_testresults();
    testresults_t make_testresults(uint32_t total, uint32_t skipped, uint32_t passed, uint32_t failed);
    testresults_t add(const testresults_t&, const testresults_t&);
    void print_test_results(const testresults_t&, wostream&);
}
