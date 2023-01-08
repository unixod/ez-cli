#ifndef EZ_UTILS_STATIC_STRING_H
#define EZ_UTILS_STATIC_STRING_H

#include <algorithm>

namespace ez::utils {

template<std::size_t n>
struct Static_string {
    constexpr /*consteval*/ Static_string(const char(&str)[n])
    {
        // Ensure that str is null-terminated string
//        if (str[n-1] != 0) {
//            throw 1;
//        }

        std::ranges::copy(str, value);
    }


    consteval auto cstr_size() const
    {
        if (value[n-1] != 0) {
            throw "The value doesn't represent null-terminated string.";
        }

        return n-1;
    }

    char value[n];
};

} // namespace ez::utils

#endif // EZ_UTILS_STATIC_STRING_H
