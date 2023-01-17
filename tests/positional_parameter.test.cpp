#include <catch2/catch_all.hpp>
#include "ez/cli/parameter.h"
#include "ez/cli/parameter/concepts.h"

using ez::cli::concepts::Positional_parameter;
using ez::cli::concepts::Regular_parameter;
using ez::cli::concepts::Boolean_parameter;
using ez::cli::concepts::Parameter;
using namespace std::string_view_literals;

TEST_CASE("Positional parameter may specify no default value (i.e. mandatory parameter)")
{
    using P = ez::cli::Positional_parameter<"some-param",
        "The parameter descritption.",
        [](auto sv) {
            return sv;
        }
    >;

    STATIC_REQUIRE(P::name == "some-param");
    STATIC_REQUIRE(P::description == "The parameter descritption.");
    STATIC_REQUIRE(P::parse_value("123") == "123");

    STATIC_REQUIRE(Positional_parameter<P>);
    STATIC_REQUIRE_FALSE(Regular_parameter<P>);
    STATIC_REQUIRE_FALSE(Boolean_parameter<P>);
    STATIC_REQUIRE(Parameter<P>);

    namespace details_ = ez::cli::concepts::details_;
    STATIC_REQUIRE_FALSE(details_::Has_short_name<P>);
    STATIC_REQUIRE_FALSE(details_::Has_long_name<P>);
    STATIC_REQUIRE(details_::Has_unit_name<P>);
    STATIC_REQUIRE(details_::Has_description<P>);
    STATIC_REQUIRE(details_::Has_value_parser<P>);
    STATIC_REQUIRE_FALSE(details_::Has_default_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_true_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_false_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_parse_repeated_value<P>);
}

TEST_CASE("Mandatory positional parameter may specify allowance for repetition")
{
    using P = ez::cli::Positional_parameter<"some-param",
        "The parameter descritption.",
        [](std::string_view arg) {
            return std::vector{arg};
        },
        [](auto& values, std::string_view arg) {
            values.push_back(arg);
        }
    >;

    STATIC_REQUIRE(P::name == "some-param");
    STATIC_REQUIRE(P::description == "The parameter descritption.");
    STATIC_REQUIRE(P::parse_value("123") == std::vector{"123"sv});
    std::vector<std::string_view> vs;
    P::parse_repeated_value(vs, "abc");
    REQUIRE(vs == std::vector{"abc"sv});

    STATIC_REQUIRE(Positional_parameter<P>);
    STATIC_REQUIRE_FALSE(Regular_parameter<P>);
    STATIC_REQUIRE_FALSE(Boolean_parameter<P>);
    STATIC_REQUIRE(Parameter<P>);

    namespace details_ = ez::cli::concepts::details_;
    STATIC_REQUIRE_FALSE(details_::Has_short_name<P>);
    STATIC_REQUIRE_FALSE(details_::Has_long_name<P>);
    STATIC_REQUIRE(details_::Has_unit_name<P>);
    STATIC_REQUIRE(details_::Has_description<P>);
    STATIC_REQUIRE(details_::Has_value_parser<P>);
    STATIC_REQUIRE_FALSE(details_::Has_default_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_true_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_false_value<P>);
    STATIC_REQUIRE(details_::Has_parse_repeated_value<P>);
}

TEST_CASE("Positional parameter may specify default value (i.e. optional parameter)")
{
    using P = ez::cli::Positional_parameter<"some-param",
        "The parameter descritption.",
        [](auto sv) {
            return sv;
        },
        []{
            return "default-value"sv;
        }
    >;

    STATIC_REQUIRE(P::name == "some-param");
    STATIC_REQUIRE(P::description == "The parameter descritption.");
    STATIC_REQUIRE(P::parse_value("456") == "456");
    STATIC_REQUIRE(P::default_value() == "default-value");

    STATIC_REQUIRE(Positional_parameter<P>);
    STATIC_REQUIRE_FALSE(Regular_parameter<P>);
    STATIC_REQUIRE_FALSE(Boolean_parameter<P>);
    STATIC_REQUIRE(Parameter<P>);

    namespace details_ = ez::cli::concepts::details_;
    STATIC_REQUIRE_FALSE(details_::Has_short_name<P>);
    STATIC_REQUIRE_FALSE(details_::Has_long_name<P>);
    STATIC_REQUIRE(details_::Has_unit_name<P>);
    STATIC_REQUIRE(details_::Has_description<P>);
    STATIC_REQUIRE(details_::Has_value_parser<P>);
    STATIC_REQUIRE(details_::Has_default_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_true_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_false_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_parse_repeated_value<P>);
}

