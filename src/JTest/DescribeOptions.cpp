#include <JTest/DescribeOptions.h>
namespace JTest {
    DescribeOptions::DescribeOptions()
    : afterAll_(std::nullopt)
    , afterEach_(std::nullopt)
    , beforeAll_(std::nullopt)
    , beforeEach_(std::nullopt) {}

    maybe_configure_fn DescribeOptions::getAfterAll() const {
        return afterAll_;
    }
    
    DescribeOptions& DescribeOptions::afterAll(maybe_configure_fn fn) {
        this->afterAll_ = fn;
        return *this;
    }

    maybe_configure_fn DescribeOptions::getAfterEach() const {
        return afterEach_;
    }

    DescribeOptions& DescribeOptions::afterEach(maybe_configure_fn fn) {
        this->afterEach_ = fn;
        return *this;
    }
    
    maybe_configure_fn DescribeOptions::getBeforeAll() const {
        return beforeAll_;
    }

    DescribeOptions& DescribeOptions::beforeAll(maybe_configure_fn fn) {
        this->beforeAll_ = fn;
        return *this;
    }

    maybe_configure_fn DescribeOptions::getBeforeEach() const {
        return beforeEach_;
    }

    DescribeOptions& DescribeOptions::beforeEach(maybe_configure_fn fn) {
        this->beforeEach_ = fn;
        return *this;
    }    
}
