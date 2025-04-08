#ifndef EZ_UTILS_STATIC_STRING_H
#define EZ_UTILS_STATIC_STRING_H

#include <algorithm>
#include <functional>
#include <ranges>
#include <string_view>

namespace ez::utils {

template<std::size_t n>
    requires (n > 0)
struct Static_string {
    constexpr /*consteval*/ Static_string(const char(&str)[n])
    {
        // Ensure that str is null-terminated string
       // if (str[n-1] != 0) {
       //     throw 1;
       // }

        std::ranges::copy(str, value);
    }

    constexpr auto up_to_null() const -> std::string_view
    {
        // NOTE: the simple std::string_view{value} isn't safe for the case if value contains no
        // null characters as std::string_view::string_view(const CharT*) constructor is used.
        return std::string_view{std::ranges::begin(value), std::ranges::find(value, 0)};
    }

    // constexpr auto begin(this auto& self) noexcept
    // {
    //     return std::ranges::begin(self.value);
    // }

    // constexpr auto end(this auto& self) noexcept
    // {
    //     return std::ranges::end(self.value);
    // }

    char value[n];
};

} // namespace ez::utils

#endif // EZ_UTILS_STATIC_STRING_H
