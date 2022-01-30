#ifndef EZ_UTILS_STATIC_STRING_H
#define EZ_UTILS_STATIC_STRING_H

#include <algorithm>

namespace ez::utils {

template<std::size_t n>
struct Static_string {
    constexpr Static_string(const char(&str)[n])
    {
        std::ranges::copy(str, value);
    }

    char value[n];
};

} // namespace ez::utils

#endif // EZ_UTILS_STATIC_STRING_H
