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
using namespace std::string_literals;

TEST_CASE("Mandatory regular param (i.e. has no default value) may have only short name")
{
    using P = ez::cli::Regular_param<"-s", "",
        "The parameter descritption.",
        [](auto arg) {
            return arg;
        }
    >;

    STATIC_REQUIRE_FALSE(Positional_param<P>);
    STATIC_REQUIRE(Regular_param<P>);
    STATIC_REQUIRE_FALSE(Boolean_param<P>);
    STATIC_REQUIRE(Param<P>);

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

    STATIC_REQUIRE(P::short_name == "-s");
    STATIC_REQUIRE(P::description == "The parameter descritption.");
    STATIC_REQUIRE(P::parse_value("123") == "123");
    STATIC_REQUIRE(std::same_as<Param_value_t<P>, std::string_view>);
}

TEST_CASE("Mandatory regular param (i.e. has no default value) may have only long name")
{
    using P = ez::cli::Regular_param<"", "--long-name",
        "The parameter descritption.",
        [](auto arg) {
            return arg;
        }
    >;

    STATIC_REQUIRE_FALSE(Positional_param<P>);
    STATIC_REQUIRE(Regular_param<P>);
    STATIC_REQUIRE_FALSE(Boolean_param<P>);
    STATIC_REQUIRE(Param<P>);

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

    STATIC_REQUIRE(P::long_name == "--long-name");
    STATIC_REQUIRE(P::description == "The parameter descritption.");
    STATIC_REQUIRE(P::parse_value("123") == "123");
    STATIC_REQUIRE(std::same_as<Param_value_t<P>, std::string_view>);
}

TEST_CASE("Mandatory regular param (i.e. has no default value) may have both long&short names")
{
    using P = ez::cli::Regular_param<"-s", "--long-name",
        "The parameter descritption.",
        [](auto arg) {
            return arg;
        }
    >;

    STATIC_REQUIRE_FALSE(Positional_param<P>);
    STATIC_REQUIRE(Regular_param<P>);
    STATIC_REQUIRE_FALSE(Boolean_param<P>);
    STATIC_REQUIRE(Param<P>);

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

    STATIC_REQUIRE(P::short_name == "-s");
    STATIC_REQUIRE(P::long_name == "--long-name");
    STATIC_REQUIRE(P::description == "The parameter descritption.");
    STATIC_REQUIRE(P::parse_value("123") == "123");
    STATIC_REQUIRE(std::same_as<Param_value_t<P>, std::string_view>);
}

TEST_CASE("Mandatory regular param may specify allowance for repetition 1")
{
    using P = ez::cli::Regular_param<"-s", "",
        "The parameter descritption.",
        [](std::string_view arg) {
            return std::vector{arg};
        },
        [](auto& values, std::string_view arg) {
            values.push_back(arg);
        }
    >;

    STATIC_REQUIRE_FALSE(Positional_param<P>);
    STATIC_REQUIRE(Regular_param<P>);
    STATIC_REQUIRE_FALSE(Boolean_param<P>);
    STATIC_REQUIRE(Param<P>);

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

    STATIC_REQUIRE(P::short_name == "-s");
    STATIC_REQUIRE(P::description == "The parameter descritption.");
    STATIC_REQUIRE(P::parse_value("123") == std::vector{"123"sv});
    std::vector<std::string_view> vs;
    P::parse_repeated_value(vs, "abc");
    REQUIRE(vs == std::vector{"abc"sv});
    STATIC_REQUIRE(std::same_as<Param_value_t<P>, std::vector<std::string_view>>);
}

TEST_CASE("Mandatory regular param may specify allowance for repetition 2")
{
    using P = ez::cli::Regular_param<"", "--long-name",
        "The parameter descritption.",
        [](std::string_view arg) {
            return std::vector{arg};
        },
        [](auto& values, std::string_view arg) {
            values.push_back(arg);
        }
    >;

    STATIC_REQUIRE_FALSE(Positional_param<P>);
    STATIC_REQUIRE(Regular_param<P>);
    STATIC_REQUIRE_FALSE(Boolean_param<P>);
    STATIC_REQUIRE(Param<P>);

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

    STATIC_REQUIRE(P::short_name == "");
    STATIC_REQUIRE(P::long_name == "--long-name");
    STATIC_REQUIRE(P::description == "The parameter descritption.");
    REQUIRE(P::parse_value("123") == std::vector{"123"sv});
    std::vector<std::string_view> vs;
    P::parse_repeated_value(vs, "abc");
    REQUIRE(vs == std::vector{"abc"sv});
    STATIC_REQUIRE(std::same_as<Param_value_t<P>, std::vector<std::string_view>>);
}

