#include <catch2/catch_all.hpp>
#include "ez/cli/parameter/concepts.h"
#include "ez/cli/parameter/traits.h"

using ez::cli::concepts::Positional_parameter;
using ez::cli::concepts::Regular_parameter;
using ez::cli::concepts::Boolean_parameter;
using ez::cli::concepts::Parameter;
using ez::cli::traits::Param_value_t;
using namespace std::string_view_literals;
using namespace std::string_literals;


struct Mandatory_regular_parameter_1 {
    static constexpr auto short_name = "short-name"sv;
    static constexpr auto description = "The parameter descritption."sv;

    static constexpr auto parse_value(auto arg)
    {
        static_assert(std::is_same_v<decltype(arg), std::string_view>);
        return arg;
    }
};

TEST_CASE("Regular parameter may specify no default value (i.e. mandatory parameter) 1")
{
    using P = Mandatory_regular_parameter_1;

    STATIC_REQUIRE_FALSE(Positional_parameter<P>);
    STATIC_REQUIRE(Regular_parameter<P>);
    STATIC_REQUIRE_FALSE(Boolean_parameter<P>);
    STATIC_REQUIRE(Parameter<P>);

    STATIC_REQUIRE(std::is_same_v<Param_value_t<P>, std::string_view>);

    namespace details_ = ez::cli::concepts::details_;
    STATIC_REQUIRE(details_::Has_short_name<P>);
    STATIC_REQUIRE_FALSE(details_::Has_long_name<P>);
    STATIC_REQUIRE_FALSE(details_::Has_unit_name<P>);
    STATIC_REQUIRE(details_::Has_description<P>);
    STATIC_REQUIRE(details_::Has_value_parser<P>);
    STATIC_REQUIRE_FALSE(details_::Has_default_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_true_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_false_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_parse_repeated_value<P>);
}

struct Mandatory_regular_parameter_2 {
    static constexpr auto long_name = "long-name"sv;
    static constexpr auto description = "The parameter descritption."sv;

    static constexpr auto parse_value(auto arg)
    {
        static_assert(std::is_same_v<decltype(arg), std::string_view>);
        return arg;
    }
};

TEST_CASE("Regular parameter may specify no default value (i.e. mandatory parameter) 2")
{
    using P = Mandatory_regular_parameter_2;

    STATIC_REQUIRE_FALSE(Positional_parameter<P>);
    STATIC_REQUIRE(Regular_parameter<P>);
    STATIC_REQUIRE_FALSE(Boolean_parameter<P>);
    STATIC_REQUIRE(Parameter<P>);

    STATIC_REQUIRE(std::is_same_v<Param_value_t<P>, std::string_view>);

    namespace details_ = ez::cli::concepts::details_;
    STATIC_REQUIRE_FALSE(details_::Has_short_name<P>);
    STATIC_REQUIRE(details_::Has_long_name<P>);
    STATIC_REQUIRE_FALSE(details_::Has_unit_name<P>);
    STATIC_REQUIRE(details_::Has_description<P>);
    STATIC_REQUIRE(details_::Has_value_parser<P>);
    STATIC_REQUIRE_FALSE(details_::Has_default_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_true_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_false_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_parse_repeated_value<P>);
}

struct Mandatory_regular_parameter_3 {
    static constexpr auto short_name = "short-name"sv;
    static constexpr auto long_name = "long-name"sv;
    static constexpr auto description = "The parameter descritption."sv;

    static constexpr auto parse_value(auto arg)
    {
        static_assert(std::is_same_v<decltype(arg), std::string_view>);
        return arg;
    }
};

TEST_CASE("Regular parameter may specify no default value (i.e. mandatory parameter) 3")
{
    using P = Mandatory_regular_parameter_3;

    STATIC_REQUIRE_FALSE(Positional_parameter<P>);
    STATIC_REQUIRE(Regular_parameter<P>);
    STATIC_REQUIRE_FALSE(Boolean_parameter<P>);
    STATIC_REQUIRE(Parameter<P>);

    STATIC_REQUIRE(std::is_same_v<Param_value_t<P>, std::string_view>);

    namespace details_ = ez::cli::concepts::details_;
    STATIC_REQUIRE(details_::Has_short_name<P>);
    STATIC_REQUIRE(details_::Has_long_name<P>);
    STATIC_REQUIRE_FALSE(details_::Has_unit_name<P>);
    STATIC_REQUIRE(details_::Has_description<P>);
    STATIC_REQUIRE(details_::Has_value_parser<P>);
    STATIC_REQUIRE_FALSE(details_::Has_default_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_true_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_false_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_parse_repeated_value<P>);
}

