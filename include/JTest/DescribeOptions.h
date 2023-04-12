#ifndef JTEST__DESCRIBEOPTIONS_H__
#define JTEST__DESCRIBEOPTIONS_H__
namespace JTest {
    class DescribeOptions;
}
#include <optional>
#include <JTest/ConfigureFunction.h>

//TODO: Add operator<< and maybe print(ostream&)
namespace JTest {
    class DescribeOptions {
        public:
            DescribeOptions();
            maybe_configure_fn getAfterAll() const;
            maybe_configure_fn getAfterEach() const;
            maybe_configure_fn getBeforeAll() const;
            maybe_configure_fn getBeforeEach() const;
            DescribeOptions& afterAll(maybe_configure_fn fn);
            DescribeOptions& afterEach(maybe_configure_fn fn);
            DescribeOptions& beforeAll(maybe_configure_fn fn);
            DescribeOptions& beforeEach(maybe_configure_fn fn);
        private:
            maybe_configure_fn afterAll_;
            maybe_configure_fn afterEach_;
            maybe_configure_fn beforeAll_;
            maybe_configure_fn beforeEach_;
    };
}
#endif
