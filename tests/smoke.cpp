// Smoke test for the JTest build system.
//
// This is a *genuine* self-check (not a demo). It exercises the public API the
// same way a user program would (describe/it/expect/toEqual/execute) and drives
// `ctest` so a broken or misconfigured build goes red. Unlike
// examples/example.cpp (which intentionally contains failing/skipping cases to
// showcase JTest's reporting and always returns 0), this test returns a
// non-zero exit code when an assertion fails, so CTest produces a real
// pass/fail.
//
// It links JTest and compiles without any external dependency, proving that the
// library archives, the include directory is exported transitively, and C++17
// is in effect.
#include <JTest/JTest.h>

#include <string>

namespace {
using JTest::describe;
using JTest::execute;
using JTest::expect;
using JTest::it;
using JTest::TestBundle;
using JTest::TestResults;
using std::string;

int add(int a, int b) {
    return a + b;
}
}   // namespace

int main() {
    TestResults results = execute(
        describe("JTest smoke", []() {
            return TestBundle(
                {
                    it("executes a passing arithmetic assertion", []() {
                        expect(add(2, 2)).toEqual(4);
                     }),
                    it("executes a passing string assertion", []() {
                        expect(string("JTest")).toEqual(string("JTest"));
                     }),
                    it("executes a second passing arithmetic assertion", []() {
                        expect(add(10, 20)).toEqual(30);
                     }),
                 },
                JTest::DescribeOptions());
         }));

    // Non-zero exit when any test failed or errored, so CTest surfaces a real
    // failure instead of silently passing.
    return results.failed == 0 ? 0 : 1;
}
