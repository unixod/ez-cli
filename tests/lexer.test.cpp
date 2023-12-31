#include <catch2/catch_all.hpp>
#include <vector>
#include "ez/cli.h"
#include "ez/c_string_view.h"
#include "ez/static_string.h" // temporarly

namespace cli = ez::cli;

// An example:
/*

    namespace my::cli_param {

    using Help = ez::cli::Param<"-h", "--help">;
    using File = ez::cli::Param<std::string, "-f", "--file">;
    using Verbose = ez::cli::Param<"-v">;

    } // namesapce cli

    namespace my {

    using Cli_error = ...;
    using Cli_help = ez::Cli<cli_param::Help, ez::cil::Ignore_rest_of_args>;
    using Cli_regular = ez::Cli<cli_param::File, cli_param::Verbose, ez::cli::Throw_error_if_any<Cli_error>>;
    // using Cli_regular = ez::Cli<cli::File, cli::Verbose, ez::cli::Return_error_if_any<Cli_error>>;

    using Cli = ez::Cli<Cli_help, Cli_regular>;

    }

    int main_(my::Cli_help help) {
        std::cout << Cli::help();
    }

    int main_(my::Cli_error error) {

    }

    int main_(my::Cli_regular cli) {
        std::fstream in{cli.get<my::cli_param::File>()};
        ...
    }

    int main(int argc, char** argv)
    {
        return std::visit([](auto cli){ main_(cli); }, my::Cli::parse(argc, argv));
        // return my::Cli::parse_and_visit(argc, argv, [](auto cli){ main_(cli); });
    }
*/



namespace details_ = ez::cli::details_;

TEST_CASE("Parameter set is empty")
{
    auto is_compiled = []<typename... T>(T...) {
        return requires { details_::tokenize<T...>({}); };
    };

    REQUIRE_FALSE(is_compiled());
}

// Bool_parameter -----

TEST_CASE("Parameter set consists of single item satisfying Bool_parameter")
{
//    SECTION("Args set is empty => no tokens on out") {
//        REQUIRE(std::ranges::distance(details_::tokenize<A_flag_so_nd>({})) == 0);
//    }

    SECTION("Args set isn't empty") {
        // args set contains both recognizable and non-recognizable args
//        using A = ez::cli::Param<int, "-a", "--abc",
//            "Count",
//            [/*default_value()*/]() {
//                return "~/hello.cpp";
//            },
//            [/*value()*/](std::true_type) {
//                return "asdf";
//            },
//            [/*value parser*/](std::string_view) {
//                return 3;
//            },
//            [/*append value*/](int& v, std::string_view /*sv*/){
//                return v;
//            }
//        >;

//        static_assert(sizeof(A) != 0);
//        static_assert(ez::cli::Bool_parameter<A>);
//        details_::tokenize<A, B>({});
    }
}

TEST_CASE("Parameter set consists of several items satisfying Bool_parameter")
{

}

// Regular_parameter -----

TEST_CASE("Parameter set consists of single item satisfying Regular_parameter")
{

}

TEST_CASE("Parameter set consists of several items satisfying Regular_parameter")
{

}

// Positional_parameter -----

TEST_CASE("Parameter set consists of single item satisfying Positional_parameter")
{

}

TEST_CASE("Parameter set consists of several items satisfying Positional_parameters")
{

}

//TEST_CASE("get_tokens")
//{
//    constexpr auto arr = ez::cli::details_::get_tokens<A_flag_so_wd, A_flag_sl_wd>();
//    REQUIRE(arr.size() == 3);
//    REQUIRE(arr[0].parameter_index == 0);
//    REQUIRE(arr[1].parameter_index == 1);
//    REQUIRE(arr[2].parameter_index == 1);

//    REQUIRE(arr[0].lexeme_begin == A_flag_so_wd::short_name);
//    REQUIRE(arr[1].lexeme_begin == A_flag_sl_wd::short_name);
//    REQUIRE(arr[2].lexeme_begin == A_flag_sl_wd::long_name);
//}

//TEST_CASE("Lexer")
//{
//    using Lexer = ez::cli::details_::Lexer<A_flag_so_wd, A_flag_sl_wd>;

