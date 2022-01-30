#include <catch2/catch_all.hpp>
#include <vector>

#include "ez/cli.h"
#include "ez/utils/match.h"

namespace cli = ez::cli;

namespace {

template<ez::utils::Static_string str>
struct Short_name {
    inline static auto& short_name = str.value;
};

template<ez::utils::Static_string str>
struct Long_name {
    inline static auto& long_name = str.value;
};

template<auto return_value>
struct Flag_value  {
    static auto value() noexcept
    {
        return return_value;
    }
};

template<auto return_value>
struct Default_value  {
    static auto default_value() noexcept
    {
        return return_value;
    }
};

struct Value_parser {
    static auto value(std::string_view v) noexcept
    {
        return std::string{v};
    }
};

// so - has short name only
// lo - has long name only
// sl - has short and long names
// nd - has no default value
// wd - has default value
struct A_flag_so_nd : Short_name<"-a">, Flag_value<123> {};
struct A_flag_lo_nd : Long_name<"--a-parameter">, Flag_value<123> {};
struct A_flag_sl_nd : Short_name<"-b">, Long_name<"--b-parameter">, Flag_value<123> {};
struct A_flag_so_wd : Short_name<"-c">, Flag_value<123>, Default_value<-123> {};
struct A_flag_lo_wd : Long_name<"--c-parameter">, Flag_value<123>, Default_value<-123> {};
struct A_flag_sl_wd : Short_name<"-d">, Long_name<"--d-parameter">, Flag_value<123>, Default_value<-123> {};

struct A_named_param_so_nd : Short_name<"-e">, Value_parser {};
struct A_named_param_lo_nd : Long_name<"--e-parameter">, Value_parser {};
struct A_named_param_sl_nd : Short_name<"-f">, Long_name<"--f-parameter">, Value_parser {};
struct A_named_param_so_wd : Short_name<"-g">, Value_parser, Default_value<-123> {};
struct A_named_param_lo_wd : Long_name<"--g-parameter">, Value_parser, Default_value<-123> {};
struct A_named_param_sl_wd : Short_name<"-h">, Long_name<"--h-parameter">, Value_parser, Default_value<-123> {};

struct A_positional_param : Value_parser {};

} // namespace