TEST_CASE("Optional positional parameter may specify allowance for repetition")
{
    using P = ez::cli::Positional_parameter<"some-param",
        "The parameter descritption.",
        [](std::string_view arg) {
            return std::vector{arg};
        },
        [](auto& values, std::string_view arg) {
            values.push_back(arg);
        },
        []{
            return std::vector{"default-value"sv};
        }
    >;

    STATIC_REQUIRE(P::name == "some-param");
    STATIC_REQUIRE(P::description == "The parameter descritption.");
    STATIC_REQUIRE(P::parse_value("123") == std::vector{"123"sv});
    std::vector<std::string_view> vs;
    P::parse_repeated_value(vs, "abc");
    REQUIRE(vs == std::vector{"abc"sv});

    STATIC_REQUIRE(Positional_parameter<P>);
    STATIC_REQUIRE_FALSE(Regular_parameter<P>);
    STATIC_REQUIRE_FALSE(Boolean_parameter<P>);
    STATIC_REQUIRE(Parameter<P>);

    namespace details_ = ez::cli::concepts::details_;
    STATIC_REQUIRE_FALSE(details_::Has_short_name<P>);
    STATIC_REQUIRE_FALSE(details_::Has_long_name<P>);
    STATIC_REQUIRE(details_::Has_unit_name<P>);
    STATIC_REQUIRE(details_::Has_description<P>);
    STATIC_REQUIRE(details_::Has_value_parser<P>);
    STATIC_REQUIRE(details_::Has_default_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_true_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_false_value<P>);
    STATIC_REQUIRE(details_::Has_parse_repeated_value<P>);
}

namespace {

using Incorrect_parameter_types = std::tuple<
    // A parameter name is missing (empty string).
    ez::cli::Positional_parameter<"",
        "A descritption of the param.",
        [](auto arg) {
            return arg;
        }
    >,

    // A parameter description is missing (empty string).
    ez::cli::Positional_parameter<"some-param",
        "",
        [](auto arg) {
            return arg;
        }
    >,

    // An argument value parsing funciton is missing.
    ez::cli::Positional_parameter<"some-param",
        "A descritption of the param."
    >,

    //==-----------------------------------------------==//
    //  Ditto as above but with default value specified  //
    //==-----------------------------------------------==//

    // A parameter name is missing (empty string).
    ez::cli::Positional_parameter<"",
        "A descritption of the param.",
        [](auto arg) {
            return arg;
        },
        [] {
            return "default value"sv;
        }
    >,

    // A parameter description is missing (empty string).
    ez::cli::Positional_parameter<"some-param",
        "",
        [](auto arg) {
            return arg;
        },
        [] {
            return "default value"sv;
        }
    >,

    // An argument value parsing funciton is missing.
    ez::cli::Positional_parameter<"some-param",
        "A descritption of the param.",
        [] {
            return "default value"sv;
        }
    >
>;

} // namespace

TEMPLATE_LIST_TEST_CASE("Incorrect positional parameter spec", "", Incorrect_parameter_types)
{
    using P = TestType;

    static_assert(!std::is_same_v<P, Incorrect_parameter_types>,
        "Ensure using TEMPLATE_LIST_TEST_CASE and not TEMPLATE_TEST_CASE.");

    STATIC_REQUIRE_FALSE(Positional_parameter<P>);
    STATIC_REQUIRE_FALSE(Regular_parameter<P>);
    STATIC_REQUIRE_FALSE(Boolean_parameter<P>);
    STATIC_REQUIRE_FALSE(Parameter<P>);
}

