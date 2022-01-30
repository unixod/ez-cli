#ifndef EZ_GSL_H
#define EZ_GSL_H

namespace ez::gsl {

template<typename To, typename From>
To narrow_cast(From&& from) noexcept
{
    assert(std::cmp_less_equal(from, std::numeric_limits<To>::max()));
    assert(std::cmp_greater_equal(from, std::numeric_limits<To>::min()));
    return static_cast<To>(std::forward<From>(from));
}

} // namespace ez::gsl

#endif // EZ_GSL_H