struct Mandatory_repeated_regular_param_1 {
    static constexpr auto short_name = "short-param"sv;
    static constexpr auto description = "The parameter descritption."sv;

    static constexpr auto parse_value(auto arg)
    {
        static_assert(std::is_same_v<decltype(arg), std::string_view>);
        return std::vector{arg};
    }

    static constexpr void parse_repeated_value(auto& values, std::string_view arg)
    {
        values.push_back(arg);
    }
};

TEST_CASE("Mandatory regular parameter may specify allowance for repetition 1")
{
    using P = Mandatory_repeated_regular_param_1;

    STATIC_REQUIRE_FALSE(Positional_parameter<P>);
    STATIC_REQUIRE(Regular_parameter<P>);
    STATIC_REQUIRE_FALSE(Boolean_parameter<P>);
    STATIC_REQUIRE(Parameter<P>);

    STATIC_REQUIRE(std::is_same_v<Param_value_t<P>, std::vector<std::string_view>>);

    namespace details_ = ez::cli::concepts::details_;
    STATIC_REQUIRE(details_::Has_short_name<P>);
    STATIC_REQUIRE_FALSE(details_::Has_long_name<P>);
    STATIC_REQUIRE_FALSE(details_::Has_unit_name<P>);
    STATIC_REQUIRE(details_::Has_description<P>);
    STATIC_REQUIRE(details_::Has_value_parser<P>);
    STATIC_REQUIRE_FALSE(details_::Has_default_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_true_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_false_value<P>);
    STATIC_REQUIRE(details_::Has_parse_repeated_value<P>);
}

struct Mandatory_repeated_regular_param_2 {
    static constexpr auto long_name = "long-param"sv;
    static constexpr auto description = "The parameter descritption."sv;

    static constexpr auto parse_value(auto arg)
    {
        static_assert(std::is_same_v<decltype(arg), std::string_view>);
        return std::vector{arg};
    }

    static constexpr void parse_repeated_value(auto& values, std::string_view arg)
    {
        values.push_back(arg);
    }
};

TEST_CASE("Mandatory regular parameter may specify allowance for repetition 2")
{
    using P = Mandatory_repeated_regular_param_2;

    STATIC_REQUIRE_FALSE(Positional_parameter<P>);
    STATIC_REQUIRE(Regular_parameter<P>);
    STATIC_REQUIRE_FALSE(Boolean_parameter<P>);
    STATIC_REQUIRE(Parameter<P>);

    STATIC_REQUIRE(std::is_same_v<Param_value_t<P>, std::vector<std::string_view>>);

    namespace details_ = ez::cli::concepts::details_;
    STATIC_REQUIRE_FALSE(details_::Has_short_name<P>);
    STATIC_REQUIRE(details_::Has_long_name<P>);
    STATIC_REQUIRE_FALSE(details_::Has_unit_name<P>);
    STATIC_REQUIRE(details_::Has_description<P>);
    STATIC_REQUIRE(details_::Has_value_parser<P>);
    STATIC_REQUIRE_FALSE(details_::Has_default_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_true_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_false_value<P>);
    STATIC_REQUIRE(details_::Has_parse_repeated_value<P>);
}

struct Mandatory_repeated_regular_param_3 {
    static constexpr auto short_name = "short-name"sv;
    static constexpr auto long_name = "long-name"sv;
    static constexpr auto description = "The parameter descritption."sv;

    static constexpr auto parse_value(auto arg)
    {
        static_assert(std::is_same_v<decltype(arg), std::string_view>);
        return std::vector{arg};
    }

    static constexpr void parse_repeated_value(auto& values, std::string_view arg)
    {
        values.push_back(arg);
    }
};

TEST_CASE("Mandatory regular parameter may specify allowance for repetition 3")
{
    using P = Mandatory_repeated_regular_param_3;

    STATIC_REQUIRE_FALSE(Positional_parameter<P>);
    STATIC_REQUIRE(Regular_parameter<P>);
    STATIC_REQUIRE_FALSE(Boolean_parameter<P>);
    STATIC_REQUIRE(Parameter<P>);

    STATIC_REQUIRE(std::is_same_v<Param_value_t<P>, std::vector<std::string_view>>);

    namespace details_ = ez::cli::concepts::details_;
    STATIC_REQUIRE(details_::Has_short_name<P>);
    STATIC_REQUIRE(details_::Has_long_name<P>);
    STATIC_REQUIRE_FALSE(details_::Has_unit_name<P>);
    STATIC_REQUIRE(details_::Has_description<P>);
    STATIC_REQUIRE(details_::Has_value_parser<P>);
    STATIC_REQUIRE_FALSE(details_::Has_default_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_true_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_false_value<P>);
    STATIC_REQUIRE(details_::Has_parse_repeated_value<P>);
}

