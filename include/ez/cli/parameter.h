#ifndef EZ_CLI_PARAMETER_H
#define EZ_CLI_PARAMETER_H

#include <string_view>
#include "ez/cli/api/parameter.h"
#include "ez/static_string.h"

namespace ez::cli {

namespace details_ {

// Short_name -----
template<utils::Static_string shor_name>
struct Short_name {};

template<utils::Static_string param_name>
    requires (!std::ranges::empty(param_name.value))
struct Short_name<param_name> {
    constexpr static std::string_view short_name = param_name.value;
};

// Long_name -----
template<utils::Static_string long_name>
struct Long_name {};

template<utils::Static_string param_name>
    requires (!std::ranges::empty(param_name.value))
struct Long_name<param_name> {
    constexpr static std::string_view long_name = param_name.value;
};

// Description -----
template<utils::Static_string param_description>
struct Description {};

template<utils::Static_string param_description>
    requires (!std::ranges::empty(param_description.value))
struct Description<param_description> {
    constexpr static std::string_view description = param_description.value;
};

template<auto...>
struct Default_value {};

template<auto...>
struct Value_constructor {};

template<auto...>
struct Value_parser {};

template<auto...>
struct Append_value {};

} // details_

template<typename T, utils::Static_string short_name = "", utils::Static_string long_name = "", utils::Static_string description = "", auto... f>
struct Parameter :
    details_::Short_name<short_name>,
    details_::Long_name<long_name>,
    details_::Description<description>,
    details_::Default_value<f...>,
    details_::Value_constructor<f...>,
    details_::Value_parser<f...>,
    details_::Append_value<f...> {};

} // namespace ez::cli


#endif // EZ_CLI_PARAMETER_H
