#include <iostream> // TODO: Maybe just ostream.

namespace JTest {
    using std::ostream;
    // TODO: Consider making testresults_t a class so we can hide the vectors behind accessor methods void add(...), T get(), vector<T> get(uint32_t index)

    struct testbundle_t;
    struct test_t;
    typedef std::function<void()> test_fn;
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

        std::string _label;
        test_fn _test_method;
        bool _disabled;
    };

    struct testbundle_t {
        std::string _label;
        std::vector<test_t> _tests;
        std::vector<testbundle_t> _children;
        std::optional<configure_fn> _beforeEach;
        std::optional<configure_fn> _afterEach;
        std::optional<configure_fn> _beforeAll;
        std::optional<configure_fn> _afterAll;
        bool _disabled;
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
    void print_test_results(const testresults_t&, ostream&);

    // Executes the tests in tests. Possibly in parallel. Will block until all async tests have completed.
    testresults_t execute(testbundle_t tests);
    testresults_t execute(test_t test);

    // 
    testbundle_t describe(const std::string& label, const make_testbundle_fn& make_tests, std::optional<describeoptions_t> options = std::nullopt);
    testbundle_t xdescribe(const std::string& label, const make_testbundle_fn& make_tests, std::optional<describeoptions_t> options = std::nullopt);

    testbundle_t make_testbundle(const std::vector<testbundle_t>& tests, const describeoptions_t& options);
    
    // TODO: Make this return a test_t instead.
    testbundle_t it(const std::string& label, const test_fn& test_method, std::optional<testoptions_t> options = std::nullopt);
    testbundle_t xit(const std::string& label, const test_fn& test_method, std::optional<testoptions_t> options = std::nullopt);

    describeoptions_t make_describeoptions();

    testbundle_t make_testbundle(const std::string& label, const std::vector<test_t>& tests);

}