struct Optional_regular_parameter_1 {
    static constexpr auto short_name = "short-name"sv;
    static constexpr auto description = "The parameter descritption."sv;

    static constexpr auto parse_value(auto arg)
    {
        static_assert(std::is_same_v<decltype(arg), std::string_view>);
        return arg;
    }

    static constexpr auto default_value()
    {
        return "default-value"sv;
    }
};

TEST_CASE("Regular parameter may specify a default value (i.e. optional parameter) 1")
{
    using P = Optional_regular_parameter_1;

    STATIC_REQUIRE_FALSE(Positional_parameter<P>);
    STATIC_REQUIRE(Regular_parameter<P>);
    STATIC_REQUIRE_FALSE(Boolean_parameter<P>);
    STATIC_REQUIRE(Parameter<P>);

    STATIC_REQUIRE(std::is_same_v<Param_value_t<P>, std::string_view>);

    namespace details_ = ez::cli::concepts::details_;
    STATIC_REQUIRE(details_::Has_short_name<P>);
    STATIC_REQUIRE_FALSE(details_::Has_long_name<P>);
    STATIC_REQUIRE_FALSE(details_::Has_unit_name<P>);
    STATIC_REQUIRE(details_::Has_description<P>);
    STATIC_REQUIRE(details_::Has_value_parser<P>);
    STATIC_REQUIRE(details_::Has_default_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_true_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_false_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_parse_repeated_value<P>);
}

struct Optional_regular_parameter_2 {
    static constexpr auto long_name = "long-name"sv;
    static constexpr auto description = "The parameter descritption."sv;

    static constexpr auto parse_value(auto arg)
    {
        static_assert(std::is_same_v<decltype(arg), std::string_view>);
        return arg;
    }

    static constexpr auto default_value()
    {
        return "default-value"sv;
    }
};

TEST_CASE("Regular parameter may specify a default value (i.e. optional parameter) 2")
{
    using P = Optional_regular_parameter_2;

    STATIC_REQUIRE_FALSE(Positional_parameter<P>);
    STATIC_REQUIRE(Regular_parameter<P>);
    STATIC_REQUIRE_FALSE(Boolean_parameter<P>);
    STATIC_REQUIRE(Parameter<P>);

    STATIC_REQUIRE(std::is_same_v<Param_value_t<P>, std::string_view>);

    namespace details_ = ez::cli::concepts::details_;
    STATIC_REQUIRE_FALSE(details_::Has_short_name<P>);
    STATIC_REQUIRE(details_::Has_long_name<P>);
    STATIC_REQUIRE_FALSE(details_::Has_unit_name<P>);
    STATIC_REQUIRE(details_::Has_description<P>);
    STATIC_REQUIRE(details_::Has_value_parser<P>);
    STATIC_REQUIRE(details_::Has_default_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_true_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_false_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_parse_repeated_value<P>);
}

struct Optional_regular_parameter_3 {
    static constexpr auto short_name = "short-name"sv;
    static constexpr auto long_name = "long-name"sv;
    static constexpr auto description = "The parameter descritption."sv;

    static constexpr auto parse_value(auto arg)
    {
        static_assert(std::is_same_v<decltype(arg), std::string_view>);
        return arg;
    }

    static constexpr auto default_value()
    {
        return "default-value"sv;
    }
};

TEST_CASE("Regular parameter may specify a default value (i.e. optional parameter) 3")
{
    using P = Optional_regular_parameter_3;

    STATIC_REQUIRE_FALSE(Positional_parameter<P>);
    STATIC_REQUIRE(Regular_parameter<P>);
    STATIC_REQUIRE_FALSE(Boolean_parameter<P>);
    STATIC_REQUIRE(Parameter<P>);

    STATIC_REQUIRE(std::is_same_v<Param_value_t<P>, std::string_view>);

    namespace details_ = ez::cli::concepts::details_;
    STATIC_REQUIRE(details_::Has_short_name<P>);
    STATIC_REQUIRE(details_::Has_long_name<P>);
    STATIC_REQUIRE_FALSE(details_::Has_unit_name<P>);
    STATIC_REQUIRE(details_::Has_description<P>);
    STATIC_REQUIRE(details_::Has_value_parser<P>);
    STATIC_REQUIRE(details_::Has_default_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_true_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_false_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_parse_repeated_value<P>);
}

