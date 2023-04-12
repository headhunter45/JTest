#ifndef JTEST__EXPECTABLE_H__
#define JTEST__EXPECTABLE_H__
namespace JTest {
    template<typename T>
    class Expectable;
}
#include <functional>
#include <string>
#include <sstream>
#include <optional>
#include <tuple>
#include <JTest/FailedExpectation.h>
#include <type_traits>

namespace JTest {
    template<typename T>
    class Expectable {
        public:
            Expectable(const T& actual)
            : actual_(actual)
            , is_inverted_(false) {}

            Expectable(const T& actual, bool is_inverted)
            : actual_(actual)
            , is_inverted_(is_inverted) {}

            virtual ~Expectable() {}

            const T& actual() {
                return actual_;
            }

            bool isInverted() {
                return is_inverted_;
            }

            virtual Expectable<T> nevermore() {
                return Expectable(actual_, !is_inverted_);
            }

            void toBe(std::function<std::optional<std::string>(const T&)> matcher) {
                auto reason = matcher(actual_);
                if (is_false(reason.has_value())) {
                    throw FailedExpectation(make_failure_message(*reason));
                }
            }

            template<typename T2 = T>
            typename std::enable_if<std::is_convertible<T2, bool>::value, void>::type
            toBeFalse() {
                if (is_true(actual_)) {
                    throw FailedExpectation(make_failure_message("to be false"));
                }
            }

            template<typename T2 = T>
            typename std::enable_if<std::is_pointer<T2>::value, void>::type
            toBeNull() {
                if (is_false(actual_ == nullptr)) {
                    throw FailedExpectation(make_failure_message("to be null"));
                }
            }

            template<typename T2 = T>
            typename std::enable_if<std::is_convertible<T2, bool>::value, bool>::type
            toBeTrue() {
                if (is_false(actual_)) {
                    throw FailedExpectation(make_failure_message("to be true"));
                }
            }

            template<typename T2 = T>
            typename std::enable_if<decltype(std::declval<std::equal_to <>>()(std::declval<T2>(), std::declval<T2>()), void(), std::true_type())::type::value, void>::type
            toEqual(const T& value) {
                if (is_false(actual_ == value)) {
                    throw FailedExpectation(make_failure_message("to equal", value));
                }
            }

            template<typename T2 = T>
            typename std::enable_if<std::is_invocable<T2>::value, bool>::type
            toThrow() {
                bool has_failed;
                std::string reason;
                try {
                    // TODO: Do some magic here to allow for args and member functions.
                    std::invoke(actual_);
                    has_failed = is_false();
                    if (has_failed) {
                        reason = "to throw, but did not";
                    }
                } catch(...) {
                    has_failed = is_true();
                    if (has_failed) {
                        reason = "to throw, but threw something";
                    }
                }
                if (has_failed) {
                    throw FailedExpectation(make_failure_message(reason));
                }
            }

            template<typename T2=T>
            typename std::enable_if<std::is_invocable<T2>::value, bool>::type
            toThrow(const std::exception& expected) {
                bool has_failed;
                std::string reason = "";
                try {
                    // TODO: Do some magic here to allow for args and member functions.
                    std::invoke(actual_);
                    has_failed = is_false();
                    if (has_failed) {
                        reason = ", but did not throw";
                    }
                } catch(const std::exception& actual) {
                    has_failed = is_true(strcmp(expected.what(), actual.what()) == 0);
                    if (has_failed) {
                        throw FailedExpectation(make_failure_message("to throw", expected.what(), actual.what()));
                        reason = string(is_inverted_ ? "" : ", but threw ") + actual.what();
                    }
                } catch(...) {
                    has_failed = is_false();
                    reason = "but threw something else";
                    reason = is_inverted_ ? "" : ", but threw something else";
                }
                if (has_failed) {
                    throw FailedExpectation(make_failure_message("to throw " + (expected.what() + reason)));
                }
            }

