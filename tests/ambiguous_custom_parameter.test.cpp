#include <catch2/catch_all.hpp>
#include "ez/cli/parameter/concepts.h"

using ez::cli::api::Positional_parameter;
using ez::cli::api::Regular_parameter;
using ez::cli::api::Boolean_parameter;
using ez::cli::api::Parameter;


struct Ambigous_parameter_1 {
    static constexpr auto name = "...";
    static constexpr auto short_name = "...";
    static constexpr auto description = "...";

    static auto parse_value(auto)
    {
        return int{3};
    }
};

TEST_CASE("Ambiguity: param contains necessary minimum of both positional and regular params 1")
{
    using P = Ambigous_parameter_1;

    STATIC_REQUIRE_FALSE(Positional_parameter<P>);
    STATIC_REQUIRE_FALSE(Regular_parameter<P>);
    STATIC_REQUIRE_FALSE(Boolean_parameter<P>);
    STATIC_REQUIRE_FALSE(Parameter<P>);

    namespace details_ = ez::cli::api::details_;
    STATIC_REQUIRE(details_::Has_short_name<P>);
    STATIC_REQUIRE_FALSE(details_::Has_long_name<P>);
    STATIC_REQUIRE(details_::Has_unit_name<P>);
    STATIC_REQUIRE(details_::Has_description<P>);
    STATIC_REQUIRE(details_::Has_value_parser<P>);
    STATIC_REQUIRE_FALSE(details_::Has_default_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_true_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_false_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_parse_repeated_value<P>);
}


struct Ambigous_parameter_2 {
    static constexpr auto name = "...";
    static constexpr auto long_name = "...";
    static constexpr auto description = "...";

    static auto parse_value(auto)
    {
        return int{3};
    }
};

TEST_CASE("Ambiguity: param contains necessary minimum of both positional and regular params 2")
{
    using P = Ambigous_parameter_2;

    STATIC_REQUIRE_FALSE(Positional_parameter<P>);
    STATIC_REQUIRE_FALSE(Regular_parameter<P>);
    STATIC_REQUIRE_FALSE(Boolean_parameter<P>);
    STATIC_REQUIRE_FALSE(Parameter<P>);

    namespace details_ = ez::cli::api::details_;
    STATIC_REQUIRE_FALSE(details_::Has_short_name<P>);
    STATIC_REQUIRE(details_::Has_long_name<P>);
    STATIC_REQUIRE(details_::Has_unit_name<P>);
    STATIC_REQUIRE(details_::Has_description<P>);
    STATIC_REQUIRE(details_::Has_value_parser<P>);
    STATIC_REQUIRE_FALSE(details_::Has_default_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_true_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_false_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_parse_repeated_value<P>);
}

struct Ambigous_parameter_3 {
    static constexpr auto name = "...";
    static constexpr auto short_name = "...";
    static constexpr auto description = "...";

    static auto parse_value(auto)
    {
        return int{3};
    }

    static auto true_value()
    {
        return "true";
    }

    static auto false_value()
    {
        return "false";
    }
};

TEST_CASE("Ambiguity: parameter contains necessary minimum of both positional and bool parameters")
{
    using P = Ambigous_parameter_3;

    STATIC_REQUIRE_FALSE(Positional_parameter<P>);
    STATIC_REQUIRE_FALSE(Regular_parameter<P>);
    STATIC_REQUIRE_FALSE(Boolean_parameter<P>);
    STATIC_REQUIRE_FALSE(Parameter<P>);

    namespace details_ = ez::cli::api::details_;
    STATIC_REQUIRE(details_::Has_short_name<P>);
    STATIC_REQUIRE_FALSE(details_::Has_long_name<P>);
    STATIC_REQUIRE(details_::Has_unit_name<P>);
    STATIC_REQUIRE(details_::Has_description<P>);
    STATIC_REQUIRE(details_::Has_value_parser<P>);
    STATIC_REQUIRE_FALSE(details_::Has_default_value<P>);
    STATIC_REQUIRE(details_::Has_true_value<P>);
    STATIC_REQUIRE(details_::Has_false_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_parse_repeated_value<P>);
}


struct Ambigous_parameter_4 {
    static constexpr auto short_name = "...";
    static constexpr auto description = "...";

    static auto parse_value(auto)
    {
        return int{3};
    }

    static auto true_value()
    {
        return "true";
    }

    static auto false_value()
    {
        return "false";
    }
};

TEST_CASE("Ambiguity: parameter contains necessary minimum of both regular and bool parameters")
{
    using P = Ambigous_parameter_4;

    STATIC_REQUIRE_FALSE(Positional_parameter<P>);
    STATIC_REQUIRE_FALSE(Regular_parameter<P>);
    STATIC_REQUIRE_FALSE(Boolean_parameter<P>);
    STATIC_REQUIRE_FALSE(Parameter<P>);

    namespace details_ = ez::cli::api::details_;
    STATIC_REQUIRE(details_::Has_short_name<P>);
    STATIC_REQUIRE_FALSE(details_::Has_long_name<P>);
    STATIC_REQUIRE_FALSE(details_::Has_unit_name<P>);
    STATIC_REQUIRE(details_::Has_description<P>);
    STATIC_REQUIRE(details_::Has_value_parser<P>);
    STATIC_REQUIRE_FALSE(details_::Has_default_value<P>);
    STATIC_REQUIRE(details_::Has_true_value<P>);
    STATIC_REQUIRE(details_::Has_false_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_parse_repeated_value<P>);
}