//    REQUIRE(Lexer::recognize("").first == -1);
//    REQUIRE(Lexer::recognize("-a").first == -1);
//    REQUIRE(Lexer::recognize("--c").first == -1);
//    REQUIRE(Lexer::recognize("--d").first == -1);
//    REQUIRE(Lexer::recognize("-cd").first == -1);
//    REQUIRE(Lexer::recognize("-dd").first == -1);
//    REQUIRE(Lexer::recognize("-d-parameter").first == -1);
//    REQUIRE(Lexer::recognize("--d-parameterk").first == -1);
//    REQUIRE(Lexer::recognize("--d-parameterk=12").first == -1);

//    REQUIRE(Lexer::recognize("-c").first == 0);
//    REQUIRE(Lexer::recognize("-d").first == 1);
//    REQUIRE(Lexer::recognize("--d-parameter").first == 1);
//    REQUIRE(Lexer::recognize("--d-parameter=12").first == 1);

//    auto str0 = "-d";
//    REQUIRE(Lexer::recognize(str0).second == str0+2);

//    auto str1 = "--d-parameter";
//    REQUIRE(Lexer::recognize(str1).second == str1+13);

//    auto str2 = "--d-parameter=value";
//    REQUIRE(Lexer::recognize(str2).second == str2+13);
//}

TEST_CASE("tokenize")
{
//    auto args = std::array{
//        "-a", "-b"
//    };

    // TODO: Cover cases:
    // - 0 number of named parameters
    // - 0 number of positional parameters
    // - 0 number of parameters or any kind
//    for (auto tok : cli::details_::tokenize<A_flag_so_wd, A_flag_sl_wd>(args)) {
//        tok.index();
//    }

}


TEST_CASE("Cli")
{
//    using vec = std::vector<const char*>;

//    auto argv = GENERATE(
//        vec{},
//        vec{"<app>"},
//        vec{"<app>", "a/b/c", "d/e/f"},

//        vec{"<app>", "-e"},
//        vec{"<app>", "-e", "a/b/c"},
//        vec{"<app>", "a/b/c", "-e"},
//        vec{"<app>", "a/b/c", "d/e/f", "-e"},

//        vec{"<app>", "-e" "a/b/c", "d/e/f", "g/h/i"},
//        vec{"<app>", "a/b/c", "-e", "d/e/f", "g/h/i"},
//        vec{"<app>", "a/b/c", "d/e/f", "-e", "g/h/i"},

//        vec{"<app>", "a/b/c", "d/e/f", "-e="},
//        vec{"<app>", "a/b/c", "d/e/f", "-e=asdf"}
//    );

//    argv.push_back(nullptr);

//    CAPTURE(argv);
//    REQUIRE_FALSE(argv.empty());

//    static_assert(ez::cli::details_::Has_append_value<A_named_param_so_nd_wa>);
//    [[maybe_unused]]
//    auto aaa = std::array{
//        "app-name", "-c", static_cast<const char*>(nullptr)
//    };

//    ez::Cli<
//        A_flag_so_wd/*, A_flag_sl_wd,
//        A_named_param_so_nd_wa, A_named_param_so_nd_na*/> asdf{aaa.size() - 1, aaa.data()};
//    asdf.get<A_flag_so_wd>();
//    REQUIRE(asdf.get<A_flag_so_wd>() == 123);

//    using Compile = ez::P<int, "-c", "--compile">;
//    using Opt_lvl = ez::P<Custom_type, "-o", "--optimize", 7>; // Custom_type should have trait specializaiton



//    auto kk0 = asdf;
//    auto kk = std::move(asdf);




    // flag: -a, arg: -a=23 -> exception
    // nparam: -a, arg -a=23 -> ok

//    using Vertices_cnt = ez::cli::Positional_parameter<int, "Some description">;
//    using Max_out_degree = ez::cli::Positional_parameter<int, "Some other description">;

//    auto cli = ez::Cli<Vertices_cnt, Max_out_degree>{"rgg", argv, argc};

//    if (cli.get<Vertices_cnt>()) {
//    }

//    use = cli.get<Max_out_degree>();


//    [[maybe_unused]]
//    ez::cli::details_::Parameter_value<A_flag_so_wd> d;

//    [[maybe_unused]]
//    details_::Lexer<std::tuple<>> sd;
//    static_assert(Static_string("hello") == Static_string("hello"));
}