TEST_CASE("Mandatory regular param may specify allowance for repetition 3")
{
    using P = ez::cli::Regular_param<"-s", "--long-name",
        "The parameter descritption.",
        [](std::string_view arg) {
            return std::vector{arg};
        },
        [](auto& values, std::string_view arg) {
            values.push_back(arg);
        }
    >;

    STATIC_REQUIRE_FALSE(Positional_param<P>);
    STATIC_REQUIRE(Regular_param<P>);
    STATIC_REQUIRE_FALSE(Boolean_param<P>);
    STATIC_REQUIRE(Param<P>);

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

    STATIC_REQUIRE(P::short_name == "-s");
    STATIC_REQUIRE(P::long_name == "--long-name");
    STATIC_REQUIRE(P::description == "The parameter descritption.");
    REQUIRE(P::parse_value("123") == std::vector{"123"sv});
    std::vector<std::string_view> vs;
    P::parse_repeated_value(vs, "abc");
    REQUIRE(vs == std::vector{"abc"sv});
    STATIC_REQUIRE(std::same_as<Param_value_t<P>, std::vector<std::string_view>>);
}

TEST_CASE("Optional regular param (i.e. has default value) may have only short name")
{
    using P = ez::cli::Regular_param<"-s", "",
        "The parameter descritption.",
        [](auto arg) {
            return arg;
        },
        [] {
            return "default value"sv;
        }
    >;

    STATIC_REQUIRE_FALSE(Positional_param<P>);
    STATIC_REQUIRE(Regular_param<P>);
    STATIC_REQUIRE_FALSE(Boolean_param<P>);
    STATIC_REQUIRE(Param<P>);

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

    STATIC_REQUIRE(P::short_name == "-s");
    STATIC_REQUIRE(P::description == "The parameter descritption.");
    STATIC_REQUIRE(P::parse_value("123") == "123");
    STATIC_REQUIRE(std::same_as<Param_value_t<P>, std::string_view>);
}

TEST_CASE("Optional regular param (i.e. has default value) may have only long name")
{
    using P = ez::cli::Regular_param<"", "--long-name",
        "The parameter descritption.",
        [](auto arg) {
            return arg;
        },
        [] {
            return "default value"sv;
        }
    >;

    STATIC_REQUIRE_FALSE(Positional_param<P>);
    STATIC_REQUIRE(Regular_param<P>);
    STATIC_REQUIRE_FALSE(Boolean_param<P>);
    STATIC_REQUIRE(Param<P>);

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

    STATIC_REQUIRE(P::long_name == "--long-name");
    STATIC_REQUIRE(P::description == "The parameter descritption.");
    STATIC_REQUIRE(P::parse_value("123") == "123");
    STATIC_REQUIRE(std::same_as<Param_value_t<P>, std::string_view>);
}

TEST_CASE("Optional regular param (i.e. has default value) may have both long&short names")
{
    using P = ez::cli::Regular_param<"-s", "--long-name",
        "The parameter descritption.",
        [](auto arg) {
            return arg;
        },
        [] {
            return "default value"sv;
        }
    >;

    STATIC_REQUIRE_FALSE(Positional_param<P>);
    STATIC_REQUIRE(Regular_param<P>);
    STATIC_REQUIRE_FALSE(Boolean_param<P>);
    STATIC_REQUIRE(Param<P>);

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

    STATIC_REQUIRE(P::short_name == "-s");
    STATIC_REQUIRE(P::long_name == "--long-name");
    STATIC_REQUIRE(P::description == "The parameter descritption.");
    STATIC_REQUIRE(P::parse_value("123") == "123");
    STATIC_REQUIRE(std::same_as<Param_value_t<P>, std::string_view>);
}

TEST_CASE("Optional regular param may specify allowance for repetition 1")
{
    using P = ez::cli::Regular_param<"-s", "",
        "The parameter descritption.",
        [] {
            return std::vector{"default value"sv};
        },
        [](auto arg) {
            return std::vector{arg};
        },
        [](auto& values, std::string_view arg) {
            values.push_back(arg);
        }
    >;

    STATIC_REQUIRE_FALSE(Positional_param<P>);
    STATIC_REQUIRE(Regular_param<P>);
    STATIC_REQUIRE_FALSE(Boolean_param<P>);
    STATIC_REQUIRE(Param<P>);

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

    STATIC_REQUIRE(P::short_name == "-s");
    STATIC_REQUIRE(P::description == "The parameter descritption.");
    REQUIRE(P::parse_value("456") == std::vector{"456"sv});
    std::vector<std::string_view> vs;
    P::parse_repeated_value(vs, "789");
    REQUIRE(vs == std::vector{"789"sv});
    STATIC_REQUIRE(P::default_value() == std::vector{"default value"sv});
    STATIC_REQUIRE(std::same_as<Param_value_t<P>, std::vector<std::string_view>>);

}

