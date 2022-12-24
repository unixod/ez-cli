#include <catch2/catch_all.hpp>
#include "ez/c_string_view.h"

TEST_CASE("C_string_view")
{
    using namespace std::string_view_literals;
    constexpr ez::utils::C_string_view k{"asfd"};
    std::string str = "asfd";
    STATIC_REQUIRE(std::contiguous_iterator<ez::utils::C_string_view::Iterator>);
    STATIC_REQUIRE(std::ranges::forward_range<ez::utils::C_string_view>);
    REQUIRE(std::ranges::equal(k, str));
    constexpr ez::utils::C_string_view zk{"asfd"};
    constexpr ez::utils::C_string_view zk2{"asf"};
    static_assert(zk > zk2);
    REQUIRE(k < ez::utils::C_string_view{"zz"});
    REQUIRE(k <= ez::utils::C_string_view{"zz"});
    REQUIRE(k >= ez::utils::C_string_view{"as"});
    REQUIRE(k == ez::utils::C_string_view{"asfd"});
    REQUIRE(!std::ranges::empty(k));
    REQUIRE(!k.empty());
    REQUIRE(std::ranges::empty(ez::utils::C_string_view{""}));
    REQUIRE(std::ranges::distance(ez::utils::C_string_view{"asdf"}) == 4);
    REQUIRE(std::ranges::distance(ez::utils::C_string_view{""}) == 0);
    STATIC_REQUIRE(std::ranges::borrowed_range<ez::utils::C_string_view>);
    STATIC_REQUIRE(std::ranges::range<ez::utils::C_string_view>);
    STATIC_REQUIRE(std::ranges::range<const ez::utils::C_string_view>);
}

