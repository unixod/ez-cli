#ifndef EZ_UTILS_C_STRING_VIEW_H
#define EZ_UTILS_C_STRING_VIEW_H

#include <cassert>
#include <ranges>
#include <algorithm>
#include <compare>

namespace ez::utils {

class C_string_view : public std::ranges::view_interface<C_string_view> {
public:
    class Iterator {
    public:
        using iterator_concept = std::contiguous_iterator_tag;
        using value_type = const char;

        constexpr Iterator() = default;

        constexpr Iterator(const char* p)
            : p_{p} {}

        constexpr auto& operator*() noexcept
        {
            return *p_;
        }

        constexpr auto& operator*() const noexcept
        {
            return *p_;
        }

        constexpr auto& operator [](std::ptrdiff_t n) noexcept
        {
            return p_[n];
        }

        constexpr auto& operator [](std::ptrdiff_t n) const noexcept
        {
            return p_[n];
        }

        constexpr auto operator <=> (const Iterator&) const noexcept = default;

        constexpr auto operator == (Iterator i) const noexcept
        {
            return p_ == i.p_;
        }

        constexpr auto operator == (std::default_sentinel_t) const noexcept
        {
            return *p_ == '\0';
        }

        constexpr auto& operator++() noexcept
        {
            ++p_;
            return *this;
        }

        constexpr auto operator++(int) noexcept
        {
            return Iterator{p_++};
        }

        constexpr auto& operator--() noexcept
        {
            --p_;
            return *this;
        }

        constexpr auto operator--(int) noexcept
        {
            return Iterator{p_--};
        }

        friend constexpr auto operator - (Iterator a, Iterator b) noexcept
        {
            return a.p_ - b.p_;
        }

        constexpr auto& operator += (std::ptrdiff_t n) noexcept
        {
            p_ += n;
            return *this;
        }

        friend constexpr Iterator operator + (Iterator i, std::ptrdiff_t n) noexcept
        {
            return i.p_ + n;
        }

        friend constexpr Iterator operator + (std::ptrdiff_t n, Iterator i) noexcept
        {
            return i.p_ + n;
        }

        constexpr auto& operator -= (std::ptrdiff_t n) noexcept
        {
            p_ -= n;
            return *this;
        }

        friend constexpr Iterator operator - (Iterator i, std::ptrdiff_t n) noexcept
        {
            return i.p_ - n;
        }

        // It's to satisfy std::contiguous_iterator concept which requires the
        // support of std::to_address(i), where i is an instance of Iterator.
        constexpr auto* operator -> () noexcept
        {
            return p_;
        }

        // It's to satisfy std::contiguous_iterator concept which requires the
        // support of std::to_address(i), where i is an instance of Iterator.
        constexpr auto* operator -> () const noexcept
        {
            return p_;
        }

    private:
        const char* p_;
    };

public:
    constexpr C_string_view(const char* p) noexcept
        : p_{p} {

        assert(p != nullptr);
    }

    constexpr auto begin() const noexcept {
        return Iterator{p_};
    }

    constexpr auto end() const noexcept {
        return std::default_sentinel;
    }

    constexpr std::strong_ordering operator <=> (C_string_view oth) const noexcept
    {
        // Note: end iterator of C_string_view can safely be dereferenced as it
        // points to terminating null by design.
        auto [i1, i2] = std::ranges::mismatch(*this, oth);
        return *i1 <=> *i2;
    }

    constexpr bool operator == (C_string_view oth) const noexcept
    {
        return std::ranges::equal(*this, oth);
    }

private:
    const char* p_;
};


} // namespace ez::utils

template<>
inline constexpr bool std::ranges::enable_borrowed_range<ez::utils::C_string_view> = true;


static_assert(std::ranges::view<ez::utils::C_string_view>);
static_assert(std::ranges::borrowed_range<ez::utils::C_string_view>);
static_assert(std::contiguous_iterator<ez::utils::C_string_view::Iterator>);


#endif // EZ_UTILS_C_STRING_VIEW_H
