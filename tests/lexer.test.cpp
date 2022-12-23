#include <catch2/catch_all.hpp>
#include <vector>

#include <iostream>
#include "ez/cli.h"
#include "ez/utils/match.h"
#include "ez/c_string_view.h"
#include "ez/utils/badge.h"
#include "ez/cli/details_/uninitialized.h"
#include "ez/static_string.h" // temporarly

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

struct Append_value {
    static void append_value(auto&, std::string_view /*v*/) noexcept
    {}
};


class Very_restricted_type {
public:
    enum class Foo {};

public:
    Very_restricted_type(Foo) {}

    Very_restricted_type() = delete;
    Very_restricted_type(Very_restricted_type&&) = delete;
    Very_restricted_type(const Very_restricted_type&) = delete;
    Very_restricted_type& operator = (Very_restricted_type&&) = delete;
    Very_restricted_type& operator = (const Very_restricted_type&) = delete;
    ~Very_restricted_type() = default;
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

struct A_named_param_so_nd_na : Short_name<"-i">, Value_parser {};
struct A_named_param_so_nd_wa : Short_name<"-j">, Value_parser, Append_value {};

struct A_positional_param : Value_parser {};

} // namespace


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

TEST_CASE("Flag")
{
    STATIC_REQUIRE(cli::details_::Has_short_name<A_flag_so_nd>);
    STATIC_REQUIRE_FALSE(cli::details_::Has_long_name<A_flag_so_nd>);
    STATIC_REQUIRE(cli::details_::Has_value_constructor<A_flag_so_nd>);
    STATIC_REQUIRE_FALSE(cli::details_::Has_value_parser<A_flag_so_nd>);
    STATIC_REQUIRE_FALSE(cli::details_::Has_default_value<A_flag_so_nd>);
    STATIC_REQUIRE(cli::Named_parameter_without_value<A_flag_so_nd>);
    STATIC_REQUIRE_FALSE(cli::Named_parameter_with_value<A_flag_so_nd>);
    STATIC_REQUIRE_FALSE(cli::Positional_parameter<A_flag_so_nd>);
    STATIC_REQUIRE(cli::Parameter<A_flag_so_nd>);

    STATIC_REQUIRE_FALSE(cli::details_::Has_short_name<A_flag_lo_nd>);
    STATIC_REQUIRE(cli::details_::Has_long_name<A_flag_lo_nd>);
    STATIC_REQUIRE(cli::details_::Has_value_constructor<A_flag_lo_nd>);
    STATIC_REQUIRE_FALSE(cli::details_::Has_value_parser<A_flag_lo_nd>);
    STATIC_REQUIRE_FALSE(cli::details_::Has_default_value<A_flag_lo_nd>);
    STATIC_REQUIRE(cli::Named_parameter_without_value<A_flag_lo_nd>);
    STATIC_REQUIRE_FALSE(cli::Named_parameter_with_value<A_flag_lo_nd>);
    STATIC_REQUIRE_FALSE(cli::Positional_parameter<A_flag_lo_nd>);
    STATIC_REQUIRE(cli::Parameter<A_flag_lo_nd>);

    STATIC_REQUIRE(cli::details_::Has_short_name<A_flag_sl_nd>);
    STATIC_REQUIRE(cli::details_::Has_long_name<A_flag_sl_nd>);
    STATIC_REQUIRE(cli::details_::Has_value_constructor<A_flag_sl_nd>);
    STATIC_REQUIRE_FALSE(cli::details_::Has_value_parser<A_flag_sl_nd>);
    STATIC_REQUIRE_FALSE(cli::details_::Has_default_value<A_flag_sl_nd>);
    STATIC_REQUIRE(cli::Named_parameter_without_value<A_flag_sl_nd>);
    STATIC_REQUIRE_FALSE(cli::Named_parameter_with_value<A_flag_sl_nd>);
    STATIC_REQUIRE_FALSE(cli::Positional_parameter<A_flag_sl_nd>);
    STATIC_REQUIRE(cli::Parameter<A_flag_sl_nd>);

    STATIC_REQUIRE(cli::details_::Has_short_name<A_flag_so_wd>);
    STATIC_REQUIRE_FALSE(cli::details_::Has_long_name<A_flag_so_wd>);
    STATIC_REQUIRE(cli::details_::Has_value_constructor<A_flag_so_wd>);
    STATIC_REQUIRE_FALSE(cli::details_::Has_value_parser<A_flag_so_wd>);
    STATIC_REQUIRE(cli::details_::Has_default_value<A_flag_so_wd>);
    STATIC_REQUIRE(cli::Named_parameter_without_value<A_flag_so_wd>);
    STATIC_REQUIRE_FALSE(cli::Named_parameter_with_value<A_flag_so_wd>);
    STATIC_REQUIRE_FALSE(cli::Positional_parameter<A_flag_so_wd>);
    STATIC_REQUIRE(cli::Parameter<A_flag_so_wd>);

    STATIC_REQUIRE_FALSE(cli::details_::Has_short_name<A_flag_lo_wd>);
    STATIC_REQUIRE(cli::details_::Has_long_name<A_flag_lo_wd>);
    STATIC_REQUIRE(cli::details_::Has_value_constructor<A_flag_lo_wd>);
    STATIC_REQUIRE_FALSE(cli::details_::Has_value_parser<A_flag_lo_wd>);
    STATIC_REQUIRE(cli::details_::Has_default_value<A_flag_lo_wd>);
    STATIC_REQUIRE(cli::Named_parameter_without_value<A_flag_lo_wd>);
    STATIC_REQUIRE_FALSE(cli::Named_parameter_with_value<A_flag_lo_wd>);
    STATIC_REQUIRE_FALSE(cli::Positional_parameter<A_flag_lo_wd>);
    STATIC_REQUIRE(cli::Parameter<A_flag_lo_wd>);

    STATIC_REQUIRE(cli::details_::Has_short_name<A_flag_sl_wd>);
    STATIC_REQUIRE(cli::details_::Has_long_name<A_flag_sl_wd>);
    STATIC_REQUIRE(cli::details_::Has_value_constructor<A_flag_sl_wd>);
    STATIC_REQUIRE_FALSE(cli::details_::Has_value_parser<A_flag_sl_wd>);
    STATIC_REQUIRE(cli::details_::Has_default_value<A_flag_sl_wd>);
    STATIC_REQUIRE(cli::Named_parameter_without_value<A_flag_sl_wd>);
    STATIC_REQUIRE_FALSE(cli::Named_parameter_with_value<A_flag_sl_wd>);
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
    STATIC_REQUIRE(cli::Named_parameter_with_value<A_named_param_so_nd>);
    STATIC_REQUIRE_FALSE(cli::Named_parameter_without_value<A_named_param_so_nd>);
    STATIC_REQUIRE_FALSE(cli::Positional_parameter<A_named_param_so_nd>);
    STATIC_REQUIRE(cli::Parameter<A_named_param_so_nd>);

    STATIC_REQUIRE_FALSE(cli::details_::Has_short_name<A_named_param_lo_nd>);
    STATIC_REQUIRE(cli::details_::Has_long_name<A_named_param_lo_nd>);
    STATIC_REQUIRE_FALSE(cli::details_::Has_value_constructor<A_named_param_lo_nd>);
    STATIC_REQUIRE(cli::details_::Has_value_parser<A_named_param_lo_nd>);
    STATIC_REQUIRE_FALSE(cli::details_::Has_default_value<A_named_param_lo_nd>);
    STATIC_REQUIRE(cli::Named_parameter_with_value<A_named_param_lo_nd>);
    STATIC_REQUIRE_FALSE(cli::Named_parameter_without_value<A_named_param_lo_nd>);
    STATIC_REQUIRE_FALSE(cli::Positional_parameter<A_named_param_lo_nd>);
    STATIC_REQUIRE(cli::Parameter<A_named_param_lo_nd>);

    STATIC_REQUIRE(cli::details_::Has_short_name<A_named_param_sl_nd>);
    STATIC_REQUIRE(cli::details_::Has_long_name<A_named_param_sl_nd>);
    STATIC_REQUIRE_FALSE(cli::details_::Has_value_constructor<A_named_param_sl_nd>);
    STATIC_REQUIRE(cli::details_::Has_value_parser<A_named_param_sl_nd>);
    STATIC_REQUIRE_FALSE(cli::details_::Has_default_value<A_named_param_sl_nd>);
    STATIC_REQUIRE(cli::Named_parameter_with_value<A_named_param_sl_nd>);
    STATIC_REQUIRE_FALSE(cli::Named_parameter_without_value<A_named_param_sl_nd>);
    STATIC_REQUIRE_FALSE(cli::Positional_parameter<A_named_param_sl_nd>);
    STATIC_REQUIRE(cli::Parameter<A_named_param_sl_nd>);

    STATIC_REQUIRE(cli::details_::Has_short_name<A_named_param_so_wd>);
    STATIC_REQUIRE_FALSE(cli::details_::Has_long_name<A_named_param_so_wd>);
    STATIC_REQUIRE_FALSE(cli::details_::Has_value_constructor<A_named_param_so_wd>);
    STATIC_REQUIRE(cli::details_::Has_value_parser<A_named_param_so_wd>);
    STATIC_REQUIRE(cli::details_::Has_default_value<A_named_param_so_wd>);
    STATIC_REQUIRE(cli::Named_parameter_with_value<A_named_param_so_wd>);
    STATIC_REQUIRE_FALSE(cli::Named_parameter_without_value<A_named_param_so_wd>);
    STATIC_REQUIRE_FALSE(cli::Positional_parameter<A_named_param_so_wd>);
    STATIC_REQUIRE(cli::Parameter<A_named_param_so_wd>);

    STATIC_REQUIRE_FALSE(cli::details_::Has_short_name<A_named_param_lo_wd>);
    STATIC_REQUIRE(cli::details_::Has_long_name<A_named_param_lo_wd>);
    STATIC_REQUIRE_FALSE(cli::details_::Has_value_constructor<A_named_param_lo_wd>);
    STATIC_REQUIRE(cli::details_::Has_value_parser<A_named_param_lo_wd>);
    STATIC_REQUIRE(cli::details_::Has_default_value<A_named_param_lo_wd>);
    STATIC_REQUIRE(cli::Named_parameter_with_value<A_named_param_lo_wd>);
    STATIC_REQUIRE_FALSE(cli::Named_parameter_without_value<A_named_param_lo_wd>);
    STATIC_REQUIRE_FALSE(cli::Positional_parameter<A_named_param_lo_wd>);
    STATIC_REQUIRE(cli::Parameter<A_named_param_lo_wd>);

    STATIC_REQUIRE(cli::details_::Has_short_name<A_named_param_sl_wd>);
    STATIC_REQUIRE(cli::details_::Has_long_name<A_named_param_sl_wd>);
    STATIC_REQUIRE_FALSE(cli::details_::Has_value_constructor<A_named_param_sl_wd>);
    STATIC_REQUIRE(cli::details_::Has_value_parser<A_named_param_sl_wd>);
    STATIC_REQUIRE(cli::details_::Has_default_value<A_named_param_sl_wd>);
    STATIC_REQUIRE(cli::Named_parameter_with_value<A_named_param_sl_wd>);
    STATIC_REQUIRE_FALSE(cli::Named_parameter_without_value<A_named_param_sl_wd>);
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
    STATIC_REQUIRE_FALSE(cli::Named_parameter_with_value<A_positional_param>);
    STATIC_REQUIRE_FALSE(cli::Named_parameter_without_value<A_positional_param>);
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
    auto args = std::array{
        "-a", "-b"
    };

    // TODO: Cover cases:
    // - 0 number of named parameters
    // - 0 number of positional parameters
    // - 0 number of parameters or any kind
    for (auto tok : cli::details_::tokenize<A_flag_so_wd, A_flag_sl_wd>(args)) {
        tok.index();
    }

}