TEST_CASE("Flag")
{
    STATIC_REQUIRE(cli::details_::Has_short_name<A_flag_so_nd>);
    STATIC_REQUIRE_FALSE(cli::details_::Has_long_name<A_flag_so_nd>);
    STATIC_REQUIRE(cli::details_::Has_value_constructor<A_flag_so_nd>);
    STATIC_REQUIRE_FALSE(cli::details_::Has_value_parser<A_flag_so_nd>);
    STATIC_REQUIRE_FALSE(cli::details_::Has_default_value<A_flag_so_nd>);
    STATIC_REQUIRE(cli::Flag<A_flag_so_nd>);
    STATIC_REQUIRE_FALSE(cli::Named_parameter<A_flag_so_nd>);
    STATIC_REQUIRE_FALSE(cli::Positional_parameter<A_flag_so_nd>);
    STATIC_REQUIRE(cli::Parameter<A_flag_so_nd>);

    STATIC_REQUIRE_FALSE(cli::details_::Has_short_name<A_flag_lo_nd>);
    STATIC_REQUIRE(cli::details_::Has_long_name<A_flag_lo_nd>);
    STATIC_REQUIRE(cli::details_::Has_value_constructor<A_flag_lo_nd>);
    STATIC_REQUIRE_FALSE(cli::details_::Has_value_parser<A_flag_lo_nd>);
    STATIC_REQUIRE_FALSE(cli::details_::Has_default_value<A_flag_lo_nd>);
    STATIC_REQUIRE(cli::Flag<A_flag_lo_nd>);
    STATIC_REQUIRE_FALSE(cli::Named_parameter<A_flag_lo_nd>);
    STATIC_REQUIRE_FALSE(cli::Positional_parameter<A_flag_lo_nd>);
    STATIC_REQUIRE(cli::Parameter<A_flag_lo_nd>);

    STATIC_REQUIRE(cli::details_::Has_short_name<A_flag_sl_nd>);
    STATIC_REQUIRE(cli::details_::Has_long_name<A_flag_sl_nd>);
    STATIC_REQUIRE(cli::details_::Has_value_constructor<A_flag_sl_nd>);
    STATIC_REQUIRE_FALSE(cli::details_::Has_value_parser<A_flag_sl_nd>);
    STATIC_REQUIRE_FALSE(cli::details_::Has_default_value<A_flag_sl_nd>);
    STATIC_REQUIRE(cli::Flag<A_flag_sl_nd>);
    STATIC_REQUIRE_FALSE(cli::Named_parameter<A_flag_sl_nd>);
    STATIC_REQUIRE_FALSE(cli::Positional_parameter<A_flag_sl_nd>);
    STATIC_REQUIRE(cli::Parameter<A_flag_sl_nd>);

    STATIC_REQUIRE(cli::details_::Has_short_name<A_flag_so_wd>);
    STATIC_REQUIRE_FALSE(cli::details_::Has_long_name<A_flag_so_wd>);
    STATIC_REQUIRE(cli::details_::Has_value_constructor<A_flag_so_wd>);
    STATIC_REQUIRE_FALSE(cli::details_::Has_value_parser<A_flag_so_wd>);
    STATIC_REQUIRE(cli::details_::Has_default_value<A_flag_so_wd>);
    STATIC_REQUIRE(cli::Flag<A_flag_so_wd>);
    STATIC_REQUIRE_FALSE(cli::Named_parameter<A_flag_so_wd>);
    STATIC_REQUIRE_FALSE(cli::Positional_parameter<A_flag_so_wd>);
    STATIC_REQUIRE(cli::Parameter<A_flag_so_wd>);

    STATIC_REQUIRE_FALSE(cli::details_::Has_short_name<A_flag_lo_wd>);
    STATIC_REQUIRE(cli::details_::Has_long_name<A_flag_lo_wd>);
    STATIC_REQUIRE(cli::details_::Has_value_constructor<A_flag_lo_wd>);
    STATIC_REQUIRE_FALSE(cli::details_::Has_value_parser<A_flag_lo_wd>);
    STATIC_REQUIRE(cli::details_::Has_default_value<A_flag_lo_wd>);
    STATIC_REQUIRE(cli::Flag<A_flag_lo_wd>);
    STATIC_REQUIRE_FALSE(cli::Named_parameter<A_flag_lo_wd>);
    STATIC_REQUIRE_FALSE(cli::Positional_parameter<A_flag_lo_wd>);
    STATIC_REQUIRE(cli::Parameter<A_flag_lo_wd>);

    STATIC_REQUIRE(cli::details_::Has_short_name<A_flag_sl_wd>);
    STATIC_REQUIRE(cli::details_::Has_long_name<A_flag_sl_wd>);
    STATIC_REQUIRE(cli::details_::Has_value_constructor<A_flag_sl_wd>);
    STATIC_REQUIRE_FALSE(cli::details_::Has_value_parser<A_flag_sl_wd>);
    STATIC_REQUIRE(cli::details_::Has_default_value<A_flag_sl_wd>);
    STATIC_REQUIRE(cli::Flag<A_flag_sl_wd>);
    STATIC_REQUIRE_FALSE(cli::Named_parameter<A_flag_sl_wd>);
    STATIC_REQUIRE_FALSE(cli::Positional_parameter<A_flag_sl_wd>);
    STATIC_REQUIRE(cli::Parameter<A_flag_sl_wd>);
}

