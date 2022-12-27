#include <catch2/catch_all.hpp>
#include "ez/cli/parameter.h"
#include "ez/cli/parameter/traits.h"
#include "ez/utils/type-conversion.h"

template<typename T>
constexpr bool complete_t() {
    if constexpr (requires { T{}; } ) {
        return true;
    }
    else {
        return false;
    }
}



TEST_CASE("ez::cli::Positional_parameter must have name, description and value-parser" )
{
    using P = ez::cli::Positional_parameter<"some-param",
        "The some-param descritption.",
        [](std::string_view sv) {
                                                std::from_ch
            return 3;
        }
    >;

    STATIC_REQUIRE(P::name == "some-param");
    STATIC_REQUIRE(P::description == "The some-param descritption.");
    STATIC_REQUIRE(P::value("123") == 123);
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