TEST_CASE("asdf") {
    const char* p = "--\0d";
    using namespace std::string_view_literals;
    REQUIRE(p == "--"sv);
}


TEST_CASE("C_string_view")
{
    using namespace std::string_view_literals;
    constexpr ez::utils::C_string_view k{"asfd"};
    std::string str = "asfd";
    STATIC_REQUIRE(std::contiguous_iterator<ez::utils::C_string_view::Iterator>);
    STATIC_REQUIRE(std::ranges::forward_range<ez::utils::C_string_view>);
    REQUIRE(std::ranges::equal(k, str));
    constexpr ez::utils::C_string_view zk{"asfd"};
    constexpr ez::utils::C_string_view zk2{"asf"};
    static_assert(zk > zk2);
    REQUIRE(k < ez::utils::C_string_view{"zz"});
    REQUIRE(k <= ez::utils::C_string_view{"zz"});
    REQUIRE(k >= ez::utils::C_string_view{"as"});
    REQUIRE(k == ez::utils::C_string_view{"asfd"});
    REQUIRE(!std::ranges::empty(k));
    REQUIRE(!k.empty());
    REQUIRE(std::ranges::empty(ez::utils::C_string_view{""}));
    REQUIRE(std::ranges::distance(ez::utils::C_string_view{"asdf"}) == 4);
    REQUIRE(std::ranges::distance(ez::utils::C_string_view{""}) == 0);
    STATIC_REQUIRE(std::ranges::borrowed_range<ez::utils::C_string_view>);
    STATIC_REQUIRE(std::ranges::range<ez::utils::C_string_view>);
    STATIC_REQUIRE(std::ranges::range<const ez::utils::C_string_view>);
}


