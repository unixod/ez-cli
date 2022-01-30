#ifndef EZ_CLI_H
#define EZ_CLI_H

#include <concepts>
#include <ranges>
#include <type_traits>
#include "ez/utils/type-conversion.h"
#include "mp.h"  // tmporary
#include "gsl.h" // temporary
#include "static_string.h" // temporary

namespace ez::cli {

namespace details_ {

// Concepts ---------
template<typename T, typename U>
concept Not_same_as = !std::same_as<T, U>;

template<typename P>
concept Has_short_name = requires {
    { P::short_name } -> std::convertible_to<std::string_view>;
};

template<typename P>
concept Has_long_name = requires {
    { P::long_name } -> std::convertible_to<std::string_view>;
};

template<typename P>
concept Has_default_value = requires {
    { P::default_value() } -> Not_same_as<void>;
};

template<typename P>
concept Has_value_parser = requires(std::string_view sv) {
    { P::value(sv) } -> Not_same_as<void>;
};

template<typename P>
concept Has_value_constructor = requires(std::string_view sv) {
    { P::value() } -> Not_same_as<void>;
};

template<typename P>
concept Has_append_value = requires(std::string_view sv) {
    P::append_value(std::declval<decltype(P::value(sv))&>(), sv);
};

} // namespce details_

// gcc -g
template<typename P>
concept Flag =
    (details_::Has_short_name<P> || details_::Has_long_name<P>) &&
    details_::Has_value_constructor<P>;

// gcc -std=c++20
template<typename P>
concept Named_parameter =
    (details_::Has_short_name<P> || details_::Has_long_name<P>) &&
    details_::Has_value_parser<P>;

// gcc main.cpp
template<typename P>
concept Positional_parameter =
    !details_::Has_short_name<P> &&
    !details_::Has_long_name<P> &&
    details_::Has_value_parser<P>;

template<typename P>
concept Parameter =
    Positional_parameter<P> ||
    Named_parameter<P> ||
    Flag<P>;

} // namespace ez::cli


namespace ez::cli::details_ {
template<std::size_t idx, typename Callable, typename Tuple, typename... Rest_args>
auto apply_tuple_element_impl(Callable&& func, std::integral auto index, Tuple&& tpl, Rest_args&&... args)
{
    // TODO: Refactor structure (reduce branches)

    if constexpr (idx < std::tuple_size_v<std::remove_cvref_t<Tuple>>) {
        if (index == idx) {
            return std::invoke(std::forward<Callable>(func), std::get<idx>(std::forward<Tuple>(tpl)), std::forward<Rest_args>(args)...);
        }
    }

    if constexpr (idx+1 < std::tuple_size_v<std::remove_cvref_t<Tuple>>) {
        return apply_tuple_element_impl<idx+1>(std::forward<Callable>(func), index, std::forward<Tuple>(tpl), std::forward<Rest_args>(args)...);
    }

    assert(false && "Unreachable");
}

template<typename Callable, typename Tuple, typename... Rest_args>
auto apply_tuple_element(Callable&& func, std::integral auto index, Tuple&& tpl, Rest_args&&... args)
{
    assert(index >= 0);
    return apply_tuple_element_impl<0>(std::forward<Callable>(func), index, std::forward<Tuple>(tpl), std::forward<Rest_args>(args)...);
}

} // namespace ez::cli::details_

