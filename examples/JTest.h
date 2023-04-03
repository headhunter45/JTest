#include <iostream> // TODO: Maybe just ostream.

namespace JTest {
    using std::wostream;
    // TODO: Consider making testresults_t a class so we can hide the vectors behind accessor methods void add(...), T get(), vector<T> get(uint32_t index)

    struct testresults_t {
        uint32_t total;
        uint32_t skipped;
        uint32_t passed;
        uint32_t failed;
        // vector<error_t> errors;
        // vector<testfailure_t> failures;
        // vector<testmethod_t> skipped;
    };

    testresults_t operator+(const testresults_t& left, const testresults_t& right);

    testresults_t& operator+=(testresults_t& left, const testresults_t& right);
    
    testresults_t make_testresults();
    testresults_t make_testresults(uint32_t total, uint32_t skipped, uint32_t passed, uint32_t failed);
    testresults_t add(const testresults_t&, const testresults_t&);
    void print_test_results(const testresults_t&, wostream&);

    struct testbundle_t {};

    // Executes the tests in tests. Possibly in parallel. Will block until all async tests have completed.
    testresults_t execute(testbundle_t tests);


    typedef std::function<testbundle_t()> make_testbundle_fn;

    typedef std::function<void()> configure_fn;
    struct describeoptions_t {
        // TODO: Should these configure_fn params be const and/or &?
        describeoptions_t& beforeEach(configure_fn);
        describeoptions_t& afterEach(configure_fn);
        describeoptions_t& beforeAll(configure_fn);
        describeoptions_t& afterAll(configure_fn);
        private:
            configure_fn _beforeEach;
            configure_fn _afterEach;
            configure_fn _beforeAll;
            configure_fn _afterAll;
    };

    // 
    testbundle_t describe(const std::wstring& label, const make_testbundle_fn& make_tests, std::optional<describeoptions_t> options = std::nullopt);

    struct testoptions_t {};
    
    
    struct test_t {};

    testbundle_t make_testbundle(const std::vector<testbundle_t>& tests, const describeoptions_t& options);
    // testbundle_t make_testbundle( initializer_list tests, const testoptions_t& options);

    typedef std::function<test_t()> make_test_fn;

    // TODO: Make this return a test_t instead.
    // TOOD: Bake make_test_fn not need to return testresults_t. Method calls should be surrounded with try/catch.
    //   The label should be extracted from the test_t it returns.
    //   The testresults_t should be constructed based on the try/catch block and whether this was called as it/xit.
    testbundle_t it(const std::wstring& label, const make_test_fn& test_method, std::optional<testoptions_t> options = std::nullopt);

    describeoptions_t make_describeoptions();






}
