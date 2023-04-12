## TODO:
*   Make Expectable toThrow methods work with arguments, class methods, static class methods, and possibly constructors/destructors.
*   Report all errors at the end instead of printing them inline. This is needed for parallel support.
*   Figure out how to make JTest self testing. Maybe use some namespace magic to run a special instance in another namespace.
*   Mocks
    *   Design them.
    *   Add them.
*   Spies
    *   Design them.
    *   Add them.
*   Expectable<T>
    *   Clean up failure messages in Expectable.
    *   Make failure messages in Expectable only use ostreams if operator<< is defined for T.
    *   Pick a name for Expectable<T>::not()
*   DescribeOptions
    *   Add operator<< and print for DescribeOptions
*   JTest
    *   Make it and xit return a Test instead of a TestBundle
    *   Make describe and xdescribe take a variant of Test or TestBundle instead of just TestBundle
    *   Make TestOptions useful
    *   Find a way to set label/path from it/xit/describe/xdescribe instead of when we execute them.
    *   If executing a disabled bundle report all of it's child tests as skipped/disabled.
    *   Execute tests in parallel.
    *   Recheck that bundles/tests inherit before/after methods from their parents.
        *   Before* of parents should be called before that of children.
        *   After* of parents should be called after that of children.
    *   When reporting Unhandled exceptions in tests, report the fully qualified test name and label.
*   TestResults
    *   Add commented out vectors to hold skipped, failed, errored test info.
*   FailedExpectation
    *   Figure out a way to get label and path when creating the expectation from an Expect method.
        *   We need to get the label and path from the parent method (execute) that is calling expect<T>(T) to create the Expectable.
        *   We will likely be creating the FailedExpectation exception where the test fails and adding the label and path when we catch it before reporting the failure.
