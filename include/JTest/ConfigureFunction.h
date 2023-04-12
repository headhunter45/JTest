#ifndef JTEST__CONFIGUREFUNCTION_H__
#define JTEST__CONFIGUREFUNCTION_H__
#include <functional>
#include <optional>
namespace JTest {
    typedef std::function<void()> configure_fn;
    typedef std::optional<configure_fn> maybe_configure_fn;
    maybe_configure_fn combine(maybe_configure_fn first, maybe_configure_fn second);
}
#endif
