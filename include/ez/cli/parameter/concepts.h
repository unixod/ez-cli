#ifndef EZ_CLI_PARAMETER_CONCEPTS_H
#define EZ_CLI_PARAMETER_CONCEPTS_H

#include <string_view>
#include "ez/static_string.h"

// Parameter concepts ---------------------------------------------------------

namespace ez::cli::concepts {
namespace details_ {

template<typename T, typename U>
concept Not_same_as = !std::same_as<T, U>;

template<typename P>
concept Has_unit_name = requires {
    { P::name } -> std::convertible_to<std::string_view>;
    requires !std::string_view{P::name}.empty();
};

template<typename P>
concept Has_short_name = requires {
    { P::short_name } -> std::convertible_to<std::string_view>;
    requires !std::string_view{P::short_name}.empty();
};

template<typename P>
concept Has_long_name = requires {
    { P::long_name } -> std::convertible_to<std::string_view>;
    requires !std::string_view{P::long_name}.empty();
};

template<typename P>
concept Has_description = requires {
    { P::description } -> std::convertible_to<std::string_view>;
    requires !std::string_view{P::description}.empty();
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
concept Default_value_type_matches_parsed_value_type = requires(std::string_view sv) {
    { P::parse_value(sv) } -> std::same_as<decltype(P::default_value())>;
};

struct Mimic {
    template<typename T>
    constexpr operator T&() const noexcept;

    template<typename T>
    constexpr operator T&&() const noexcept;

    static Mimic& value();
};

template<typename P>
concept Has_parse_repeated_value_alike = requires(std::string_view arg) {
    P::parse_repeated_value(Mimic::value(), arg);
};

template<typename P>
concept Has_parse_repeated_value =
    requires(std::string_view arg, decltype(P::parse_value(arg)) accumulator) {
        P::parse_repeated_value(accumulator, arg);
    };

template<typename P>
concept Positional_param =
    Has_unit_name<P> &&
    Has_description<P> &&
    Has_value_parser<P> &&
    (!Has_default_value<P> || Default_value_type_matches_parsed_value_type<P>);

template<typename P>
concept Regular_param =
    (Has_short_name<P> || Has_long_name<P>) &&
    Has_description<P> &&
    Has_value_parser<P> &&
    (!Has_default_value<P> || Default_value_type_matches_parsed_value_type<P>) &&
    (!Has_parse_repeated_value_alike<P> || Has_parse_repeated_value<P>);

template<typename P>
concept Bool_param =
    (Has_short_name<P> || Has_long_name<P>) &&
    Has_description<P> &&
    Has_true_value<P> &&
    Has_false_value<P> &&
    requires {
        { P::false_value() } -> std::same_as<decltype(P::true_value())>;
    };

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
concept Boolean_parameter =
    !details_::Positional_param<P> &&
    !details_::Regular_param<P> &&
    details_::Bool_param<P>;

template<typename P>
concept Parameter =
    Regular_parameter<P> ||
    Boolean_parameter<P> ||
    Positional_parameter<P>;

} // namespace ez::cli::concepts


// Parameter traits -----------------------------------------------------------

namespace ez::cli::traits {
namespace details_ {

template<concepts::Parameter P>
struct Param_value_t_impl {
    using Type = decltype(P::parse_value(std::string_view{}));
};

template<concepts::Boolean_parameter P>
struct Param_value_t_impl<P> {
    using Type = decltype(P::true_value());
};

} // namespace details_


template<concepts::Parameter P>
using Param_value_t = typename details_::Param_value_t_impl<P>::Type;

} // namespace ez::cli::traits

#endif // EZ_CLI_PARAMETER_CONCEPTS_H
