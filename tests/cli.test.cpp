#include <catch2/catch_all.hpp>
#include <ez/cli.h>
#include <iostream>

namespace {

template<std::size_t args_cnt>
class Argc_argv {
public:
    using Structured_binding_signature = std::tuple</*argc:*/ int, /*argv*/ const char**>;

    consteval Argc_argv(auto... args) requires ((std::same_as<decltype(args), const char*> && ...))
        : args_{args..., nullptr}
    {}

    template<std::size_t elt_idx, typename A>
        requires std::same_as<std::remove_cvref_t<A>, Argc_argv>
    friend constexpr auto get(A&& a) noexcept {
        if constexpr (elt_idx == 0) {
            return static_cast<int>(args_cnt);
        }
        else if constexpr (elt_idx == 1) {
            return a.args_.data();
        }
        ez::support::std23::unreachable();
    }

private:
    std::array<const char*, args_cnt + 1> args_;
};


Argc_argv(auto... args) -> Argc_argv<sizeof...(args)>;


} // namespace

template<std::size_t args_cnt>
struct std::tuple_size<Argc_argv<args_cnt>>
    : std::tuple_size<typename Argc_argv<args_cnt>::Structured_binding_signature> {};

template<std::size_t elt_idx, std::size_t args_cnt>
struct std::tuple_element<elt_idx, Argc_argv<args_cnt>>
    : std::tuple_element<elt_idx, typename Argc_argv<args_cnt>::Structured_binding_signature> {};


TEST_CASE("Argc_arv")
{
    {
        //        auto [argc, argv] = Argc_argv{"af", "adf"};
        //        REQUIRE(argc == 2);
        auto k = Argc_argv{"asdf", ""};
        static_assert(get<0>(k) == 2);
        REQUIRE(get<1>(k)[0] == std::string_view{"asdf"});
        REQUIRE(get<1>(k)[1] == std::string_view{""});

        auto [ac, av] = k;
        REQUIRE(ac == 2);
        REQUIRE(av[0] == std::string_view{"asdf"});
        REQUIRE(av[1] == std::string_view{""});

    }

    {
        auto [argc, argv] = Argc_argv{};
        REQUIRE(argc == 0);
        REQUIRE(argv[argc] == nullptr);
    }

    {
        auto [argc, argv] = Argc_argv{"asdf", "", "gon"};
        REQUIRE(argc == 3);
        REQUIRE(argv[0] == std::string_view{"asdf"});
        REQUIRE(argv[1] == std::string_view{""});
        REQUIRE(argv[2] == std::string_view{"gon"});
        REQUIRE(argv[argc] == nullptr);
    }

    {
        //        auto [argc, argv] = make_args("af", "adf");
        //        REQUIRE(argc == 2);
    }
    ////    using P = ez::cli::Regular_parameter<"-s", "--some",
    ////        "Test param",
    ////        [](std::string_view){ return 3; }
    ////    >;

    ////    using My_cli = ez::Cli<P>;

    ////    My_cli::parse()

}

//==----------- Incorrect definition --------------==//

TEST_CASE("Parameter set is empty")
{
    using P = ez::cli::Regular_param<"-s", "--some",
        "Test param",
        [](std::string_view){ return 3; }
    >;

    using My_cli = ez::Cli<P>;

    auto [argc, argv] = Argc_argv{};
    My_cli::parse(argc, argv);

}

TEST_CASE("Parameter set contains duplicates")
{
}

TEST_CASE("Parameter set contains parameters with the same name but different type")
{

}

TEST_CASE("Parameter set contains incorrect paameter type")
{

}

//==----------- Use --------------==//
TEST_CASE("The argv[0] isn't treated as a program parameter") {
    // According to [basic.start.main] (C++20 draft) if argc > 0 then argv[0]
    // is either the name used to invoke the programm or an empty string.

//    using Pos_param = ez::cli::Positional_parameter<
//        "some",
//        "Test positional param",
//        []{ return 1; }
//    >;

//    using My_cli = ez::Cli<Pos_param>;

//    auto [argc, argv] = make_args("app", "123");
//    [[maybe_unused]] My_cli::parse(argc, argv);
}

TEST_CASE("Command line is empty")
{
    // correct: Parameter set specifies only optional parameters
    // incorrect: Parameter set specifies mandatory parameters {regular, positional}
}

TEST_CASE("Command line contains only mandatory parameters")
{
    // correct
}

TEST_CASE("Command line doesn't specify some of mandatory parameters")
{
    // incorrect {regular, positional}
}

#include <expected>

using Lexeme = std::string_view;

struct Token {
    enum struct Class {};

    Lexeme lexeme;
    Class token_class;
};

std::optional<Token::Class> find_token(std::span<Token> tokens, Lexeme lexeme)
{
    auto i = std::ranges::find(tokens, lexeme, &Token::lexeme);
    if (i != tokens.end()) {
        return i->token_class;
    }
    return std::nullopt;
}

// template<ez::cli::concepts::Parameter... P>
// auto dfa()
// {
//            P::
// }

TEST_CASE("Dfa")
{

}