struct Optional_repeated_regular_param_1 {
    static constexpr auto short_name = "short-name"sv;
    static constexpr auto description = "The parameter descritption."sv;

    static constexpr auto parse_value(auto arg)
    {
        static_assert(std::is_same_v<decltype(arg), std::string_view>);
        return std::vector{arg};
    }

    static constexpr void parse_repeated_value(auto& values, std::string_view arg)
    {
        values.push_back(arg);
    }

    static constexpr auto default_value()
    {
        return std::vector{"default-value"sv};
    }
};

TEST_CASE("Optional regular parameter may specify allowance for repetition 1")
{
    using P = Optional_repeated_regular_param_1;

    STATIC_REQUIRE_FALSE(Positional_parameter<P>);
    STATIC_REQUIRE(Regular_parameter<P>);
    STATIC_REQUIRE_FALSE(Boolean_parameter<P>);
    STATIC_REQUIRE(Parameter<P>);

    STATIC_REQUIRE(std::is_same_v<Param_value_t<P>, std::vector<std::string_view>>);

    namespace details_ = ez::cli::concepts::details_;
    STATIC_REQUIRE(details_::Has_short_name<P>);
    STATIC_REQUIRE_FALSE(details_::Has_long_name<P>);
    STATIC_REQUIRE_FALSE(details_::Has_unit_name<P>);
    STATIC_REQUIRE(details_::Has_description<P>);
    STATIC_REQUIRE(details_::Has_value_parser<P>);
    STATIC_REQUIRE(details_::Has_default_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_true_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_false_value<P>);
    STATIC_REQUIRE(details_::Has_parse_repeated_value<P>);
}

struct Optional_repeated_regular_param_2 {
    static constexpr auto long_name = "long-name"sv;
    static constexpr auto description = "The parameter descritption."sv;

    static constexpr auto parse_value(auto arg)
    {
        static_assert(std::is_same_v<decltype(arg), std::string_view>);
        return std::vector{arg};
    }

    static constexpr void parse_repeated_value(auto& values, std::string_view arg)
    {
        values.push_back(arg);
    }

    static constexpr auto default_value()
    {
        return std::vector{"default-value"sv};
    }
};

TEST_CASE("Optional regular parameter may specify allowance for repetition 2")
{
    using P = Optional_repeated_regular_param_2;

    STATIC_REQUIRE_FALSE(Positional_parameter<P>);
    STATIC_REQUIRE(Regular_parameter<P>);
    STATIC_REQUIRE_FALSE(Boolean_parameter<P>);
    STATIC_REQUIRE(Parameter<P>);

    STATIC_REQUIRE(std::is_same_v<Param_value_t<P>, std::vector<std::string_view>>);

    namespace details_ = ez::cli::concepts::details_;
    STATIC_REQUIRE_FALSE(details_::Has_short_name<P>);
    STATIC_REQUIRE(details_::Has_long_name<P>);
    STATIC_REQUIRE_FALSE(details_::Has_unit_name<P>);
    STATIC_REQUIRE(details_::Has_description<P>);
    STATIC_REQUIRE(details_::Has_value_parser<P>);
    STATIC_REQUIRE(details_::Has_default_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_true_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_false_value<P>);
    STATIC_REQUIRE(details_::Has_parse_repeated_value<P>);
}

struct Optional_repeated_regular_param_3 {
    static constexpr auto short_name = "short-name"sv;
    static constexpr auto long_name = "long-name"sv;
    static constexpr auto description = "The parameter descritption."sv;

    static constexpr auto parse_value(auto arg)
    {
        static_assert(std::is_same_v<decltype(arg), std::string_view>);
        return std::vector{arg};
    }

    static constexpr void parse_repeated_value(auto& values, std::string_view arg)
    {
        values.push_back(arg);
    }

    static constexpr auto default_value()
    {
        return std::vector{"default-value"sv};
    }
};

