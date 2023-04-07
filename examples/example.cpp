// #include <JTest.h>
#include "JTest.h"
#include "ClassToTest.h"
#include <string>
#include <vector>
#include <tuple>
#include <stdexcept>
#include <iostream>
#include <sstream>

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

template<typename T>
class Expectable {
    public:
        Expectable(const T& actual);
        virtual ~Expectable();
        virtual void toEqual(const T& value);
        virtual void toNotEqual(const T& value);
        // Maybe these funcs should return tuple<bool, string> instead.
        virtual void toBe(std::function<std::tuple<bool, std::optional<std::string>>(const T& actual)> evaluator);
        //virtual void toBeNull();
        //virtual void toNotBeNull();
        //virtual void toThrow(...);
    private:
        const T& actual_;
};

template<typename T>
Expectable<T>::Expectable(const T& actual)
: actual_(actual) {}

template<typename T>
Expectable<T>::~Expectable() {}

class FailedExpectation: std::runtime_error {
    public:
        FailedExpectation(const std::string& message);
        virtual ~FailedExpectation();
};

FailedExpectation::FailedExpectation(const std::string& message)
: runtime_error(message) {
}

FailedExpectation::~FailedExpectation() {}

template<typename T>
void Expectable<T>::toEqual(const T& value) {
    if (actual_ != value) {
        std::ostringstream os;
        os << "Expected: " << actual_ << " to be " << value;
        throw(FailedExpectation(os.str()));
    }
}

template<typename T>
void Expectable<T>::toNotEqual(const T& value) {
    if (actual_ == value) {
        std::ostringstream os;
        os << "Expected: " << actual_ << " to not be " << value;
        throw(FailedExpectation(os.str()));
    }
}

template<typename T>
void Expectable<T>::toBe(std::function<std::tuple<bool, std::optional<std::string>>(const T& actual)> evaluator) {
    auto result = evaluator(actual_);
    if (!std::get<0>(result)) {
        std::ostringstream os;
        // std::optional<std::string> message = std::get<1>;
        std::optional<std::string> message = std::get<1>(result);
        if (message.has_value()) {
            os << "Expected: " << actual_ << " to pass validation, but " << message.value();
        } else {
            os << "Expected: " << actual_ << " to pass validation.";
        }
        throw(FailedExpectation(os.str()));
    }
}

// template<typename T>
// void Expectable<T>::toBeNull() {
//     if (actual_ != nullptr) {
//         std::ostringstream os;
//         os << "Expected null, but got " << actual_;
//         throw(FailedExpectation(os.str()));
//     }
// }

// template<typename T>
// void Expectable<T>::toNotBeNull() {
//     if (actual_ == nullptr) {
//         throw(FailedExpectation("Expected non-null, but got null"));
//     }
// }

// template<typename T>
// void Expectable<T>::toThrow(...) {}

template<typename T>
Expectable<T> expect(const T& actual);

template<typename T>
Expectable<T> expect(const T& actual) {
    return Expectable<T>(actual);
}
        
int MyAddFunction(int a, int b) {
    return 0;
}

testresults_t test_BasicExpectable_main(const vector<string>& argv) {
    return execute(
        describe("MyAddFunction", [](){
            return make_testbundle({
                it("should add 2 and 2 to get 4", [](){
                    expect(MyAddFunction(2, 2)).toEqual(4);
                }),
                it("should ", [](){
                    // Throw exception if somethings goes wrong
                }),
            }, make_describeoptions());
        }));
}

// Dummy test harness
int main(int argc, char* argv[]) {
    try {
        vector<string> args;
        testresults_t results;

        results = add(results, test_ClassToTest_main(args));
        results = add(results, test_BasicExpectable_main(args));

        print_test_results(results, cout);
    }
    catch (std::runtime_error ex) {
        std::cout << "Unhandled exception: " << ex.what() << endl;
    }

    return 0;
}
