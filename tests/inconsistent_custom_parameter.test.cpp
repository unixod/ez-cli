#include <catch2/catch_all.hpp>
#include "ez/cli/parameter/concepts.h"

using ez::cli::api::Positional_parameter;
using ez::cli::api::Regular_parameter;
using ez::cli::api::Bool_parameter;
using ez::cli::api::Parameter;


struct Inconsistent_positional_parameter_1 {
    static constexpr auto name = "...";
    static constexpr auto description = "...";

    static auto parse_value(auto)
    {
        return int{3};
    }

    static auto default_value()
    {
        return double{1};
    }
};

TEST_CASE("Mismatch of return value types of parse_value and default_value")
{
    using P = Inconsistent_positional_parameter_1;

    STATIC_REQUIRE_FALSE(Positional_parameter<P>);
    STATIC_REQUIRE_FALSE(Regular_parameter<P>);
    STATIC_REQUIRE_FALSE(Bool_parameter<P>);
    STATIC_REQUIRE_FALSE(Parameter<P>);

    namespace details_ = ez::cli::api::details_;
    STATIC_REQUIRE_FALSE(details_::Has_short_name<P>);
    STATIC_REQUIRE_FALSE(details_::Has_long_name<P>);
    STATIC_REQUIRE(details_::Has_unit_name<P>);
    STATIC_REQUIRE(details_::Has_description<P>);
    STATIC_REQUIRE(details_::Has_value_parser<P>);
    STATIC_REQUIRE(details_::Has_default_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_true_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_false_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_repeated_value_parser<P>);
}




