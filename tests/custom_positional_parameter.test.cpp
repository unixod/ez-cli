#include <catch2/catch_all.hpp>
#include "ez/cli/parameter/concepts.h"

using ez::cli::api::Positional_parameter;
using ez::cli::api::Regular_parameter;
using ez::cli::api::Bool_parameter;
using ez::cli::api::Parameter;
using namespace std::string_view_literals;


struct Mandatory_positional_parameter {
    static constexpr auto name = "Mandatory_positional_parameter"sv;
    static constexpr auto description = "A description of Mandatory_positional_parameter."sv;

    static constexpr auto parse_value(std::string_view arg)
    {
        return arg;
    }
};

TEST_CASE("Mandatory_positional_parameter (i.e. has no default value)")
{
    using P = Mandatory_positional_parameter;

    STATIC_REQUIRE(P::name == "Mandatory_positional_parameter");
    STATIC_REQUIRE(P::description == "A description of Mandatory_positional_parameter.");
    STATIC_REQUIRE(P::parse_value("123") == "123");

    STATIC_REQUIRE(Positional_parameter<P>);
    STATIC_REQUIRE_FALSE(Regular_parameter<P>);
    STATIC_REQUIRE_FALSE(Bool_parameter<P>);
    STATIC_REQUIRE(Parameter<P>);

    namespace details_ = ez::cli::api::details_;
    STATIC_REQUIRE_FALSE(details_::Has_short_name<P>);
    STATIC_REQUIRE_FALSE(details_::Has_long_name<P>);
    STATIC_REQUIRE(details_::Has_unit_name<P>);
    STATIC_REQUIRE(details_::Has_description<P>);
    STATIC_REQUIRE(details_::Has_value_parser<P>);
    STATIC_REQUIRE_FALSE(details_::Has_default_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_true_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_false_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_repeated_value_parser<P>);
}

struct Optional_positional_parameter {
    static constexpr auto name = "Optional_positional_parameter"sv;
    static constexpr auto description = "A description of Optional_positional_parameter."sv;

    static constexpr auto parse_value(std::string_view arg)
    {
        return arg;
    }

    static constexpr auto default_value()
    {
        return "default-value"sv;
    }
};

TEST_CASE("Optional_positional_parameter (i.e. has default value)")
{
    using P = Optional_positional_parameter;

    STATIC_REQUIRE(P::name == "Optional_positional_parameter");
    STATIC_REQUIRE(P::description == "A description of Optional_positional_parameter.");
    STATIC_REQUIRE(P::parse_value("456") == "456");
    STATIC_REQUIRE(P::default_value() == "default-value");

    STATIC_REQUIRE(Positional_parameter<P>);
    STATIC_REQUIRE_FALSE(Regular_parameter<P>);
    STATIC_REQUIRE_FALSE(Bool_parameter<P>);
    STATIC_REQUIRE(Parameter<P>);

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
