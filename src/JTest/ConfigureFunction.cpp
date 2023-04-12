#include <JTest/ConfigureFunction.h>
#include <optional>
namespace JTest {
    maybe_configure_fn combine(maybe_configure_fn first, maybe_configure_fn second) {
        if (first.has_value()) {
            auto first_v = first.value();
            if (second.has_value()) {
                auto second_v = second.value();
                return [first_v, second_v]() {
                    first_v();
                    second_v();
                };
            } else {
                return first;
            }
        } else {
            return second;
        }
    }
}
