#include <catch2/catch_all.hpp>
#include "ez/cli/parameter.h"
#include "ez/cli/parameter/traits.h"
#include "ez/cli/parameter/concepts.h"

using ez::cli::api::Positional_parameter;
using ez::cli::api::Regular_parameter;
using ez::cli::api::Bool_parameter;
using ez::cli::api::Parameter;

TEST_CASE("Mandatory ez::cli::Positional_parameter (i.e. has no default value)")
{
    using P = ez::cli::Positional_parameter<"some-param",
        "The some-param descritption.",
        [](std::string_view sv) {
            return sv;
        }
    >;

    STATIC_REQUIRE(P::name == "some-param");
    STATIC_REQUIRE(P::description == "The some-param descritption.");
    STATIC_REQUIRE(P::parse_value("123") == "123");

    STATIC_REQUIRE(Positional_parameter<P>);
    STATIC_REQUIRE_FALSE(Regular_parameter<P>);
    STATIC_REQUIRE_FALSE(Bool_parameter<P>);
    STATIC_REQUIRE(Parameter<P>);
}

TEST_CASE("Optional ez::cli::Positional_parameter (i.e. has default value)")
{
    using namespace std::string_view_literals;

    using P = ez::cli::Positional_parameter<"some-param",
        "The some-param descritption.",
        []() {
            return "default-value"sv;
        },
        [](std::string_view sv) {
            return sv;
        }
    >;

    STATIC_REQUIRE(P::name == "some-param");
    STATIC_REQUIRE(P::description == "The some-param descritption.");
    REQUIRE(P::default_value() == "default-value");
    REQUIRE(P::parse_value("123") == "123");

    STATIC_REQUIRE(Positional_parameter<P>);
    STATIC_REQUIRE_FALSE(Regular_parameter<P>);
    STATIC_REQUIRE_FALSE(Bool_parameter<P>);
    STATIC_REQUIRE(Parameter<P>);
}

TEST_CASE("Mandatory ez::cli::Regular_parameter (i.e. has no default value)")
{
    using P = ez::cli::Regular_parameter<"short-name", "long-name",
        "The parameter descritption.",
        [](std::string_view sv) {
            return sv;
        }
    >;

    STATIC_REQUIRE(P::short_name == "short-name");
    STATIC_REQUIRE(P::long_name == "long-name");
    STATIC_REQUIRE(P::description == "The parameter descritption.");
    STATIC_REQUIRE(P::parse_value("123") == "123");

    STATIC_REQUIRE_FALSE(Positional_parameter<P>);
    STATIC_REQUIRE(Regular_parameter<P>);
    STATIC_REQUIRE_FALSE(Bool_parameter<P>);
    STATIC_REQUIRE(Parameter<P>);
}

struct AmbigousP {
    static constexpr std::string short_name = "short-some";
    static constexpr std::string name = "some";
    static int parse_value(std::string_view) {
        return 1;
    }
};


struct TypesDontMatchPpositional {
    static constexpr std::string name = "some";

    static int parse_value(std::string_view) {
        return 1;
    }
    static char default_value() {
        return 1;
    }
};

TEST_CASE("Ambigous paramter type")
{
    STATIC_REQUIRE(!Positional_parameter<AmbigousP>);
    STATIC_REQUIRE(!Regular_parameter<AmbigousP>);
    STATIC_REQUIRE(!Positional_parameter<TypesDontMatchPpositional>);
}

//TEST_CASE("")
//{
//    enum struct Int {};

//    using A = ez::cli::Parameter<"-a", "",
//        "Foo is optional flag",
//        [/*default value*/]() {
//            return Int{9};
//        },
//        [](std::true_type) {
//            return Int{123};
//        }
//    >;

//    static_assert(std::is_same_v<ez::cli::api::Parameter_value_t<A>, Int>);
//    static_assert(sizeof(A) != 0);
//}



namespace details_ = ez::cli::details_;

//template<typename T>
//constexpr auto has_default_value_func = requires {
//    T::default_value();
//};


//TEST_CASE("cli::details_::Default_value<T, f>, T is convertible from f()")
//{
//    using P_af = ez::cli::Positional_parameter<"af", "asdf",
//        []{
//            return 3;
//        },
//        [](std::string_view){
//            return std::string{""};
//        }
//    >;
//    REQUIRE(P_af::default_value() == 3);
//    SECTION("f is not noexcept") {
//        auto f = []{ return 3; };
//        using Dv = details_::Default_value<int, f>;
//        STATIC_REQUIRE(Dv::default_value() == f());
//        STATIC_REQUIRE(noexcept(Dv::default_value()) == noexcept(f()));
//    }