TEST_CASE("Named parameter")
{
    STATIC_REQUIRE(cli::details_::Has_short_name<A_named_param_so_nd>);
    STATIC_REQUIRE_FALSE(cli::details_::Has_long_name<A_named_param_so_nd>);
    STATIC_REQUIRE_FALSE(cli::details_::Has_value_constructor<A_named_param_so_nd>);
    STATIC_REQUIRE(cli::details_::Has_value_parser<A_named_param_so_nd>);
    STATIC_REQUIRE_FALSE(cli::details_::Has_default_value<A_named_param_so_nd>);
    STATIC_REQUIRE(cli::Named_parameter<A_named_param_so_nd>);
    STATIC_REQUIRE_FALSE(cli::Flag<A_named_param_so_nd>);
    STATIC_REQUIRE_FALSE(cli::Positional_parameter<A_named_param_so_nd>);
    STATIC_REQUIRE(cli::Parameter<A_named_param_so_nd>);

    STATIC_REQUIRE_FALSE(cli::details_::Has_short_name<A_named_param_lo_nd>);
    STATIC_REQUIRE(cli::details_::Has_long_name<A_named_param_lo_nd>);
    STATIC_REQUIRE_FALSE(cli::details_::Has_value_constructor<A_named_param_lo_nd>);
    STATIC_REQUIRE(cli::details_::Has_value_parser<A_named_param_lo_nd>);
    STATIC_REQUIRE_FALSE(cli::details_::Has_default_value<A_named_param_lo_nd>);
    STATIC_REQUIRE(cli::Named_parameter<A_named_param_lo_nd>);
    STATIC_REQUIRE_FALSE(cli::Flag<A_named_param_lo_nd>);
    STATIC_REQUIRE_FALSE(cli::Positional_parameter<A_named_param_lo_nd>);
    STATIC_REQUIRE(cli::Parameter<A_named_param_lo_nd>);

    STATIC_REQUIRE(cli::details_::Has_short_name<A_named_param_sl_nd>);
    STATIC_REQUIRE(cli::details_::Has_long_name<A_named_param_sl_nd>);
    STATIC_REQUIRE_FALSE(cli::details_::Has_value_constructor<A_named_param_sl_nd>);
    STATIC_REQUIRE(cli::details_::Has_value_parser<A_named_param_sl_nd>);
    STATIC_REQUIRE_FALSE(cli::details_::Has_default_value<A_named_param_sl_nd>);
    STATIC_REQUIRE(cli::Named_parameter<A_named_param_sl_nd>);
    STATIC_REQUIRE_FALSE(cli::Flag<A_named_param_sl_nd>);
    STATIC_REQUIRE_FALSE(cli::Positional_parameter<A_named_param_sl_nd>);
    STATIC_REQUIRE(cli::Parameter<A_named_param_sl_nd>);

    STATIC_REQUIRE(cli::details_::Has_short_name<A_named_param_so_wd>);
    STATIC_REQUIRE_FALSE(cli::details_::Has_long_name<A_named_param_so_wd>);
    STATIC_REQUIRE_FALSE(cli::details_::Has_value_constructor<A_named_param_so_wd>);
    STATIC_REQUIRE(cli::details_::Has_value_parser<A_named_param_so_wd>);
    STATIC_REQUIRE(cli::details_::Has_default_value<A_named_param_so_wd>);
    STATIC_REQUIRE(cli::Named_parameter<A_named_param_so_wd>);
    STATIC_REQUIRE_FALSE(cli::Flag<A_named_param_so_wd>);
    STATIC_REQUIRE_FALSE(cli::Positional_parameter<A_named_param_so_wd>);
    STATIC_REQUIRE(cli::Parameter<A_named_param_so_wd>);

    STATIC_REQUIRE_FALSE(cli::details_::Has_short_name<A_named_param_lo_wd>);
    STATIC_REQUIRE(cli::details_::Has_long_name<A_named_param_lo_wd>);
    STATIC_REQUIRE_FALSE(cli::details_::Has_value_constructor<A_named_param_lo_wd>);
    STATIC_REQUIRE(cli::details_::Has_value_parser<A_named_param_lo_wd>);
    STATIC_REQUIRE(cli::details_::Has_default_value<A_named_param_lo_wd>);
    STATIC_REQUIRE(cli::Named_parameter<A_named_param_lo_wd>);
    STATIC_REQUIRE_FALSE(cli::Flag<A_named_param_lo_wd>);
    STATIC_REQUIRE_FALSE(cli::Positional_parameter<A_named_param_lo_wd>);
    STATIC_REQUIRE(cli::Parameter<A_named_param_lo_wd>);

    STATIC_REQUIRE(cli::details_::Has_short_name<A_named_param_sl_wd>);
    STATIC_REQUIRE(cli::details_::Has_long_name<A_named_param_sl_wd>);
    STATIC_REQUIRE_FALSE(cli::details_::Has_value_constructor<A_named_param_sl_wd>);
    STATIC_REQUIRE(cli::details_::Has_value_parser<A_named_param_sl_wd>);
    STATIC_REQUIRE(cli::details_::Has_default_value<A_named_param_sl_wd>);
    STATIC_REQUIRE(cli::Named_parameter<A_named_param_sl_wd>);
    STATIC_REQUIRE_FALSE(cli::Flag<A_named_param_sl_wd>);
    STATIC_REQUIRE_FALSE(cli::Positional_parameter<A_named_param_sl_wd>);
    STATIC_REQUIRE(cli::Parameter<A_named_param_sl_wd>);
}

