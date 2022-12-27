#ifndef EZ_CLI_PARAMETER_CONCEPTS_H
#define EZ_CLI_PARAMETER_CONCEPTS_H

#include <string_view>
#include "ez/static_string.h"

namespace ez::cli::api {

namespace details_ {

template<typename T, typename U>
concept Not_same_as = !std::same_as<T, U>;

template<typename P>
concept Has_unit_name = requires {
    { P::name } -> std::convertible_to<std::string_view>;
};

template<typename P>
concept Has_short_name = requires {
    { P::short_name } -> std::convertible_to<std::string_view>;
};

template<typename P>
concept Has_long_name = requires {
    { P::long_name } -> std::convertible_to<std::string_view>;
};

template<typename P>
concept Has_description = requires {
    { P::description } -> std::convertible_to<std::string_view>;
};

template<typename P>
concept Has_default_value = requires {
    { P::default_value() } -> Not_same_as<void>;
};

template<typename P>
concept Has_value_parser = requires(std::string_view sv) {
    { P::parse_value(sv) } -> Not_same_as<void>;
};

template<typename P>
concept Has_true_value = requires {
    { P::true_value() } -> Not_same_as<void>;
};

template<typename P>
concept Has_false_value = requires {
    { P::false_value() } -> Not_same_as<void>;
};

template<typename P>
concept Has_repeated_value_parser = requires(std::string_view sv) {
    P::parse_repeated_value(std::declval<decltype(P::value(sv))&>(), sv);
};

template<typename P>
concept Positional_param =
    Has_unit_name<P> &&
    Has_description<P> &&
    Has_value_parser<P> &&
    (!Has_default_value<P> || requires(std::string_view sv) {
        { P::parse_value(sv) } -> std::same_as<decltype(P::default_value())>;
    });

template<typename P>
concept Regular_param =
    (Has_short_name<P> || Has_long_name<P>) &&
    Has_description<P> &&
    Has_value_parser<P> &&
    (!Has_default_value<P> || requires(std::string_view sv) {
        { P::parse_value(sv) } -> std::same_as<decltype(P::default_value())>;
    });

template<typename P>
concept Bool_param =
    (Has_short_name<P> || Has_long_name<P>) &&
    Has_description<P> &&
    Has_true_value<P> &&
    Has_false_value<P>;

} // namespce details_

// <app> value
template<typename P>
concept Positional_parameter =
    details_::Positional_param<P> &&
    !details_::Regular_param<P> &&
    !details_::Bool_param<P>;

// <app> -a=value -a value --arg=value --arg value
template<typename P>
concept Regular_parameter =
    !details_::Positional_param<P> &&
    details_::Regular_param<P> &&
    !details_::Bool_param<P>;

// <app> -a --arg
template<typename P>
concept Bool_parameter =
    !details_::Positional_param<P> &&
    !details_::Regular_param<P> &&
    details_::Bool_param<P>;

template<typename P>
concept Parameter =
    Regular_parameter<P> ||
    Bool_parameter<P> ||
    Positional_parameter<P>;

} // namespace ez::cli::api

#endif // EZ_CLI_PARAMETER_CONCEPTS_H
