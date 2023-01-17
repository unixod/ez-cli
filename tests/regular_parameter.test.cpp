#include <catch2/catch_all.hpp>
#include "ez/cli/parameter.h"
#include "ez/cli/parameter/concepts.h"

using ez::cli::concepts::Positional_parameter;
using ez::cli::concepts::Regular_parameter;
using ez::cli::concepts::Boolean_parameter;
using ez::cli::concepts::Parameter;
using namespace std::string_view_literals;
using namespace std::string_literals;

TEST_CASE("Mandatory regular parameter (i.e. has no default value) may have only short name")
{
    using P = ez::cli::Regular_parameter<"short-name", "",
        "The parameter descritption.",
        [](auto arg) {
            return arg;
        }
    >;

    STATIC_REQUIRE(P::short_name == "short-name");
    STATIC_REQUIRE(P::description == "The parameter descritption.");
    STATIC_REQUIRE(P::parse_value("123") == "123");

    STATIC_REQUIRE_FALSE(Positional_parameter<P>);
    STATIC_REQUIRE(Regular_parameter<P>);
    STATIC_REQUIRE_FALSE(Boolean_parameter<P>);
    STATIC_REQUIRE(Parameter<P>);

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

TEST_CASE("Mandatory regular parameter (i.e. has no default value) may have only long name")
{
    using P = ez::cli::Regular_parameter<"", "long-name",
        "The parameter descritption.",
        [](auto arg) {
            return arg;
        }
    >;

    STATIC_REQUIRE(P::long_name == "long-name");
    STATIC_REQUIRE(P::description == "The parameter descritption.");
    STATIC_REQUIRE(P::parse_value("123") == "123");

    STATIC_REQUIRE_FALSE(Positional_parameter<P>);
    STATIC_REQUIRE(Regular_parameter<P>);
    STATIC_REQUIRE_FALSE(Boolean_parameter<P>);
    STATIC_REQUIRE(Parameter<P>);

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

TEST_CASE("Mandatory regular parameter (i.e. has no default value) may have both long&short names")
{
    using P = ez::cli::Regular_parameter<"short-name", "long-name",
        "The parameter descritption.",
        [](auto arg) {
            return arg;
        }
    >;

    STATIC_REQUIRE(P::short_name == "short-name");
    STATIC_REQUIRE(P::long_name == "long-name");
    STATIC_REQUIRE(P::description == "The parameter descritption.");
    STATIC_REQUIRE(P::parse_value("123") == "123");

    STATIC_REQUIRE_FALSE(Positional_parameter<P>);
    STATIC_REQUIRE(Regular_parameter<P>);
    STATIC_REQUIRE_FALSE(Boolean_parameter<P>);
    STATIC_REQUIRE(Parameter<P>);

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

TEST_CASE("Mandatory regular parameter may specify allowance for repetition 1")
{
    using P = ez::cli::Regular_parameter<"short-name", "",
        "The parameter descritption.",
        [](std::string_view arg) {
            return std::vector{arg};
        },
        [](auto& values, std::string_view arg) {
            values.push_back(arg);
        }
    >;

    STATIC_REQUIRE(P::short_name == "short-name");
    STATIC_REQUIRE(P::description == "The parameter descritption.");
    STATIC_REQUIRE(P::parse_value("123") == std::vector{"123"sv});
    std::vector<std::string_view> vs;
    P::parse_repeated_value(vs, "abc");
    REQUIRE(vs == std::vector{"abc"sv});

    STATIC_REQUIRE_FALSE(Positional_parameter<P>);
    STATIC_REQUIRE(Regular_parameter<P>);
    STATIC_REQUIRE_FALSE(Boolean_parameter<P>);
    STATIC_REQUIRE(Parameter<P>);

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

TEST_CASE("Mandatory regular parameter may specify allowance for repetition 2")
{
    using P = ez::cli::Regular_parameter<"", "long-name",
        "The parameter descritption.",
        [](std::string_view arg) {
            return std::vector{arg};
        },
        [](auto& values, std::string_view arg) {
            values.push_back(arg);
        }
    >;

    STATIC_REQUIRE(P::short_name == "");
    STATIC_REQUIRE(P::long_name == "long-name");
    STATIC_REQUIRE(P::description == "The parameter descritption.");
    REQUIRE(P::parse_value("123") == std::vector{"123"sv});
    std::vector<std::string_view> vs;
    P::parse_repeated_value(vs, "abc");
    REQUIRE(vs == std::vector{"abc"sv});

    STATIC_REQUIRE_FALSE(Positional_parameter<P>);
    STATIC_REQUIRE(Regular_parameter<P>);
    STATIC_REQUIRE_FALSE(Boolean_parameter<P>);
    STATIC_REQUIRE(Parameter<P>);

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

TEST_CASE("Mandatory regular parameter may specify allowance for repetition 3")
{
    using P = ez::cli::Regular_parameter<"short-name", "long-name",
        "The parameter descritption.",
        [](std::string_view arg) {
            return std::vector{arg};
        },
        [](auto& values, std::string_view arg) {
            values.push_back(arg);
        }
    >;

    STATIC_REQUIRE(P::short_name == "short-name");
    STATIC_REQUIRE(P::long_name == "long-name");
    STATIC_REQUIRE(P::description == "The parameter descritption.");
    REQUIRE(P::parse_value("123") == std::vector{"123"sv});
    std::vector<std::string_view> vs;
    P::parse_repeated_value(vs, "abc");
    REQUIRE(vs == std::vector{"abc"sv});

    STATIC_REQUIRE_FALSE(Positional_parameter<P>);
    STATIC_REQUIRE(Regular_parameter<P>);
    STATIC_REQUIRE_FALSE(Boolean_parameter<P>);
    STATIC_REQUIRE(Parameter<P>);

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

TEST_CASE("Optional regular parameter (i.e. has default value) may have only short name")
{
    using P = ez::cli::Regular_parameter<"short-name", "",
        "The parameter descritption.",
        [](auto arg) {
            return arg;
        },
        [] {
            return "default value"sv;
        }
    >;

    STATIC_REQUIRE(P::short_name == "short-name");
    STATIC_REQUIRE(P::description == "The parameter descritption.");
    STATIC_REQUIRE(P::parse_value("123") == "123");

    STATIC_REQUIRE_FALSE(Positional_parameter<P>);
    STATIC_REQUIRE(Regular_parameter<P>);
    STATIC_REQUIRE_FALSE(Boolean_parameter<P>);
    STATIC_REQUIRE(Parameter<P>);

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

TEST_CASE("Optional regular parameter (i.e. has default value) may have only long name")
{
    using P = ez::cli::Regular_parameter<"", "long-name",
        "The parameter descritption.",
        [](auto arg) {
            return arg;
        },
        [] {
            return "default value"sv;
        }
    >;

    STATIC_REQUIRE(P::long_name == "long-name");
    STATIC_REQUIRE(P::description == "The parameter descritption.");
    STATIC_REQUIRE(P::parse_value("123") == "123");

    STATIC_REQUIRE_FALSE(Positional_parameter<P>);
    STATIC_REQUIRE(Regular_parameter<P>);
    STATIC_REQUIRE_FALSE(Boolean_parameter<P>);
    STATIC_REQUIRE(Parameter<P>);

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

TEST_CASE("Optional regular parameter (i.e. has default value) may have both long&short names")
{
    using P = ez::cli::Regular_parameter<"short-name", "long-name",
        "The parameter descritption.",
        [](auto arg) {
            return arg;
        },
        [] {
            return "default value"sv;
        }
    >;

    STATIC_REQUIRE(P::short_name == "short-name");
    STATIC_REQUIRE(P::long_name == "long-name");
    STATIC_REQUIRE(P::description == "The parameter descritption.");
    STATIC_REQUIRE(P::parse_value("123") == "123");

    STATIC_REQUIRE_FALSE(Positional_parameter<P>);
    STATIC_REQUIRE(Regular_parameter<P>);
    STATIC_REQUIRE_FALSE(Boolean_parameter<P>);
    STATIC_REQUIRE(Parameter<P>);

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

TEST_CASE("Optional regular parameter may specify allowance for repetition 1")
{
    using P = ez::cli::Regular_parameter<"short-name", "",
        "The parameter descritption.",
        [](auto arg) {
            return std::vector{arg};
        },
        [] {
            return std::vector{"default value"sv};
        },
        [](auto& values, std::string_view arg) {
            values.push_back(arg);
        }
    >;

    STATIC_REQUIRE(P::short_name == "short-name");
    STATIC_REQUIRE(P::description == "The parameter descritption.");
    REQUIRE(P::parse_value("456") == std::vector{"456"sv});
    std::vector<std::string_view> vs;
    P::parse_repeated_value(vs, "789");
    REQUIRE(vs == std::vector{"789"sv});
    STATIC_REQUIRE(P::default_value() == std::vector{"default value"sv});

    STATIC_REQUIRE_FALSE(Positional_parameter<P>);
    STATIC_REQUIRE(Regular_parameter<P>);
    STATIC_REQUIRE_FALSE(Boolean_parameter<P>);
    STATIC_REQUIRE(Parameter<P>);

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

TEST_CASE("Optional regular parameter may specify allowance for repetition 2")
{
    using P = ez::cli::Regular_parameter<"", "long-name",
        "The parameter descritption.",
        [](auto arg) {
            return std::vector{arg};
        },
        [] {
            return std::vector{"default value"sv};
        },
        [](auto& values, std::string_view arg) {
            values.push_back(arg);
        }
    >;

    STATIC_REQUIRE(P::long_name == "long-name");
    STATIC_REQUIRE(P::description == "The parameter descritption.");
    REQUIRE(P::parse_value("456") == std::vector{"456"sv});
    std::vector<std::string_view> vs;
    P::parse_repeated_value(vs, "789");
    REQUIRE(vs == std::vector{"789"sv});
    STATIC_REQUIRE(P::default_value() == std::vector{"default value"sv});

    STATIC_REQUIRE_FALSE(Positional_parameter<P>);
    STATIC_REQUIRE(Regular_parameter<P>);
    STATIC_REQUIRE_FALSE(Boolean_parameter<P>);
    STATIC_REQUIRE(Parameter<P>);

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

TEST_CASE("Optional regular parameter may specify allowance for repetition 3")
{
    using P = ez::cli::Regular_parameter<"short-name", "long-name",
        "The parameter descritption.",
        [](auto arg) {
            return std::vector{arg};
        },
        [] {
            return std::vector{"default value"sv};
        },
        [](auto& values, std::string_view arg) {
            values.push_back(arg);
        }
    >;

    STATIC_REQUIRE(P::short_name == "short-name");
    STATIC_REQUIRE(P::long_name == "long-name");
    STATIC_REQUIRE(P::description == "The parameter descritption.");
    REQUIRE(P::parse_value("456") == std::vector{"456"sv});
    std::vector<std::string_view> vs;
    P::parse_repeated_value(vs, "789");
    REQUIRE(vs == std::vector{"789"sv});
    STATIC_REQUIRE(P::default_value() == std::vector{"default value"sv});

    STATIC_REQUIRE_FALSE(Positional_parameter<P>);
    STATIC_REQUIRE(Regular_parameter<P>);
    STATIC_REQUIRE_FALSE(Boolean_parameter<P>);
    STATIC_REQUIRE(Parameter<P>);

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

namespace {

using Incorrect_parameter_types = std::tuple<
    // Both short and long parameter names are missing (empty strings).
    ez::cli::Regular_parameter<"", "",
        "The parameter descritption.",
        [](auto arg) { return arg; }
    >,

    // A parameter description is missing (empty string).
    ez::cli::Regular_parameter<"short-name", "",
        "",
        [](auto arg) {
            return arg;
        }
    >,

    // A parameter description is missing (empty string).
    ez::cli::Regular_parameter<"", "long-name",
        "",
        [](auto arg) {
            return arg;
        }
    >,

    // An argument value parsing funciton is missing.
    ez::cli::Regular_parameter<"short-name", "",
        "The parameter descritption."
    >,

    //==-----------------------------------------------==//
    //  Ditto as above but with default value specified  //
    //==-----------------------------------------------==//

    // Both short and long parameter names are missing (empty strings).
    ez::cli::Regular_parameter<"", "",
        "The parameter descritption.",
        [](auto arg) {
            return arg;
        },
        [] {
            return "default value"sv;
        }
    >,

    // A parameter description is missing (empty string).
    ez::cli::Regular_parameter<"short-name", "",
        "",
        [](auto arg) {
            return arg;
        },
        [] {
            return "default value"sv;
        }
    >,

    // A parameter description is missing (empty string).
    ez::cli::Regular_parameter<"", "long-name",
        "",
        [](auto arg) {
            return arg;
        },
        [] {
            return "default value"sv;
        }
    >,

    // An argument value-parser funciton is missing.
    ez::cli::Regular_parameter<"short-name", "",
        "The parameter descritption.",
        [] {
            return "default value"sv;
        }
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
