#ifndef EZ_CLI_DETAILS_UNINITIALIZED_H
#define EZ_CLI_DETAILS_UNINITIALIZED_H

#include <cstddef>
#include <concepts>
#include "ez/utils/badge.h"

namespace ez::cli::details_ {

template<typename T>
class Uninitialized;

class Detached_destructor {
public:
    Detached_destructor() = default;

    template<typename T>
    Detached_destructor(Uninitialized<T>& s, ez::utils::Badge<Uninitialized<T>>)
        : storage_{&s},
        destruct_{[](void* st) {
            assert(st);
            reinterpret_cast<Uninitialized<T>*>(st)->destruct();
        }}
    {}

    Detached_destructor(Detached_destructor&& oth) noexcept
        : ex_cnt_{oth.ex_cnt_} ,
        storage_{oth.storage_},
        destruct_{std::exchange(oth.destruct_, no_op_destruct_)}
    {}

    Detached_destructor& operator = (Detached_destructor&& oth) noexcept
    {
        ex_cnt_ = oth.ex_cnt_;
        storage_ = oth.storage_;
        destruct_ = std::exchange(oth.destruct_, no_op_destruct_);
        return *this;
    }

    Detached_destructor(const Detached_destructor&) = delete;
    Detached_destructor& operator = (const Detached_destructor&) = delete;

    ~Detached_destructor()
    {
        assert(destruct_);

        if (ex_cnt_ != std::uncaught_exceptions()) {
            destruct_(storage_);
        }
    }

    void release()
    {
        destruct_ = no_op_destruct_;
    }

private:
    static void no_op_destruct_(void*) {}

private:
    int ex_cnt_ = std::uncaught_exceptions();
    void* storage_;
    void(*destruct_)(void*) = no_op_destruct_;
};

// NOTE: This type doesn't use virtual functions due to increase of object size and
// thereby losing the whole reason behind this class (as we simply may make use
// std::optional than).
//
// WARNING: Don't move/copy/value until construct or after destruct.
template<typename T>
class Uninitialized {
public:
    Uninitialized() = default;
    Uninitialized(Uninitialized&&) = delete;
    Uninitialized(const Uninitialized&) = delete;
    Uninitialized& operator = (Uninitialized&&) = delete;
    Uninitialized& operator = (const Uninitialized&) = delete;
    ~Uninitialized() = default;

    template<typename... Args>
        requires std::constructible_from<T, Args...>
    Detached_destructor construct(Args&&... args)
    {
        std::construct_at(reinterpret_cast<T*>(storage_), std::forward<Args>(args)...);
        return Detached_destructor{*this, ez::utils::Badge<Uninitialized>{}};
    }

    void destruct()
    {
        std::destroy_at<T>(reinterpret_cast<T*>(storage_));
    }

    auto& get() noexcept
    {
        return *reinterpret_cast<T*>(storage_);
    }

    auto& get() const noexcept
    {
        return *reinterpret_cast<const T*>(storage_);
    }

private:
    alignas(T) std::byte storage_[sizeof(T)];
};

// Resemble Uninitialized interface.
template<typename T>
class Uninitialized_like {
public:
    template<typename... Args>
        requires std::constructible_from<T, Args...>
    Detached_destructor construct(Args&&... args)
    {
        value_ = T{std::forward<Args>(args)...};
        return Detached_destructor{/*noop*/};
    }

    void destruct()
    {}

    auto& get() noexcept
    {
        return value_;
    }

    auto& get() const noexcept
    {
        return value_;
    }

private:
    T value_;
};

} // namespace ez::cli::details_

#endif // EZ_CLI_DETAILS_UNINITIALIZED_H