            template<typename T2=T>
            typename std::enable_if<std::is_invocable<T2>::value, bool>::type
            toThrow(std::function<std::optional<std::string>(const std::exception&)> matcher) {
                bool has_failed;
                std::optional<std::string> reason = std::nullopt;
                try {
                    // TODO: Do some magic here to allow for args and member functions.
                    std::invoke(actual_);
                    has_failed = is_false();
                    if (has_failed) {
                        reason = "did not throw";
                    }
                } catch(const std::exception& actual) {
                    reason = matcher(actual);
                    has_failed = is_true(reason.has_value());
                } catch(...) {
                    has_failed = is_false();
                    if (has_failed) {
                        reason = "threw something else";
                    }
                }
                if (has_failed) {
                    if (reason.has_value()) {
                        throw FailedExpectation(make_failure_message(is_inverted_ ? "to throw, and " : "to throw, but " + *reason));
                    }
                    // This should never be hit.
                    throw FailedExpectation(make_failure_message("to throw"));
                }
            }
            
            template<typename T2=T>
            typename std::enable_if<std::is_invocable<T2>::value, bool>::type
            toThrow(const std::string& what) {
                bool has_failed;
                std::string reason;
                try {
                    // TODO: Do some magic here to allow for args and member functions.
                    std::invoke(actual_);
                    has_failed = is_false();
                    if (has_failed) {
                        reason = "to throw " + what + ", but did not throw anything";
                    }
                } catch(const std::exception& actual) {
                    has_failed = is_true(what == actual.what());
                    if (has_failed) {
                        reason = "to throw " + what + ", but actually threw " + actual.what();
                    }
                } catch(...) {
                    has_failed = is_false();
                    if (has_failed) {
                        reason = "to throw " + what + ", but threw something that is not an exception";
                    }
                }
                if (has_failed) {
                    throw FailedExpectation(make_failure_message(reason));
                }
            }

        private:
            const T& actual_;
            const bool is_inverted_;
            bool is_true(bool condition = true) {
                return condition != is_inverted_;
            }
            bool is_false(bool condition = true) {
                return condition == is_inverted_;
            }
 
            // std::string make_failure_message(const T& expected, const T& actual) {
            //     ostringstream os;
            //     os << "expected: " << value << ", actual: " << actual_;
            //     return os.str();
            // }
            // Failed Expectation: 
            // to be equal with expected = 12, and actual = 0
            // Failed Expectation: 
            // not to be equal with expected = 12, and actual = 12

            std::string make_failure_message(const std::string reason) {
                std::ostringstream os;
                os << (is_inverted_ ? "not " : "") << reason << " with actual = " << actual_;
                return os.str();
            }

            std::string make_failure_message(const std::string reason, const T& expected) {
                std::ostringstream os;
                os << (is_inverted_ ? "not " : "") << reason << " with expected = " << expected << ", and actual = " << actual_;
                return os.str();
            }

            std::string make_failure_message(const std::string reason, std::string expected, std::string actual) {
                std::ostringstream os;
                os << (is_inverted_ ? "not " : "") << reason
                   << " with expected message = \"" << expected
                   << "\", and actual message = \"" << actual_ << "\"";
                return os.str();
            }

            // TODO: Fix the make_failure_message methods so they only exist if operator<< is defined for (ostream&, T)
            // Maybe have it generate a shorter message.
            // Maybe also check for string + T and use that if operator<< does not exist.
            // If neither of those exist this should at least say "expected not equal to actual"
            // Also this should take is_inverted_ into account.
    };

    // Other possible names since Expectable<T> not() is not an option
    // Wanted, but can't have: expect(2+2).not().toBe(5);
    // expect(2+2).absolutelyNot().toBe(5);
    // expect(2+2).definitelyNot().toBe(5);
    // expect(2+2).isNot().toBe(5);
    // expect(2+2).isNotGoing().toBe(5);
    // expect(2+2).nay().toBe(5);
    // expect(2+2).neer().toBe(5);
    // expect(2+2).neither().toBe(5);
    // expect(2+2).never().toBe(5);
    // expect(2+2).nevermore().toBe(5);
    // expect(2+2).nix().toBe(5);
    // expect(2+2).no().toBe(5);
    // expect(2+2).notAtAll().toBe(5);
    // expect(2+2).notByAnyMeans().toBe(5);
    // expect(2+2).surelyNot().toBe(5);

    // This is for T actual(){...} since Expectable<void()> has no default constructor.
    // Expectable<void()> won't compile no default constructor exists for class "Expectable<void ()>"
    // template<typename T2 = T>
    // typename std::enable_if<std::is_default_constructible<T2>::value, T>::type
}

#endif