TEST_CASE("Optional regular param may specify allowance for repetition 2")
{
    using P = ez::cli::Regular_param<"", "--long-name",
        "The parameter descritption.",
        [] {
            return std::vector{"default value"sv};
        },
        [](auto arg) {
            return std::vector{arg};
        },
        [](auto& values, std::string_view arg) {
            values.push_back(arg);
        }
    >;

    STATIC_REQUIRE_FALSE(Positional_param<P>);
    STATIC_REQUIRE(Regular_param<P>);
    STATIC_REQUIRE_FALSE(Boolean_param<P>);
    STATIC_REQUIRE(Param<P>);

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

    STATIC_REQUIRE(P::long_name == "--long-name");
    STATIC_REQUIRE(P::description == "The parameter descritption.");
    REQUIRE(P::parse_value("456") == std::vector{"456"sv});
    std::vector<std::string_view> vs;
    P::parse_repeated_value(vs, "789");
    REQUIRE(vs == std::vector{"789"sv});
    STATIC_REQUIRE(P::default_value() == std::vector{"default value"sv});
    STATIC_REQUIRE(std::same_as<Param_value_t<P>, std::vector<std::string_view>>);
}

TEST_CASE("Optional regular param may specify allowance for repetition 3")
{
    using P = ez::cli::Regular_param<"-s", "--long-name",
        "The parameter descritption.",
        [] {
            return std::vector{"default value"sv};
        },
        [](auto arg) {
            return std::vector{arg};
        },
        [](auto& values, std::string_view arg) {
            values.push_back(arg);
        }
    >;

    STATIC_REQUIRE_FALSE(Positional_param<P>);
    STATIC_REQUIRE(Regular_param<P>);
    STATIC_REQUIRE_FALSE(Boolean_param<P>);
    STATIC_REQUIRE(Param<P>);

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

    STATIC_REQUIRE(P::short_name == "-s");
    STATIC_REQUIRE(P::long_name == "--long-name");
    STATIC_REQUIRE(P::description == "The parameter descritption.");
    REQUIRE(P::parse_value("456") == std::vector{"456"sv});
    std::vector<std::string_view> vs;
    P::parse_repeated_value(vs, "789");
    REQUIRE(vs == std::vector{"789"sv});
    STATIC_REQUIRE(P::default_value() == std::vector{"default value"sv});
    STATIC_REQUIRE(std::same_as<Param_value_t<P>, std::vector<std::string_view>>);
}

namespace {

namespace details_ = ez::cli::details_;

template<ez::utils::Static_string param_short_name, ez::utils::Static_string param_long_name,
         ez::utils::Static_string param_description,
         auto... f>
struct Test_param :
    details_::Default_value<f...>,
    details_::Value_parser<f...>,
    details_::Repeated_value_parser<details_::Value_parser<f...>, f...> {

    static constexpr auto short_name = param_short_name.up_to_null();
    static constexpr auto long_name = param_long_name.up_to_null();
    static constexpr auto description = param_description.up_to_null();
};

using Incorrect_param_types = std::tuple<
    // Both short and long param names are missing (empty strings).
    Test_param<"", "",
        "The parameter descritption.",
        [](auto arg) { return arg; }
    >,

    // A param description is missing (empty string).
    Test_param<"-s", "",
        "",
        [](auto arg) {
            return arg;
        }
    >,

    // A param description is missing (empty string).
    Test_param<"", "--long-name",
        "",
        [](auto arg) {
            return arg;
        }
    >,

    // An argument value parsing funciton is missing.
    Test_param<"-s", "",
        "The parameter descritption."
    >,

    //==-----------------------------------------------==//
    //  Ditto as above but with default value specified  //
    //==-----------------------------------------------==//

    // Both short and long param names are missing (empty strings).
    Test_param<"", "",
        "The parameter descritption.",
        [](auto arg) {
            return arg;
        },
        [] {
            return "default value"sv;
        }
    >,

    // A param description is missing (empty string).
    Test_param<"-s", "",
        "",
        [](auto arg) {
            return arg;
        },
        [] {
            return "default value"sv;
        }
    >,

    // A param description is missing (empty string).
    Test_param<"", "--long-name",
        "",
        [](auto arg) {
            return arg;
        },
        [] {
            return "default value"sv;
        }
    >,

    // An argument value-parser funciton is missing.
    Test_param<"-s", "",
        "The parameter descritption.",
        [] {
            return "default value"sv;
        }
    >
>;

} // namespace

TEMPLATE_LIST_TEST_CASE("Incorrect regular param spec", "", Incorrect_param_types)
{
    using P = TestType;

    static_assert(!std::is_same_v<P, Incorrect_param_types>,
        "Ensure using TEMPLATE_LIST_TEST_CASE and not TEMPLATE_TEST_CASE.");

    STATIC_REQUIRE_FALSE(Positional_param<P>);
    STATIC_REQUIRE_FALSE(Regular_param<P>);
    STATIC_REQUIRE_FALSE(Boolean_param<P>);
    STATIC_REQUIRE_FALSE(Param<P>);
}