//    SECTION("f is noexcept") {
//        auto f = []() noexcept { return 3; };

//        using Dv = details_::Default_value<int, f>;
//        STATIC_REQUIRE(Dv::default_value() == f());
//        STATIC_REQUIRE(noexcept(Dv::default_value()) == noexcept(f()));
//    }


////    STATIC_REQUIRE(has_default_value_func<details_::Default_value<[]{}, [](int){}>>);
////    STATIC_REQUIRE(has_default_value_func<details_::Default_value<[](int){}, [](){}>>);
////    STATIC_REQUIRE_FALSE(has_default_value_func<details_::Default_value<[](char){}, [](int){}>>);

//// TODO: Think about how to handle test cases which imply triggering static asserts somewhere in the
//// code. For instance the following line must trigger static assert inside details_::Default_value.
//#if 0
//    // Must fail due to ambiguity.
//    STATIC_REQUIRE_FALSE(has_default_value_func<details_::Default_value<[](){}, [](){}>>);
//#endif
//}

//TEST_CASE("cli::details_::Default_value<T, f>, T isn't convertible from f()")
//{
//#if 0
//    SECTION("f is not noexcept") {
//        auto f = []{ return std::string{""}; };
//        using Dv = details_::Default_value<int, f>;
//        STATIC_REQUIRE(Dv::default_value() == f());
//        STATIC_REQUIRE(noexcept(Dv::default_value()) == noexcept(f()));
//    }

//    SECTION("f is noexcept") {
//        auto f = []() noexcept { return std::string{""}; };
//        using Dv = details_::Default_value<int, f>;
//        STATIC_REQUIRE(Dv::default_value() == f());
//        STATIC_REQUIRE(noexcept(Dv::default_value()) == noexcept(f()));
//    }
//#endif
//}


//TODO:
//       1. Introduce inplace of current Parameter
//           PositionalParameter
//           RegularParameter
//           BoolParameter
//       2. Shorten name of concepts and type traits Parameter -> Param
//TEST_CASE("cli::details_::Default_value<T, f...>")
//{
//    SECTION("") {
//        auto f = []{ return 3; };

//        using Dv1 = details_::Default_value<int, f, [](double){}>;
//        STATIC_REQUIRE(Dv1::default_value() == f());
//        STATIC_REQUIRE(noexcept(Dv1::default_value()) == noexcept(f()));

//        using Dv2 = details_::Default_value<int, [](double){}, f>;
//        STATIC_REQUIRE(Dv2::default_value() == f());
//        STATIC_REQUIRE(noexcept(Dv2::default_value()) == noexcept(f()));

//        using Dv3 = details_::Default_value<int, f, [](){ return double{1}; }>;
//        STATIC_REQUIRE(Dv3::default_value() == f());
//        STATIC_REQUIRE(noexcept(Dv3::default_value()) == noexcept(f()));

//        using Dv4 = details_::Default_value<int [](){ return double{1}; }, f>;
//        STATIC_REQUIRE(Dv4::default_value() == f());
//        STATIC_REQUIRE(noexcept(Dv4::default_value()) == noexcept(f()));
//    }

//    SECTION("Funciton is noexcept") {
//        auto f = []() noexcept { return 3; };

//        using Dv1 = details_::Default_value<int, f, [](double){}>;
//        STATIC_REQUIRE(Dv1::default_value() == f());
//        STATIC_REQUIRE(noexcept(Dv1::default_value()) == noexcept(f()));

//        using Dv2 = details_::Default_value<int, [](double){}, f>;
//        STATIC_REQUIRE(Dv2::default_value() == f());
//        STATIC_REQUIRE(noexcept(Dv2::default_value()) == noexcept(f()));

////        using Dv3 = details_::Default_value<f, [](){ /*return void*/ }>;
////        STATIC_REQUIRE(Dv3::default_value() == f());
////        STATIC_REQUIRE(noexcept(Dv3::default_value()) == noexcept(f()));

////        using Dv4 = details_::Default_value<[](){ /*return void*/ }, f>;
////        STATIC_REQUIRE(Dv4::default_value() == f());
////        STATIC_REQUIRE(noexcept(Dv4::default_value()) == noexcept(f()));
//    }
//}
