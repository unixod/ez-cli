#ifndef EZ_CLI_PARAMETER_TRAITS_H
#define EZ_CLI_PARAMETER_TRAITS_H

#include "ez/cli/parameter/concepts.h"

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


#endif // EZ_CLI_PARAMETER_TRAITS_H
