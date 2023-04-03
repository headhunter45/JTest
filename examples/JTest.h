#include <iostream> // TODO: Maybe just ostream.

namespace JTest {
    using std::wostream;
    // TODO: Consider making testresults_t a class so we can hide the vectors behind accessor methods void add(...), T get(), vector<T> get(uint32_t index)

    struct testbundle_t;
    struct test_t;
    typedef std::function<test_t()> make_test_fn;
    typedef std::function<testbundle_t()> make_testbundle_fn;
    typedef std::function<void()> configure_fn;

    struct testresults_t {
        uint32_t total;
        uint32_t skipped;
        uint32_t passed;
        uint32_t failed;
        // vector<error_t> errors;
        // vector<testfailure_t> failures;
        // vector<testmethod_t> skipped;
    };

    struct test_t {};
    struct testbundle_t {
        std::wstring _label;
        std::vector<test_t> _tests;
        std::vector<testbundle_t> _children;
        std::optional<configure_fn> _beforeEach;
        std::optional<configure_fn> _afterEach;
        std::optional<configure_fn> _beforeAll;
        std::optional<configure_fn> _afterAll; 
    };

    struct describeoptions_t {
        // TODO: Should these configure_fn params be const and/or &?
        describeoptions_t& beforeEach(configure_fn);
        describeoptions_t& afterEach(configure_fn);
        describeoptions_t& beforeAll(configure_fn);
        describeoptions_t& afterAll(configure_fn);
        std::optional<configure_fn> getBeforeEach() const;
        std::optional<configure_fn> getAfterEach() const;
        std::optional<configure_fn> getBeforeAll() const;
        std::optional<configure_fn> getAfterAll() const;
        private:
            std::optional<configure_fn> _beforeEach;
            std::optional<configure_fn> _afterEach;
            std::optional<configure_fn> _beforeAll;
            std::optional<configure_fn> _afterAll;
    };
    struct testoptions_t {};

    testresults_t operator+(const testresults_t& left, const testresults_t& right);

    testresults_t& operator+=(testresults_t& left, const testresults_t& right);
    
    testresults_t make_testresults();
    testresults_t make_testresults(uint32_t total, uint32_t skipped, uint32_t passed, uint32_t failed);
    testresults_t add(const testresults_t&, const testresults_t&);
    void print_test_results(const testresults_t&, wostream&);

    // Executes the tests in tests. Possibly in parallel. Will block until all async tests have completed.
    testresults_t execute(testbundle_t tests);

    // 
    testbundle_t describe(const std::wstring& label, const make_testbundle_fn& make_tests, std::optional<describeoptions_t> options = std::nullopt);

    testbundle_t make_testbundle(const std::vector<testbundle_t>& tests, const describeoptions_t& options);
    // testbundle_t make_testbundle( initializer_list tests, const testoptions_t& options);

    // TODO: Make this return a test_t instead.
    // TOOD: Bake make_test_fn not need to return testresults_t. Method calls should be surrounded with try/catch.
    //   The label should be extracted from the test_t it returns.
    //   The testresults_t should be constructed based on the try/catch block and whether this was called as it/xit.
    testbundle_t it(const std::wstring& label, const make_test_fn& test_method, std::optional<testoptions_t> options = std::nullopt);

    describeoptions_t make_describeoptions();

    testbundle_t make_testbundle(const std::wstring& label, const std::vector<test_t>& tests);

}