TEST_CASE("Optional regular parameter may specify allowance for repetition 3")
{
    using P = Optional_repeated_regular_param_3;

    STATIC_REQUIRE_FALSE(Positional_parameter<P>);
    STATIC_REQUIRE(Regular_parameter<P>);
    STATIC_REQUIRE_FALSE(Boolean_parameter<P>);
    STATIC_REQUIRE(Parameter<P>);

    STATIC_REQUIRE(std::is_same_v<Param_value_t<P>, std::vector<std::string_view>>);

    namespace details_ = ez::cli::concepts::details_;
    STATIC_REQUIRE(details_::Has_short_name<P>);
    STATIC_REQUIRE(details_::Has_long_name<P>);
    STATIC_REQUIRE_FALSE(details_::Has_unit_name<P>);
    STATIC_REQUIRE(details_::Has_description<P>);
    STATIC_REQUIRE(details_::Has_value_parser<P>);
    STATIC_REQUIRE(details_::Has_default_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_true_value<P>);
    STATIC_REQUIRE_FALSE(details_::Has_false_value<P>);
    STATIC_REQUIRE(details_::Has_parse_repeated_value<P>);
}

// Testing of incorrect parameter types detection. ------------------

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

template<auto f>
struct Mock_parse_value_func {
    static constexpr auto parse_value(auto arg)
    {
        return f(arg);
    }
};


template<typename... Args>
struct Mock_parse_repeated_value_func {
    static constexpr auto parse_repeated_value(Args...)
    {}
};

template<auto f>
struct Mock_default_value_func {
    static constexpr auto default_value()
    {
        return f();
    }
};


template<typename... Properties>
struct Compose_test_param : Properties... {};


using Incorrect_parameter_types = std::tuple<
    // None of long or short parameter names are specified (either misssing or
    // emty strings).
    Compose_test_param<
        Mock_short_name<"">,
        Mock_description<"The parameter description">,
        Mock_parse_value_func<[](auto arg) { return arg; }>
    >,

    // None of long or short parameter names are specified (either misssing or
    // emty strings).
    Compose_test_param<
        Mock_long_name<"">,
        Mock_description<"The parameter description">,
        Mock_parse_value_func<[](auto arg) { return arg; }>
    >,

    // None of long or short parameter names are specified (either misssing or
    // emty strings).
    Compose_test_param<
        Mock_short_name<"">,
        Mock_long_name<"">,
        Mock_description<"The parameter description">,
        Mock_parse_value_func<[](auto arg) { return arg; }>
    >,

    // None of long or short parameter names are specified (either misssing or
    // emty strings).
    Compose_test_param<
        Mock_short_name<"">,
        Mock_long_name<"">,
        Mock_description<"The parameter description">,
        Mock_parse_value_func<[](auto arg) { return arg; }>
    >,

    // Paremeter description is empty string.
    Compose_test_param<
        Mock_short_name<"some-param">,
        Mock_description<"">,
        Mock_parse_value_func<[](auto arg) { return arg; }>
    >,

    // Paremeter description is empty string.
    Compose_test_param<
        Mock_long_name<"some-param">,
        Mock_description<"">,
        Mock_parse_value_func<[](auto arg) { return arg; }>
    >,

    // Parameter description is missing.
    Compose_test_param<
        Mock_short_name<"some-param">,
        /* Mock_description<"The parameter description">, */
        Mock_parse_value_func<[](auto arg) { return arg; }>
    >,

    // Parameter description is missing.
    Compose_test_param<
        Mock_long_name<"some-param">,
        /* Mock_description<"The parameter description">, */
        Mock_parse_value_func<[](auto arg) { return arg; }>
    >,

    // Parameter value parsing function is missing.
    Compose_test_param<
        Mock_short_name<"some-param">,
        Mock_description<"The parameter description">
        /* Mock_parse_value_func<[](auto arg) { return arg; }>, */
    >,

    // Parameter value parsing function is missing.
    Compose_test_param<
        Mock_long_name<"some-param">,
        Mock_description<"The parameter description">
        /* Mock_parse_value_func<[](auto arg) { return arg; }>, */
    >,

   // Parameter default value type doesn't match return value type of
   // the parameter value parsing function.
    Compose_test_param<
        Mock_short_name<"some-param">,
        Mock_description<"The parameter description">,
        Mock_parse_value_func<[](auto) { return "parsed-value"; }>,
        Mock_default_value_func<[] { return 3; }>
    >,

    // A return value of parameter parse_value function can't be passed to
    // presenting parse_repeated_value funciton as a first argument.
    Compose_test_param<
        Mock_short_name<"some-param">,
        Mock_description<"The parameter description">,
        Mock_parse_value_func<[](auto) { return 3; }>,
        Mock_parse_repeated_value_func<std::vector<int>&, std::string_view>
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
