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
concept Has_default_value = requires {
    { P::default_value() } -> Not_same_as<void>;
};

template<typename P>
concept Has_value_parser = requires(std::string_view sv) {
    { P::value(sv) } -> Not_same_as<void>;
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
concept Has_append_value = requires(std::string_view sv) {
    P::append_value(std::declval<decltype(P::value(sv))&>(), sv);
};

} // namespce details_

// <app> value
template<typename P>
concept Positional_parameter =
    details_::Has_unit_name<P> &&
    details_::Has_value_parser<P/*, Param_value_t<P>*/>;

// <app> -a=value -a value --arg=value --arg value
template<typename P>
concept Regular_parameter =
    (details_::Has_short_name<P> || details_::Has_long_name<P>) &&
    details_::Has_value_parser<P/*, Param_value_t<P>*/>;

// <app> -a --arg
template<typename P>
concept Bool_parameter =
    (details_::Has_short_name<P> || details_::Has_long_name<P>) &&
    details_::Has_true_value<P> &&
    details_::Has_false_value<P>;

template<typename P>
concept Named_parameter =
    Regular_parameter<P> ||
    Bool_parameter<P>;

template<typename P>
concept Parameter =
    Named_parameter<P> ||
    Positional_parameter<P>;

} // namespace ez::cli::api

#endif // EZ_CLI_PARAMETER_CONCEPTS_H
