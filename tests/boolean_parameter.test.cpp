#include <catch2/catch_all.hpp>
#include <ez/cli/parameter.h>
#include <ez/cli/parameter/concepts.h>
#include <ez/cli/parameter/traits.h>

using ez::cli::concepts::Positional_param;
using ez::cli::concepts::Regular_param;
using ez::cli::concepts::Boolean_param;
using ez::cli::concepts::Param;
using ez::cli::traits::Param_value_t;
using namespace std::string_view_literals;

TEST_CASE("Boolean param may have only short name")
{
    using P = ez::cli::Boolean_param<"-s", "",
        "The parameter descritption.",
        [](std::true_type) {
            return 123;
        },
        [](std::false_type) {
            return 321;
        }
    >;

    STATIC_REQUIRE_FALSE(Positional_param<P>);
    STATIC_REQUIRE_FALSE(Regular_param<P>);
    STATIC_REQUIRE(Boolean_param<P>);
    STATIC_REQUIRE(Param<P>);

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

    STATIC_REQUIRE(P::short_name == "-s");
    STATIC_REQUIRE(P::description == "The parameter descritption.");
    STATIC_REQUIRE(P::true_value() == 123);
    STATIC_REQUIRE(P::false_value() == 321);
    STATIC_REQUIRE(std::same_as<Param_value_t<P>, int>);
}

TEST_CASE("Boolean param may have only long name")
{
    using P = ez::cli::Boolean_param<"", "--long-name",
        "The parameter descritption.",
        [](std::true_type) {
            return 123;
        },
        [](std::false_type) {
            return 321;
        }
    >;

    STATIC_REQUIRE_FALSE(Positional_param<P>);
    STATIC_REQUIRE_FALSE(Regular_param<P>);
    STATIC_REQUIRE(Boolean_param<P>);
    STATIC_REQUIRE(Param<P>);

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

    STATIC_REQUIRE(P::long_name == "--long-name");
    STATIC_REQUIRE(P::description == "The parameter descritption.");
    STATIC_REQUIRE(P::true_value() == 123);
    STATIC_REQUIRE(P::false_value() == 321);
    STATIC_REQUIRE(std::same_as<Param_value_t<P>, int>);
}

TEST_CASE("Boolean param may have both short and long names")
{
    using P = ez::cli::Boolean_param<"-s", "--long-name",
        "The parameter descritption.",
        [](std::true_type) {
            return 123;
        },
        [](std::false_type) {
            return 321;
        }
    >;

    STATIC_REQUIRE_FALSE(Positional_param<P>);
    STATIC_REQUIRE_FALSE(Regular_param<P>);
    STATIC_REQUIRE(Boolean_param<P>);
    STATIC_REQUIRE(Param<P>);

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

    STATIC_REQUIRE(P::short_name == "-s");
    STATIC_REQUIRE(P::long_name == "--long-name");
    STATIC_REQUIRE(P::description == "The parameter descritption.");
    STATIC_REQUIRE(P::true_value() == 123);
    STATIC_REQUIRE(P::false_value() == 321);
    STATIC_REQUIRE(std::same_as<Param_value_t<P>, int>);
}

namespace {

namespace details_ = ez::cli::details_;

template<ez::utils::Static_string param_short_name, ez::utils::Static_string param_long_name,
         ez::utils::Static_string param_description,
         auto... f>
struct Test_param :
    details_::True_value<f...>,
    details_::False_value<f...> {

    static constexpr auto short_name = param_short_name.up_to_null();
    static constexpr auto long_name = param_long_name.up_to_null();
    static constexpr auto description = param_description.up_to_null();
};

using Incorrect_param_types = std::tuple<
     // Both short and long param names are missing (empty strings).
    Test_param<"", "",
        "The parameter descritption.",
        [](std::true_type) { return 123; },
        [](std::false_type) { return 321; }
    >,

    // A param description is missing (empty string).
    Test_param<"-s", "",
        "",
        [](std::true_type) { return 123; },
        [](std::false_type) { return 321; }
    >,

    // A param description is missing (empty string).
    Test_param<"", "--long-name",
        "",
        [](std::true_type) { return 123; },
        [](std::false_type) { return 321; }
    >,

    // A param description is missing (empty string).
    Test_param<"-s", "--long-name",
        "",
        [](std::true_type) { return 123; },
        [](std::false_type) { return 321; }
    >,

    // An true-value and false-value functions are missing.
    Test_param<"-s", "",
        "The parameter descritption."
    >,

    // A true-value function is missing.
    Test_param<"-s", "--long-name",
        "The parameter descritption.",
        /*[](std::true_type) { return 123; },*/
        [](std::false_type) { return 321; }
    >,

    // A false-value function is missing.
    Test_param<"-s", "--long-name",
        "The parameter descritption.",
        [](std::true_type) { return 123; }/*,
        [](std::false_type) { return 321; }*/
    >
>;

} // namespace

TEMPLATE_LIST_TEST_CASE("Incorrect boolean param spec", "", Incorrect_param_types)
{
    using P = TestType;

    static_assert(!std::is_same_v<P, Incorrect_param_types>,
        "Ensure using TEMPLATE_LIST_TEST_CASE and not TEMPLATE_TEST_CASE.");

    STATIC_REQUIRE_FALSE(Positional_param<P>);
    STATIC_REQUIRE_FALSE(Regular_param<P>);
    STATIC_REQUIRE_FALSE(Boolean_param<P>);
    STATIC_REQUIRE_FALSE(Param<P>);
}
