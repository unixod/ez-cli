#ifndef EZ_CLI_PARAMETER_TRAITS_H
#define EZ_CLI_PARAMETER_TRAITS_H

#include "ez/cli/parameter.h"

namespace ez::cli::api {

namespace details_ {

template<Parameter P>
struct Param_value_type_impl {
    using type = decltype(P::value(std::declval<std::string_view>()));
};

template<Boolean_parameter P>
struct Param_value_type_impl<P> {
    using type = decltype(P::value());
};

} // namespce details_


template<Parameter P>
using Param_value_t = typename details_::Param_value_type_impl<P>::type;

} // namespace ez::cli::api


#endif // EZ_CLI_PARAMETER_TRAITS_H
