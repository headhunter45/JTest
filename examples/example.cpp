// #include <JTest.h>
#include "JTest.h"
#include "ClassToTest.h"
#include <string>
#include <vector>
#include <tuple>
#include <stdexcept>

using namespace JTest;
using namespace MyNS;

using std::string;
using std::vector;
using std::cout;
using std::exception;
using std::endl;

// const vector<string>& might stop being a reference
testresults_t test_ClassToTest_main(const vector<string>& argv) {
    return execute(
        describe("ClassToTest", [](){
            return make_testbundle(
                {
                    it("should do the thing", [](){
                        // Throw exception if somethings goes wrong


                        // TODO: This unnecessary, by throwing exceptions.
                        return (test_t){};
                    }),

                    it("should do the other thing", [](){
                    }),

                    it("should not do the bad thing", [](){
                    }),

                    it("should throw an exception if we do the other bad thing", [](){
                    }),
                },
                make_describeoptions()
                    .beforeEach([](){})
                    .afterEach([](){})
                    .beforeAll([](){})
                    .afterAll([](){}));
        })
    );
}

testresults_t test_temp(const vector<string>& argv) {
    return execute(
        describe("ClassToTest", [](){
            return make_testbundle({
                describe("FeatureToTest", [](){
                    return make_testbundle({
                        // it("should do the thing", [](){

                        // }),
                        // it("should not do the other thing", [](){

                        // }),
                    }, make_describeoptions());
                }),
                }, make_describeoptions());
        })
    );
}

// Exmple of nested describes.
testresults_t test_something(const vector<string>& argv) {
    return execute(
        describe("", [](){
            return make_testbundle({
                describe("", [](){
                    return make_testbundle({

                    }, make_describeoptions());
                }, make_describeoptions()),
            }, make_describeoptions());
        })
    );
}

testresults_t test_ClassToTest_2(const vector<string>& argv) {
    return execute(
        describe("ClassToTest", [](){
            return make_testbundle({
                },
                make_describeoptions()
                    .beforeEach([](){})
                    .afterEach([](){})
                    .beforeAll([](){})
                    .afterAll([](){}));
        })
    );
}

// Dummy test harness
int main(int argc, char* argv[]) {
    try {
        vector<string> args;
        testresults_t results;

        results = add(results, test_ClassToTest_main(args));

        print_test_results(results, cout);
    }
    catch (std::runtime_error ex) {
        std::cout << "Unhandled exception: " << ex.what() << endl;
    }

    return 0;
}
