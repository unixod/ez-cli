#include <catch2/catch_all.hpp>
#include "ez/cli/parameter/concepts.h"

using ez::cli::api::Positional_parameter;
using ez::cli::api::Regular_parameter;
using ez::cli::api::Bool_parameter;
using ez::cli::api::Parameter;


struct Incomplete_custom_positional_parameter_1 {
    static constexpr auto name = "...";
};

TEST_CASE("parse_value and parameter description are missing")
{
    using P = Incomplete_custom_positional_parameter_1;

    STATIC_REQUIRE_FALSE(Positional_parameter<P>);
    STATIC_REQUIRE_FALSE(Regular_parameter<P>);
    STATIC_REQUIRE_FALSE(Bool_parameter<P>);
    STATIC_REQUIRE_FALSE(Parameter<P>);

    namespace details_ = ez::cli::api::details_;
    STATIC_REQUIRE_FALSE(details_::Has_short_name<P>);
    STATIC_REQUIRE_FALSE(details_::Has_long_name<P>);
    STATIC_REQUIRE(details_::Has_unit_name<P>);
    STATIC_REQUIRE_FALSE(details_::Has_description<P>);
    STATIC_REQUIRE_FALSE(details_::Has_value_parser<P>);
    STATIC_REQUIRE_FALSE(details_::Has_default_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_true_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_false_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_repeated_value_parser<P>);
}

struct Incomplete_custom_positional_parameter_2 {
    static constexpr auto name = "...";
    static constexpr auto description = "...";
};

TEST_CASE("parse_value is missing 1")
{
    using P = Incomplete_custom_positional_parameter_2;

    STATIC_REQUIRE_FALSE(Positional_parameter<P>);
    STATIC_REQUIRE_FALSE(Regular_parameter<P>);
    STATIC_REQUIRE_FALSE(Bool_parameter<P>);
    STATIC_REQUIRE_FALSE(Parameter<P>);

    namespace details_ = ez::cli::api::details_;
    STATIC_REQUIRE_FALSE(details_::Has_short_name<P>);
    STATIC_REQUIRE_FALSE(details_::Has_long_name<P>);
    STATIC_REQUIRE(details_::Has_unit_name<P>);
    STATIC_REQUIRE(details_::Has_description<P>);
    STATIC_REQUIRE_FALSE(details_::Has_value_parser<P>);
    STATIC_REQUIRE_FALSE(details_::Has_default_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_true_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_false_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_repeated_value_parser<P>);
}

// parse_value is absent.
struct Incomplete_custom_positional_parameter_3 {
    static constexpr auto name = "...";
    static constexpr auto description = "...";

    static auto default_value()
    {
        return 1;
    }
};

TEST_CASE("parse_value is missing 2")
{
    using P = Incomplete_custom_positional_parameter_3;

    STATIC_REQUIRE_FALSE(Positional_parameter<P>);
    STATIC_REQUIRE_FALSE(Regular_parameter<P>);
    STATIC_REQUIRE_FALSE(Bool_parameter<P>);
    STATIC_REQUIRE_FALSE(Parameter<P>);

    namespace details_ = ez::cli::api::details_;
    STATIC_REQUIRE_FALSE(details_::Has_short_name<P>);
    STATIC_REQUIRE_FALSE(details_::Has_long_name<P>);
    STATIC_REQUIRE(details_::Has_unit_name<P>);
    STATIC_REQUIRE(details_::Has_description<P>);
    STATIC_REQUIRE_FALSE(details_::Has_value_parser<P>);
    STATIC_REQUIRE(details_::Has_default_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_true_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_false_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_repeated_value_parser<P>);
}

struct Incomplete_custom_positional_parameter_4 {
    static constexpr auto description = "...";

    static auto parse_value(auto)
    {
        return 1;
    }
};


TEST_CASE("Parameter name is missing")
{
    using P = Incomplete_custom_positional_parameter_4;

    STATIC_REQUIRE_FALSE(Positional_parameter<P>);
    STATIC_REQUIRE_FALSE(Regular_parameter<P>);
    STATIC_REQUIRE_FALSE(Bool_parameter<P>);
    STATIC_REQUIRE_FALSE(Parameter<P>);

    namespace details_ = ez::cli::api::details_;
    STATIC_REQUIRE_FALSE(details_::Has_short_name<P>);
    STATIC_REQUIRE_FALSE(details_::Has_long_name<P>);
    STATIC_REQUIRE_FALSE(details_::Has_unit_name<P>);
    STATIC_REQUIRE(details_::Has_description<P>);
    STATIC_REQUIRE(details_::Has_value_parser<P>);
    STATIC_REQUIRE_FALSE(details_::Has_default_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_true_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_false_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_repeated_value_parser<P>);
}

struct Incomplete_custom_positional_parameter_5 {
    static constexpr auto name = "...";

    static auto parse_value(auto)
    {
        return 1;
    }
};

TEST_CASE("Parameter description is missing")
{
    using P = Incomplete_custom_positional_parameter_5;

    STATIC_REQUIRE_FALSE(Positional_parameter<P>);
    STATIC_REQUIRE_FALSE(Regular_parameter<P>);
    STATIC_REQUIRE_FALSE(Bool_parameter<P>);
    STATIC_REQUIRE_FALSE(Parameter<P>);

    namespace details_ = ez::cli::api::details_;
    STATIC_REQUIRE_FALSE(details_::Has_short_name<P>);
    STATIC_REQUIRE_FALSE(details_::Has_long_name<P>);
    STATIC_REQUIRE(details_::Has_unit_name<P>);
    STATIC_REQUIRE_FALSE(details_::Has_description<P>);
    STATIC_REQUIRE(details_::Has_value_parser<P>);
    STATIC_REQUIRE_FALSE(details_::Has_default_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_true_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_false_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_repeated_value_parser<P>);
}
