#include <catch2/catch_all.hpp>
#include "ez/cli/parameter/concepts.h"
#include "ez/cli/parameter/traits.h"

using ez::cli::concepts::Positional_parameter;
using ez::cli::concepts::Regular_parameter;
using ez::cli::concepts::Boolean_parameter;
using ez::cli::concepts::Parameter;
using ez::cli::traits::Param_value_t;
using namespace std::string_view_literals;


struct Boolean_param_1 {
    static constexpr auto short_name = "short-name"sv;
    static constexpr auto description = "The parameter descritption."sv;

    static constexpr auto true_value() { return 123; }
    static constexpr auto false_value() { return 321; }
};

TEST_CASE("Boolean parameter may have only short name")
{
    using P = Boolean_param_1;

    STATIC_REQUIRE_FALSE(Positional_parameter<P>);
    STATIC_REQUIRE_FALSE(Regular_parameter<P>);
    STATIC_REQUIRE(Boolean_parameter<P>);
    STATIC_REQUIRE(Parameter<P>);

    STATIC_REQUIRE(std::is_same_v<Param_value_t<P>, int>);

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
}

struct Boolean_param_2 {
    static constexpr auto long_name = "long-name"sv;
    static constexpr auto description = "The parameter descritption."sv;

    static constexpr auto true_value() { return 123; }
    static constexpr auto false_value() { return 321; }
};

TEST_CASE("Boolean parameter may have only long name")
{
    using P = Boolean_param_2;

    STATIC_REQUIRE_FALSE(Positional_parameter<P>);
    STATIC_REQUIRE_FALSE(Regular_parameter<P>);
    STATIC_REQUIRE(Boolean_parameter<P>);
    STATIC_REQUIRE(Parameter<P>);

    STATIC_REQUIRE(std::is_same_v<Param_value_t<P>, int>);

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
}

struct Boolean_param_3 {
    static constexpr auto short_name = "short-name"sv;
    static constexpr auto long_name = "long-name"sv;
    static constexpr auto description = "The parameter descritption."sv;

    static constexpr auto true_value() { return 123; }

    static constexpr auto false_value() { return 321; }
};

TEST_CASE("Boolean parameter may have both short and long names")
{
    using P = Boolean_param_3;

    STATIC_REQUIRE_FALSE(Positional_parameter<P>);
    STATIC_REQUIRE_FALSE(Regular_parameter<P>);
    STATIC_REQUIRE(Boolean_parameter<P>);
    STATIC_REQUIRE(Parameter<P>);

    STATIC_REQUIRE(std::is_same_v<Param_value_t<P>, int>);

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
}


namespace {


template<ez::utils::Static_string str>
struct Mock_short_name {
    static constexpr auto short_name = std::string_view{str.value};
};

template<ez::utils::Static_string str>
struct Mock_long_name {
    static constexpr auto long_name = std::string_view{str.value};
};

template<ez::utils::Static_string str>
struct Mock_description {
    static constexpr auto description = std::string_view{str.value};
};


struct Mock_true_value_func {
    static constexpr auto true_value()
    {
        return 123;
    }
};


struct Mock_false_value_func {
    static constexpr auto false_value()
    {
        return 321;
    }
};


template<typename... Properties>
struct Compose_test_param : Properties... {};


using Incorrect_parameter_types = std::tuple<
    // A parameter name is not specified.
    Compose_test_param<
        /* Mock_short_name<"">, */
        /* Mock_long_name<"">, */
        Mock_description<"The parameter descritption.">,
        Mock_true_value_func,
        Mock_false_value_func
    >,

    // A parameter name is not specified (an empty string).
    Compose_test_param<
        Mock_short_name<"">,
        /* Mock_long_name<"">, */
        Mock_description<"The parameter descritption.">,
        Mock_true_value_func,
        Mock_false_value_func
    >,

    // A parameter name is not specified (empty string).
    Compose_test_param<
        /* Mock_short_name<"">, */
        Mock_long_name<"">,
        Mock_description<"The parameter descritption.">,
        Mock_true_value_func,
        Mock_false_value_func
    >,

    // A parameter name is not specified (empty string).
    Compose_test_param<
        Mock_short_name<"">,
        Mock_long_name<"">,
        Mock_description<"The parameter descritption.">,
        Mock_true_value_func,
        Mock_false_value_func
    >,

    // A parameter description is missing.
    Compose_test_param<
        Mock_short_name<"short-name">,
        Mock_long_name<"long-name">,
        /* Mock_description<"The parameter descritption.">, */
        Mock_true_value_func,
        Mock_false_value_func
    >,

    // A parameter description is missing (empty string).
    Compose_test_param<
        Mock_short_name<"short-name">,
        Mock_long_name<"long-name">,
        Mock_description<"">,
        Mock_true_value_func,
        Mock_false_value_func
    >,

    // A parameter true-value and false-value functions are missing.
    Compose_test_param<
        Mock_short_name<"">,
        Mock_long_name<"">,
        Mock_description<"The parameter descritption.">
        /* Mock_true_value_func, */
        /* Mock_false_value_func */
    >,

    // A parameter true-value function is missing.
    Compose_test_param<
        Mock_short_name<"">,
        Mock_long_name<"">,
        Mock_description<"The parameter descritption.">,
        /* Mock_true_value_func, */
        Mock_false_value_func
    >,

    // A parameter false-value function is missing.
    Compose_test_param<
        Mock_short_name<"">,
        Mock_long_name<"">,
        Mock_description<"The parameter descritption.">,
        Mock_true_value_func
        /* Mock_false_value_func */
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


