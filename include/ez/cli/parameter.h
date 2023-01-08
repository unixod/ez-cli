#ifndef EZ_CLI_PARAMETER_H
#define EZ_CLI_PARAMETER_H

#include <string_view>
#include "ez/utils/match.h"
#include "ez/cli/parameter/concepts.h"
#include "ez/static_string.h"

namespace ez::cli {

namespace details_ {

// Default value --------------------------------------------------------------
template<auto... default_value_func_candidates>
struct Default_value {};

template<auto f, auto... rest>
    requires requires { f(); }
struct Default_value<f, rest...> {
    static_assert( !requires { Default_value<rest...>::default_value(); },
        "ez::cli (positional and regular) parameters should be parametrized by"
        " at most one 'default-value function' (several ones are provided).");

    static constexpr decltype(auto) default_value() noexcept(noexcept(f())) {
        return f();
    }
};

template<auto f, auto... rest>
struct Default_value<f, rest...> : Default_value<rest...> {};

// Value parser ---------------------------------------------------------------
template<auto... value_parser_candidates>
struct Value_parser {};

template<auto f, auto... rest>
    requires requires(std::string_view sv) {
        f(sv);
    }
struct Value_parser<f, rest...> {
    static_assert(
        !(requires(std::string_view sv) { rest(sv); } || ...),

        "ez::cli (positional and regular) parameters should be parametrized by"
        " at most one 'parse-value function' (several ones are provided)."
    );

    static constexpr decltype(auto) parse_value(std::string_view sv) noexcept(noexcept(f(sv))) {
        return f(sv);
    }
};

template<auto f, auto... rest>
struct Value_parser<f, rest...> : Value_parser<rest...> {};

// Repeated value parser ------------------------------------------------------
template<typename Parse_value, auto... func_candadates>
struct Repeated_value_parser {};

template<typename Parse_value, auto f, auto... rest>
    requires requires(decltype(Parse_value::parse_value(std::string_view{})) accum) {
        f(accum, std::string_view{});
    }
struct Repeated_value_parser<Parse_value, f, rest...> {
    using Param_type = decltype(Parse_value::parse_value(std::string_view{}));

    static_assert(
        !(requires { rest(std::declval<Param_type&>(), std::string_view{}); } || ...),

        "ez::cli (positional and regular) parameters should be parametrized by"
        " at most one 'repeated-value-parser' function (several ones are provided)."
    );

    static constexpr void parse_repeated_value(Param_type& accum, std::string_view arg) noexcept(
        noexcept(f(accum, arg)))
    {
        f(accum, arg);
    }
};

template<typename Parse_value, auto f, auto... rest>
struct Repeated_value_parser<Parse_value, f, rest...> :
    Repeated_value_parser<Parse_value, rest...> {};

// True value -----------------------------------------------------------------
template<auto... true_value_func_candadates>
struct True_value {};

template<auto f, auto... rest>
    requires requires {
        f(std::true_type{});
    }
struct True_value<f, rest...> {
    static_assert(
        !(requires { rest(std::true_type{}); } || ...),

        "ez::cli::Boolean_parameter should be parametrized exactly by single true-value function"
        " with the signature: std::true_type -> parameter-value-type."
    );

    static constexpr auto true_value() noexcept(noexcept(f(std::true_type{})))
    {
        return f(std::true_type{});
    }
};

template<auto f, auto... rest>
struct True_value<f, rest...> : True_value<rest...> {};

// False value ----------------------------------------------------------------
template<auto... false_value_func_candadates>
struct False_value {};

template<auto f, auto... rest>
    requires requires {
        f(std::false_type{});
    }
struct False_value<f, rest...> {
    static_assert(
        !(requires { rest(std::false_type{}); } || ...),

        "ez::cli::Boolean_parameter should be parametrized exactly by single false-value function"
        " with the signature: std::false_type -> parameter-value-type."
    );

    static constexpr auto false_value() noexcept(noexcept(f(std::false_type{})))
    {
        return f(std::false_type{});
    }
};

template<auto f, auto... rest>
struct False_value<f, rest...> : False_value<rest...> {};

} // details_


/// Positional parameters helper constructor.
template<utils::Static_string param_name, utils::Static_string param_description, auto... f>
struct Positional_parameter :
    details_::Default_value<f...>,
    details_::Value_parser<f...>,
    details_::Repeated_value_parser<details_::Value_parser<f...>, f...> {

    static constexpr auto name = std::string_view{param_name.value};
    static constexpr auto description = std::string_view{param_description.value};
};

/// Regular (i.e. not positional nor boolean) parameters helper constructor.
template<utils::Static_string param_short_name, utils::Static_string param_long_name,
         utils::Static_string param_description,
         auto... f>
struct Regular_parameter :
    details_::Default_value<f...>,
    details_::Value_parser<f...>,
    details_::Repeated_value_parser<details_::Value_parser<f...>, f...> {

    static constexpr auto short_name = std::string_view{param_short_name.value};
    static constexpr auto long_name = std::string_view{param_long_name.value};
    static constexpr auto description = std::string_view{param_description.value};
};

/// Boolean parameters helper constructor.
template<utils::Static_string param_short_name, utils::Static_string param_long_name,
         utils::Static_string param_description,
         auto... f>
struct Boolean_parameter :
    details_::True_value<f...>,
    details_::False_value<f...> {

    static constexpr auto short_name = std::string_view{param_short_name.value};
    static constexpr auto long_name = std::string_view{param_long_name.value};
    static constexpr auto description = std::string_view{param_description.value};
};

} // namespace ez::cli


#endif // EZ_CLI_PARAMETER_H
