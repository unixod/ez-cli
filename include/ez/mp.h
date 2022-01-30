#ifndef EZ_UTILS_MP_H
#define EZ_UTILS_MP_H

#include <tuple>
#include <type_traits>

namespace ez::utils::mp {

namespace details_ {

template<
    template<typename...> typename P,
    typename Tuple,
    typename Result = std::tuple<>
    >
struct Filter_impl {
    using type = Result;
};

template<
    template<typename...> typename P,
    typename H, typename... T,
    typename... R
    >
struct Filter_impl<P, std::tuple<H, T...>, std::tuple<R...>>
    : Filter_impl<P, std::tuple<T...>,
                  std::conditional_t<P<H>::value,
                                     std::tuple<R..., H>,
                                     std::tuple<R...>>> {};

template<template<typename...> typename F, typename Tuple>
struct Transform_impl;

template<template<typename...> typename F, typename... T>
struct Transform_impl<F, std::tuple<T...>> {
    using type = std::tuple<F<T>...>;
};

template<template<typename...> typename F, typename Tuple>
struct Apply_impl;

template<template<typename...> typename F, typename... T>
struct Apply_impl<F, std::tuple<T...>> {
    using type = F<T...>;
};

} // namespace details


template<template<typename...> typename Predicate, typename Tuple>
using Filter = typename details_::Filter_impl<Predicate, Tuple>::type;

template<template<typename...> typename F, typename Tuple>
using Transform = typename details_::Transform_impl<F, Tuple>::type;

template<template<typename...> typename F, typename Tuple>
using Apply = typename details_::Apply_impl<F, Tuple>::type;

} // namespace ez::utils::mp

#endif // EZ_UTILS_MP_H
