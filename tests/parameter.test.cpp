#include <catch2/catch_all.hpp>
#include "ez/cli/parameter.h"
#include "ez/cli/parameter_traits.h"

TEST_CASE("")
{
    enum struct Int {};

    using A = ez::cli::Parameter<Int, "-a", "",
        "Foo is optional flag",
        [/*default value*/]() {
            return Int{9};
        },
        [](std::true_type) {
            return Int{123};
        }
    >;

    static_assert(std::is_same_v<ez::cli::api::Parameter_value_t<A>, Int>);
    static_assert(sizeof(A) != 0);
}
