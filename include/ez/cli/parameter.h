#ifndef EZ_CLI_PARAMETER_H
#define EZ_CLI_PARAMETER_H

namespace ez::cli {

namespace details_ {

template<typename T, typename U>
concept Not_same_as = !std::same_as<T, U>;

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
concept Has_value_constructor = requires(std::string_view sv) {
    { P::value() } -> Not_same_as<void>;
};

template<typename P>
concept Has_append_value = requires(std::string_view sv) {
    P::append_value(std::declval<decltype(P::value(sv))&>(), sv);
};

} // namespce details_

// <app> -a --arg
template<typename P>
concept Named_parameter_without_value =
    (details_::Has_short_name<P> || details_::Has_long_name<P>) &&
    details_::Has_value_constructor<P> &&
    (details_::Has_default_value<P> || std::constructible_from<decltype(P::value())>);

// <app> -a=value -a value --arg=value --arg value
template<typename P>
concept Named_parameter_with_value =
    (details_::Has_short_name<P> || details_::Has_long_name<P>) &&
    details_::Has_value_parser<P>;

// <app> value
template<typename P>
concept Positional_parameter =
    !details_::Has_short_name<P> &&
    !details_::Has_long_name<P> &&
    details_::Has_value_parser<P>;

template<typename P>
concept Named_parameter =
    Named_parameter_with_value<P> ||
    Named_parameter_without_value<P>;

template<typename P>
concept Parameter =
    Named_parameter<P> ||
    Positional_parameter<P>;

} // namespace ez::cli


#endif // EZ_CLI_PARAMETER_H
