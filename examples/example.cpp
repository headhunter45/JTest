// #include <JTest.h>
#include "JTest.h"
#include "ClassToTest.h"
#include <string>
#include <vector>
#include <tuple>
#include <stdexcept>

using namespace JTest;
using namespace MyNS;

using std::wstring;
using std::vector;
using std::wcout;
using std::exception;
using std::endl;

// const vector<wstring>& might stop being a reference
testresults_t test_ClassToTest_main(int argc, const vector<wstring>& argv) {
    return execute(
        describe(L"ClassToTest", [](){
            return make_testbundle(
                {
                    it(L"should do the thing", [](){
                        // Throw exception if somethings goes wrong


                        // TODO: This unnecessary, by throwing exceptions.
                        return (test_t){};
                    }),

                    it(L"should do the other thing", [](){
                        return (test_t){};
                    }),

                    it(L"should not do the bad thing", [](){
                        return (test_t){};
                    }),

                    it(L"should throw an exception if we do the other bad thing", [](){
                        return (test_t){};
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

testresults_t test_temp(int argc, const vector<wstring>& argv) {
    return execute(
        describe(L"ClassToTest", [](){
            return make_testbundle({
                describe(L"FeatureToTest", [](){
                    return make_testbundle({
                        // it(L"should do the thing", [](){

                        // }),
                        // it(L"should not do the other thing", [](){

                        // }),
                    }, make_describeoptions());
                }),
                }, make_describeoptions());
        })
    );
}

// Exmple of nested describes.
testresults_t test_something(int argc, const vector<wstring>& argv) {
    return execute(
        describe(L"", [](){
            return make_testbundle({
                describe(L"", [](){
                    return make_testbundle({

                    }, make_describeoptions());
                }, make_describeoptions()),
            }, make_describeoptions());
        })
    );
}

testresults_t test_ClassToTest_2(int argc, const vector<wstring>& argv) {
    return execute(
        describe(L"ClassToTest", [](){
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
        testresults_t results = make_testresults();

        results = add(results, test_ClassToTest_main(0, vector<wstring>()));

        print_test_results(results, wcout);
    }
    catch (std::runtime_error ex) {
        std::cout << ex.what() << endl;
        // wcout << L"Unhandled exception: " << ex.what() << endl;
    }

    return 0;
}
