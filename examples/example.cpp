#include <JTest/JTest.h>
#include <string>
#include <vector>
#include <tuple>
#include <stdexcept>
#include <iostream>
#include "ClassToTest.h"

using namespace JTest;
using namespace MyNS;

using std::string;
using std::vector;
using std::cout;
using std::exception;
using std::endl;
using std::runtime_error;

// const vector<string>& might stop being a reference
TestResults test_ClassToTest_main(const vector<string>& argv) {
    return execute(
        describe("ClassToTest", [](){
            return TestBundle(
                {
                    it("should do the thing", [](){
                        // Throw exception if somethings goes wrong
                    }),

                    it("should do the other thing", [](){
                        pending("we havent made the other thing yet");
                    }),

                    it("should not do the bad thing", [](){
                        fail("it did the bad thing");
                    }),

                    it("should throw an exception if we do the other bad thing", [](){
                        throw runtime_error("Bad thing happen.");
                    }),
                },
                DescribeOptions()
                    .beforeEach([](){})
                    .afterEach([](){})
                    .beforeAll([](){})
                    .afterAll([](){}));
        })
    );
}

TestResults test_temp(const vector<string>& args) {
    return execute(
        describe("ClassToTest", [](){
            return TestBundle({
                describe("FeatureToTest", [](){
                    return TestBundle({
                        // it("should do the thing", [](){

                        // }),
                        // it("should not do the other thing", [](){

                        // }),
                    }, DescribeOptions());
                }),
                }, DescribeOptions());
        })
    );
}

// Exmple of nested describes.
TestResults test_something(const vector<string>& argv) {
    return execute(
        describe("", [](){
            return TestBundle({
                describe("", [](){
                    return TestBundle({

                    }, DescribeOptions());
                }, DescribeOptions()),
            }, DescribeOptions());
        })
    );
}

TestResults test_ClassToTest_2(const vector<string>& argv) {
    return execute(
        describe("ClassToTest", [](){
            return TestBundle({
                },
                DescribeOptions()
                    .beforeEach([](){})
                    .afterEach([](){})
                    .beforeAll([](){})
                    .afterAll([](){}));
        })
    );
}
        
int MyAddFunction(int a, int b) {
    return a+b;
}

TestResults test_BasicExpectable_main(const vector<string>& argv) {
    return execute(
        describe("MyAddFunction", [](){
            return TestBundle({
                it("should add 2 and 2 to get 4", [](){
                    expect(MyAddFunction(2, 2)).toEqual(4);
                }),
                it("should fail", [](){
                    fail("because I said so");
                    // Throw exception if somethings goes wrong
                }),
            }, DescribeOptions());
        }));
}

// Dummy test harness
int main(int argc, char* argv[]) {
    try {
        vector<string> args;
        TestResults results;

        results += test_ClassToTest_main(args);
        // results += test_BasicExpectable_main(args);
        // results += test_ClassToTest_2(args);
        // results += test_something(args);
        // results += test_temp(args);

        cout << results << endl;
    }
    catch (std::runtime_error ex) {
        std::cout << "Unhandled exception: " << ex.what() << endl;
    }

    return 0;
}
