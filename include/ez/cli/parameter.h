#ifndef EZ_CLI_PARAMETER_H
#define EZ_CLI_PARAMETER_H

#include <string_view>

#include <ez/static_string.h>
#include <ez/utils/match.h>

#include "parameter/concepts.h"

namespace ez::cli {

template<typename F>
concept Default_value_func = std::invocable<F>;

template<typename F>
concept Value_parser_func = std::invocable<F, std::string_view>;

template<typename F, typename Accum>
concept Repeated_value_parser_func = std::invocable<F, Accum, std::string_view>;

template<typename F>
concept True_value_func = std::invocable<F, std::true_type>;

template<typename F>
concept False_value_func = std::invocable<F, std::false_type>;


namespace details_ {

// Default value --------------------------------------------------------------

template<auto... default_value_func_candidates>
struct Default_value {};

template<auto f, auto... rest>
    requires Default_value_func<decltype(f)>
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
    requires Value_parser_func<decltype(f)>
struct Value_parser<f, rest...> {
    using Value = std::invoke_result_t<decltype(f), std::string_view&>;

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

template<typename Parser, auto... repeated_value_func_candidates>
struct Repeated_value_parser {};

template<typename Parser, auto f, auto... rest>
    requires Repeated_value_parser_func<decltype(f), typename Parser::Value&>
struct Repeated_value_parser<Parser, f, rest...> {
    static_assert(
        !(requires(std::string_view sv) { rest(std::declval<typename Parser::Value&>(), sv); } || ...),

        "ez::cli (positional and regular) parameters should be parametrized by"
        " at most one 'repeated-value-parser' function (several ones are provided)."
    );

    static constexpr void parse_repeated_value(typename Parser::Value& accum, std::string_view arg) noexcept(
        noexcept(f(accum, arg)))
    {
        f(accum, arg);
    }
};

template<typename Parser, auto f, auto... rest>
struct Repeated_value_parser<Parser, f, rest...> : Repeated_value_parser<Parser, rest...> {};

// True value -----------------------------------------------------------------

template<auto... true_value_func_candidates>
struct True_value {};

template<auto f, auto... rest>
    requires True_value_func<decltype(f)>
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

template<auto... false_value_func_candidates>
struct False_value {};

template<auto f, auto... rest>
    requires False_value_func<decltype(f)>
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


template<auto f, auto... all>
concept Non_boolean_paremeter_behavior =
    Default_value_func<decltype(f)> ||
    Value_parser_func<decltype(f)> ||
    Repeated_value_parser_func<decltype(f), typename details_::Value_parser<all...>::Value&>;

template<typename F>
concept Boolean_paremeter_behavior = True_value_func<F> || False_value_func<F>;

/// Positional parameters helper constructor.
template<ez::utils::Static_string param_name,
         ez::utils::Static_string param_description,
         auto... f>
    requires (Non_boolean_paremeter_behavior<f, f...> && ...)
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
    requires (Non_boolean_paremeter_behavior<f, f...> && ...)
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
         Boolean_paremeter_behavior auto... f>
struct Boolean_parameter :
    details_::True_value<f...>,
    details_::False_value<f...> {

    static constexpr auto short_name = std::string_view{param_short_name.value};
    static constexpr auto long_name = std::string_view{param_long_name.value};
    static constexpr auto description = std::string_view{param_description.value};
};

} // namespace ez::cli


#endif // EZ_CLI_PARAMETER_H