TEST_CASE("Positional parameter")
{
    STATIC_REQUIRE_FALSE(cli::details_::Has_short_name<A_positional_param>);
    STATIC_REQUIRE_FALSE(cli::details_::Has_long_name<A_positional_param>);
    STATIC_REQUIRE_FALSE(cli::details_::Has_value_constructor<A_positional_param>);
    STATIC_REQUIRE(cli::details_::Has_value_parser<A_positional_param>);
    STATIC_REQUIRE_FALSE(cli::details_::Has_default_value<A_positional_param>);
    STATIC_REQUIRE_FALSE(cli::Named_parameter<A_positional_param>);
    STATIC_REQUIRE_FALSE(cli::Flag<A_positional_param>);
    STATIC_REQUIRE(cli::Positional_parameter<A_positional_param>);
    STATIC_REQUIRE(cli::Parameter<A_positional_param>);
}


TEST_CASE("apply_to_tuple")
{
    STATIC_REQUIRE(
        std::is_same_v<void, decltype(ez::cli::details_::apply_tuple_element([](auto k){}, 0, std::tuple<>{}))>);

//    auto handle = [](auto&& tuple_element, auto&&... rest_args){
//        std::ostringstream out;
//        out << tuple_element << " ";
//        (out << ... << rest_args);
//        return out.str();
//    };

//    auto tpl = std::tuple{1, 2.23, "some value"};

//    apply_tuple_element(handle, 0, tpl, )
}






TEST_CASE("get_tokens")
{
    constexpr auto arr = ez::cli::details_::get_tokens<A_flag_so_wd, A_flag_sl_wd>();
    REQUIRE(arr.size() == 3);
    REQUIRE(arr[0].parameter_index == 0);
    REQUIRE(arr[1].parameter_index == 1);
    REQUIRE(arr[2].parameter_index == 1);

    REQUIRE(arr[0].lexeme_begin == A_flag_so_wd::short_name);
    REQUIRE(arr[1].lexeme_begin == A_flag_sl_wd::short_name);
    REQUIRE(arr[2].lexeme_begin == A_flag_sl_wd::long_name);
}

TEST_CASE("Lexer")
{
    using Lexer = ez::cli::details_::Lexer<A_flag_so_wd, A_flag_sl_wd>;

    REQUIRE(Lexer::recognize("").first == -1);
    REQUIRE(Lexer::recognize("-a").first == -1);
    REQUIRE(Lexer::recognize("--c").first == -1);
    REQUIRE(Lexer::recognize("--d").first == -1);
    REQUIRE(Lexer::recognize("-cd").first == -1);
    REQUIRE(Lexer::recognize("-dd").first == -1);
    REQUIRE(Lexer::recognize("-d-parameter").first == -1);
    REQUIRE(Lexer::recognize("--d-parameterk").first == -1);
    REQUIRE(Lexer::recognize("--d-parameterk=12").first == -1);

    REQUIRE(Lexer::recognize("-c").first == 0);
    REQUIRE(Lexer::recognize("-d").first == 1);
    REQUIRE(Lexer::recognize("--d-parameter").first == 1);
    REQUIRE(Lexer::recognize("--d-parameter=12").first == 1);

    auto str0 = "-d";
    REQUIRE(Lexer::recognize(str0).second == str0+2);

    auto str1 = "--d-parameter";
    REQUIRE(Lexer::recognize(str1).second == str1+13);

    auto str2 = "--d-parameter=value";
    REQUIRE(Lexer::recognize(str2).second == str2+13);
}

TEST_CASE("Cli")
{
    using vec = std::vector<const char*>;

    auto argv = GENERATE(
        vec{},
        vec{"<app>"},
        vec{"<app>", "a/b/c", "d/e/f"},

        vec{"<app>", "-e"},
        vec{"<app>", "-e", "a/b/c"},
        vec{"<app>", "a/b/c", "-e"},
        vec{"<app>", "a/b/c", "d/e/f", "-e"},

        vec{"<app>", "-e" "a/b/c", "d/e/f", "g/h/i"},
        vec{"<app>", "a/b/c", "-e", "d/e/f", "g/h/i"},
        vec{"<app>", "a/b/c", "d/e/f", "-e", "g/h/i"}
        );

    argv.push_back(nullptr);

    CAPTURE(argv);
    REQUIRE_FALSE(argv.empty());

    [[maybe_unused]]
    ez::Cli<A_flag_so_wd, A_flag_sl_wd> asdf{argv.size() - 1, argv.data()};

//    [[maybe_unused]]
//    ez::cli::details_::Parameter_value<A_flag_so_wd> d;

//    [[maybe_unused]]
//    details_::Lexer<std::tuple<>> sd;
//    static_assert(Static_string("hello") == Static_string("hello"));
}