TEST_CASE("Uninitialized")
{
    struct Some {
        Some(int k) : i{k} {}
        int i;
    };

    ez::cli::details_::Uninitialized<Some> v;
    auto dtr = v.construct(3);
    REQUIRE(v.get().i == 3);
    auto k = std::move(dtr);
    auto don = ez::cli::details_::Detached_destructor{};
    don = std::move(k);
}

TEST_CASE("tuple_cat_view")
{
    SECTION("concat with xvalue tuple of all refs") {
        int i = 3;
        std::tuple<int&> k{i};
        [[maybe_unused]]
        auto ttt = ez::cli::details_::tuple_cat_view(std::move(k));
        REQUIRE(&std::get<0>(ttt) == &i);
    }

    SECTION("concat with lvalue tuple without refs") {
        int i = 3;
        std::tuple<int> k{i};
        [[maybe_unused]]
        auto ttt = ez::cli::details_::tuple_cat_view(k);
        //    auto ttt = ez::cli::details_::tuple_cat_view(std::move(k)); // Shouldn't compile
        REQUIRE(&std::get<0>(ttt) == &std::get<0>(k));
    }

    SECTION("mix concat lvalue, xvalue tuples") {
        int i = 3;
        int k = -23;
        std::tuple<int> a{i};
        std::tuple<int&> b{k};
        [[maybe_unused]]
        auto ttt = ez::cli::details_::tuple_cat_view(a, b, std::move(b));
        //    auto ttt = ez::cli::details_::tuple_cat_view(std::move(k)); // Shouldn't compile
        REQUIRE(ttt == std::tie(i, k, k));
        REQUIRE(&std::get<0>(ttt) == &std::get<0>(a));
        REQUIRE(&std::get<1>(ttt) == &k);
        REQUIRE(&std::get<2>(ttt) == &k);
    }
}

TEST_CASE("tuple_zip")
{
    char i = 'd';
    std::tuple<int, char&> x{3, i};
    std::tuple<double, float> y;
    auto tt = ez::cli::details_::tuple_zip(x,y);
    STATIC_REQUIRE(std::is_same_v<decltype(tt), std::tuple<std::tuple<int, double>, std::tuple<char&, float>>>);
    REQUIRE(std::get<0>(std::get<0>(tt)) == 3);
    REQUIRE(&std::get<0>(std::get<1>(tt)) == &i);
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


