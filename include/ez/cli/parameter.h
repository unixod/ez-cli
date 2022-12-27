#ifndef EZ_CLI_PARAMETER_H
#define EZ_CLI_PARAMETER_H

#include <string_view>
#include "ez/utils/match.h"
#include "ez/cli/parameter/concepts.h"
#include "ez/static_string.h"

namespace ez::cli {

namespace details_ {

// Unit_name -----
template<utils::Static_string name>
struct Unit_name {};

template<utils::Static_string param_name>
    requires (!std::ranges::empty(param_name.value))
struct Unit_name<param_name> {
    static constexpr std::string_view name = param_name.value;
};

// Short_name -----
template<utils::Static_string shor_name>
struct Short_name {};

template<utils::Static_string param_name>
    requires (!std::ranges::empty(param_name.value))
struct Short_name<param_name> {
    static constexpr std::string_view short_name = param_name.value;
};

// Long_name -----
template<utils::Static_string long_name>
struct Long_name {};

template<utils::Static_string param_name>
    requires (!std::ranges::empty(param_name.value))
struct Long_name<param_name> {
    static constexpr std::string_view long_name = param_name.value;
};

// Description -----
template<utils::Static_string param_description>
struct Description {};

template<utils::Static_string param_description>
    requires (!std::ranges::empty(param_description.value))
struct Description<param_description> {
    static constexpr std::string_view description = param_description.value;
};

// Default value -----
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

// Value value -----
template<auto... value_parser_candidates>
struct Value_parser {
    static_assert(ez::utils::always_false<decltype(value_parser_candidates)...>,
                  "ez::cli (positional and regular) parameters require to specify "
                  "a 'value-parser function': std::string_view -> parameter-value-type.");
};

template<auto f, auto... rest>
    requires requires(std::string_view sv) {
        f(sv);
    }
struct Value_parser<f, rest...> {
    static_assert(
        !(requires(std::string_view sv) {
            rest(sv);
        } || ...),

        "ez::cli (positional and regular) parameters should be parametrized by"
        " at most one 'value-parser function' (several ones are provided)."
    );

    static constexpr decltype(auto) value(std::string_view sv) noexcept(noexcept(f(sv))) {
        return f(sv);
    }
};

template<auto f, auto... rest>
struct Value_parser<f, rest...> : Value_parser<rest...> {};


template<auto...>
struct Repeated_value_parser {};

template<auto...>
struct True_value {};

template<auto...>
struct False_value {};

} // details_


/// Positional parameter.
template<utils::Static_string name, utils::Static_string description, auto... f>
struct Positional_parameter :
    details_::Unit_name<name>,
    details_::Description<description>,
    details_::Default_value<f...>,
    details_::Value_parser<f...> {

private:
    using Value_type_ = decltype(details_::Value_parser<f...>::value(std::string_view{}));

    static_assert(
        ! requires {
            details_::Default_value<f...>::default_value();
        } ||
        requires {
            {details_::Default_value<f...>::default_value()} -> std::convertible_to<Value_type_>;
        },

        "Return value type of 'default-value function' should be convertible to"
        " parameter value type."
    );
};

/// Regular (non-positional and non boolean) parameter.
template<
    utils::Static_string short_name,
    utils::Static_string long_name,
    utils::Static_string description,
    auto... f
>
struct Regular_parameter :
    details_::Short_name<short_name>,
    details_::Long_name<long_name>,
    details_::Description<description>,
    details_::Default_value<f...>,
    details_::Value_parser<f...>,
    details_::Repeated_value_parser<f...> {

private:
    using Value_type_ = decltype(details_::Value_parser<f...>::value(std::string_view{}));

    static_assert(
        ! requires {
            details_::Default_value<f...>::default_value();
        } ||
        requires {
            {details_::Default_value<f...>::default_value()} -> std::convertible_to<Value_type_>;
        },

        "Return value type of 'default-value function' should be convertible to"
        " parameter value type."
    );
};

/// Boolean parameter.
template<
    utils::Static_string short_name,
    utils::Static_string long_name,
    utils::Static_string description,
    auto... f
>
struct Bool_parameter :
    details_::Short_name<short_name>,
    details_::Long_name<long_name>,
    details_::Description<description>,
    details_::True_value<f...>,
    details_::False_value<f...> {

private:
    using True_value_type_ = decltype(details_::True_value<f...>::true_value());
    using False_value_type_ = decltype(details_::False_value<f...>::false_value());

    static_assert(std::is_same_v<True_value_type_, False_value_type_>,
        "Bool_parameter::true_value() and Bool_parameter::false_value() return value types"
        " should be same."
    );
};

} // namespace ez::cli


#endif // EZ_CLI_PARAMETER_H
