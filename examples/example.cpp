// #include <JTest.h>
#include "JTest.h"
#include "ClassToTest.h"
#include <string>
#include <vector>
#include <tuple>


using namespace JTest;
using namespace MyNS;

using std::wstring;
using std::vector;
using std::wcout;

// const vector<wstring>& might stop being a reference
testresults_t test_ClassToTest_main(int argc, const vector<wstring>& argv) {
    auto results = describe(L"ClassToTest", [](){
        auto results = make_testresults();

        results += it(L"should do the thing", [](){

        });

        results += it(L"should do the other thing", [](){

        });

        results += it(L"should not do the bad thing", [](){

        });

        results += it(L"should throw an exception if we do the other bad thing", [](){

        });

        return results;
    }, make_describe_options().beforeEach([](){}).afterEach([](){}).beforeAll([](){}).afterAll([](){}));
    return results;
}

// Dummy test harness
int main(int argc, char* argv[]) {
    testresults_t results = make_testresults();

    results = add(results, test_ClassToTest_main(0, vector<wstring>()));

    print_test_results(results, wcout);

    return 0;
}
