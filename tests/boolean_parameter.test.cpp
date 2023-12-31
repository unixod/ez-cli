#include <catch2/catch_all.hpp>
#include "ez/cli/parameter.h"
#include "ez/cli/parameter/concepts.h"
#include "ez/cli/parameter/traits.h"

using ez::cli::concepts::Positional_parameter;
using ez::cli::concepts::Regular_parameter;
using ez::cli::concepts::Boolean_parameter;
using ez::cli::concepts::Parameter;
using ez::cli::traits::Param_value_t;
using namespace std::string_view_literals;

TEST_CASE("Boolean parameter may have only short name")
{
    using P = ez::cli::Boolean_parameter<"short-name", "",
        "The parameter descritption.",
        [](std::true_type) {
            return 123;
        },
        [](std::false_type) {
            return 321;
        }
    >;

    STATIC_REQUIRE_FALSE(Positional_parameter<P>);
    STATIC_REQUIRE_FALSE(Regular_parameter<P>);
    STATIC_REQUIRE(Boolean_parameter<P>);
    STATIC_REQUIRE(Parameter<P>);

    namespace details_ = ez::cli::concepts::details_;
    STATIC_REQUIRE(details_::Has_short_name<P>);
    STATIC_REQUIRE_FALSE(details_::Has_long_name<P>);
    STATIC_REQUIRE_FALSE(details_::Has_unit_name<P>);
    STATIC_REQUIRE(details_::Has_description<P>);
    STATIC_REQUIRE_FALSE(details_::Has_value_parser<P>);
    STATIC_REQUIRE_FALSE(details_::Has_default_value<P>);
    STATIC_REQUIRE(details_::Has_true_value<P>);
    STATIC_REQUIRE(details_::Has_false_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_parse_repeated_value<P>);

    STATIC_REQUIRE(P::short_name == "short-name");
    STATIC_REQUIRE(P::description == "The parameter descritption.");
    STATIC_REQUIRE(P::true_value() == 123);
    STATIC_REQUIRE(P::false_value() == 321);
    STATIC_REQUIRE(std::same_as<Param_value_t<P>, int>);
}

TEST_CASE("Boolean parameter may have only long name")
{
    using P = ez::cli::Boolean_parameter<"", "long-name",
        "The parameter descritption.",
        [](std::true_type) {
            return 123;
        },
        [](std::false_type) {
            return 321;
        }
    >;

    STATIC_REQUIRE_FALSE(Positional_parameter<P>);
    STATIC_REQUIRE_FALSE(Regular_parameter<P>);
    STATIC_REQUIRE(Boolean_parameter<P>);
    STATIC_REQUIRE(Parameter<P>);

    namespace details_ = ez::cli::concepts::details_;
    STATIC_REQUIRE_FALSE(details_::Has_short_name<P>);
    STATIC_REQUIRE(details_::Has_long_name<P>);
    STATIC_REQUIRE_FALSE(details_::Has_unit_name<P>);
    STATIC_REQUIRE(details_::Has_description<P>);
    STATIC_REQUIRE_FALSE(details_::Has_value_parser<P>);
    STATIC_REQUIRE_FALSE(details_::Has_default_value<P>);
    STATIC_REQUIRE(details_::Has_true_value<P>);
    STATIC_REQUIRE(details_::Has_false_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_parse_repeated_value<P>);

    STATIC_REQUIRE(P::long_name == "long-name");
    STATIC_REQUIRE(P::description == "The parameter descritption.");
    STATIC_REQUIRE(P::true_value() == 123);
    STATIC_REQUIRE(P::false_value() == 321);
    STATIC_REQUIRE(std::same_as<Param_value_t<P>, int>);
}

TEST_CASE("Boolean parameter may have both short and long names")
{
    using P = ez::cli::Boolean_parameter<"short-name", "long-name",
        "The parameter descritption.",
        [](std::true_type) {
            return 123;
        },
        [](std::false_type) {
            return 321;
        }
    >;

    STATIC_REQUIRE_FALSE(Positional_parameter<P>);
    STATIC_REQUIRE_FALSE(Regular_parameter<P>);
    STATIC_REQUIRE(Boolean_parameter<P>);
    STATIC_REQUIRE(Parameter<P>);

    namespace details_ = ez::cli::concepts::details_;
    STATIC_REQUIRE(details_::Has_short_name<P>);
    STATIC_REQUIRE(details_::Has_long_name<P>);
    STATIC_REQUIRE_FALSE(details_::Has_unit_name<P>);
    STATIC_REQUIRE(details_::Has_description<P>);
    STATIC_REQUIRE_FALSE(details_::Has_value_parser<P>);
    STATIC_REQUIRE_FALSE(details_::Has_default_value<P>);
    STATIC_REQUIRE(details_::Has_true_value<P>);
    STATIC_REQUIRE(details_::Has_false_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_parse_repeated_value<P>);

    STATIC_REQUIRE(P::short_name == "short-name");
    STATIC_REQUIRE(P::long_name == "long-name");
    STATIC_REQUIRE(P::description == "The parameter descritption.");
    STATIC_REQUIRE(P::true_value() == 123);
    STATIC_REQUIRE(P::false_value() == 321);
    STATIC_REQUIRE(std::same_as<Param_value_t<P>, int>);
}

namespace {

using Incorrect_parameter_types = std::tuple<
     // Both short and long parameter names are missing (empty strings).
    ez::cli::Boolean_parameter<"", "",
        "The parameter descritption.",
        [](std::true_type) { return 123; },
        [](std::false_type) { return 321; }
    >,

    // A parameter description is missing (empty string).
    ez::cli::Boolean_parameter<"short-name", "",
        "",
        [](std::true_type) { return 123; },
        [](std::false_type) { return 321; }
    >,

    // A parameter description is missing (empty string).
    ez::cli::Boolean_parameter<"", "long-name",
        "",
        [](std::true_type) { return 123; },
        [](std::false_type) { return 321; }
    >,

    // A parameter description is missing (empty string).
    ez::cli::Boolean_parameter<"short-name", "long-name",
        "",
        [](std::true_type) { return 123; },
        [](std::false_type) { return 321; }
    >,

    // An true-value and false-value functions are missing.
    ez::cli::Boolean_parameter<"short-name", "",
        "The parameter descritption."
    >,

    // A true-value function is missing.
    ez::cli::Boolean_parameter<"short-name", "long-name",
        "The parameter descritption.",
        /*[](std::true_type) { return 123; },*/
        [](std::false_type) { return 321; }
    >,

    // A false-value function is missing.
    ez::cli::Boolean_parameter<"short-name", "long-name",
        "The parameter descritption.",
        [](std::true_type) { return 123; }/*,
        [](std::false_type) { return 321; }*/
    >
>;

} // namespace

TEMPLATE_LIST_TEST_CASE("I", "", Incorrect_parameter_types)
{
    using P = TestType;

    static_assert(!std::is_same_v<P, Incorrect_parameter_types>,
        "Ensure using TEMPLATE_LIST_TEST_CASE and not TEMPLATE_TEST_CASE.");

    STATIC_REQUIRE_FALSE(Positional_parameter<P>);
    STATIC_REQUIRE_FALSE(Regular_parameter<P>);
    STATIC_REQUIRE_FALSE(Boolean_parameter<P>);
    STATIC_REQUIRE_FALSE(Parameter<P>);
}