namespace ez::cli::details_ {

template<Parameter P>
struct Parameter_value_type_impl {
    using type = decltype(P::value(std::declval<std::string_view>()));
};

template<Flag P>
struct Parameter_value_type_impl<P> {
    using type = decltype(P::value());
};

template<Parameter P>
using Parameter_value_type = Parameter_value_type_impl<P>;

template<Parameter>
class Parameter_value;

template<Flag P>
class Parameter_value<P> {
public:
    const char** assign(const char* /*arg_lexeme_end*/, auto argv_curr, auto /*argv_end*/) {
        return argv_curr + 1;
    }

private:
    Parameter_value_type<P> value_;
};

template<Named_parameter P>
class Parameter_value<P> {
public:
    const char** assign(const char* /*arg_lexeme_end*/, auto argv_curr, auto /*argv_end*/) {
        return argv_curr + 1;
    }

private:
    Parameter_value_type<P> value_;
};

template<Positional_parameter T>
class Parameter_value<T> {

};

template<ez::cli::Parameter... P>
consteval auto get_tokens()
{
    struct Token {
        int parameter_index;
        const char* lexeme_begin;
    };

    constexpr auto num_of_tokens =
        ((ez::cli::details_::Has_short_name<P> + ez::cli::details_::Has_long_name<P>) + ... + 0);

    std::array<Token, num_of_tokens> tokens;

    std::size_t token_index = 0;
    int parameter_index = 0;
    ([&token_index, &tokens, &parameter_index]() {
        static_assert(ez::cli::details_::Has_short_name<P> || ez::cli::details_::Has_long_name<P>);

        if constexpr (ez::cli::details_::Has_short_name<P>) {
            tokens[token_index] = Token{parameter_index, std::data(P::short_name)};
            ++token_index;
        }
        if constexpr (ez::cli::details_::Has_long_name<P>) {
            tokens[token_index] = Token{parameter_index, std::data(P::long_name)};
            ++token_index;
        }
        ++parameter_index;
    }(), ...);

    return tokens;
}

template<cli::Parameter... P>
struct Lexer {
    constexpr static auto recognize(const char* p) noexcept
        requires (sizeof...(P) > 0)
    {
        auto ts = get_tokens<P...>();

        auto matched_cnt = -1;
        auto matched_parameter_index = -1;
        const char* matched_lexeme_end = nullptr;
        for (; *p != '=' && *p != '\0' && matched_cnt; ++p) {
            matched_cnt = 0;

            for (auto& [param_idx, t] : ts) {
                if (t && *p == *t) {
                    ++t;
                    matched_lexeme_end = t;
                    ++matched_cnt;
                    matched_parameter_index = param_idx;
                }
                else {
                    t = nullptr;
                }
            }
        }

        if (matched_cnt != 1 || *matched_lexeme_end != '\0') {
            return std::pair{-1, p};
        }

        return std::pair{matched_parameter_index, p};
    }
};
} // namespace ez::cli::details_

namespace ez {


template<typename...>
class Cli;

template<cli::Parameter... P>
class Cli<P...> {
    template<typename T>
    using Is_named_parameter = std::bool_constant<cli::Named_parameter<T> || cli::Flag<T>>;

    template<typename T>
    using Is_positional_param = std::bool_constant<cli::Positional_parameter<T>>;

    using Named_params_tuple = utils::mp::Filter<Is_named_parameter, std::tuple<P...>>;
    using Positional_params_tuple = utils::mp::Filter<Is_positional_param, std::tuple<P...>>;

public:
    template<typename T>
        requires std::same_as<std::remove_const_t<T>, char>
    Cli(std::integral auto argc, T** argv)
    {
        using Lexer = utils::mp::Apply<cli::details_::Lexer, Named_params_tuple>;

        auto positional_arg_idx = 0;

        auto args =
            std::views::counted(argv, gsl::narrow_cast<std::ptrdiff_t>(argc))
            | std::views::drop(1);

        for (auto p = args.begin(), e = args.end(); p != e;) {
            if (auto [token_index, lexeme_end] = Lexer::recognize(*p); token_index != -1) {
                // a flag (e.g. -a, --a-param) or named parameter (e.g. --param=value, --param value, -p value)
                auto next = handle_named_arg_(token_index, lexeme_end, p, e);
                assert(p != next);
                p = next;
            }
            else if (positional_arg_idx < utils::to_signed(std::tuple_size_v<Positional_params_tuple>)) {
                handle_positional_arg_(positional_arg_idx, *p);
                ++p;
            }
            else {
                throw_error_(args, std::distance(p, e), "Too much positional arguments.");
            }
        }
    }

private:
    auto handle_named_arg_(std::integral auto token_index, const char* arg_lexeme_end, auto argv_curr, auto argv_end)
    {
        auto assign = []<typename... T>(auto& v, T&&... args){
            return v.assign(std::forward<T>(args)...);
        };

        return cli::details_::apply_tuple_element(assign, token_index, named_args_, arg_lexeme_end, argv_curr, argv_end);
    }

    void handle_positional_arg_(std::integral auto token_index, auto arg)
    {
        auto assign = [](auto& v, auto a){
            return v.assign(a);
        };

        return cli::details_::apply_tuple_element(assign, token_index, positional_args_, *arg);
    }

    [[noreturn]] void throw_error_(std::ranges::view auto /*args*/, std::integral auto /*err_argv_idx*/, std::string_view /*msg*/)
    {
        throw std::runtime_error{"Not implemented yet."};
    }

private:
    using Named_param_values_tuple = utils::mp::Transform<cli::details_::Parameter_value, Named_params_tuple>;
    using Positional_param_values_tuple = utils::mp::Transform<cli::details_::Parameter_value, Positional_params_tuple>;

    Named_param_values_tuple named_args_;
    Positional_param_values_tuple positional_args_;
};


} // namespace ez

#endif // EZ_CLI_H
