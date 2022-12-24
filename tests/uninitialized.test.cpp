#include <catch2/catch_all.hpp>
#include "ez/cli/details_/uninitialized.h"

TEST_CASE("Uninitialized")
{
    struct Some {
        Some(int k) : i{k} {}
        int i;
    };

    ez::cli::details_::Uninitialized<Some> v;
    auto dtr = v.construct(3);
    REQUIRE(v.get().i == 3);
    auto k = std::move(dtr);
    auto don = ez::cli::details_::Detached_destructor{};
    don = std::move(k);
}

