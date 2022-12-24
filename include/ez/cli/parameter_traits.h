#ifndef EZ_CLI_PARAMETER_TRAITS_H
#define EZ_CLI_PARAMETER_TRAITS_H

#include "ez/cli/parameter.h"

namespace ez::cli::api {

namespace details_ {

template<Parameter P>
struct Parameter_value_type_impl {
    using type = decltype(P::value(std::declval<std::string_view>()));
};

template<Named_parameter_without_value P>
struct Parameter_value_type_impl<P> {
    using type = decltype(P::value());
};

} // namespce details_


template<Parameter P>
using Parameter_value_t = typename details_::Parameter_value_type_impl<P>::type;

} // namespace ez::cli::api


#endif // EZ_CLI_PARAMETER_TRAITS_H
