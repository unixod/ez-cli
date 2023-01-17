#ifndef EZ_CLI_DETAILS_LEXER_H
#define EZ_CLI_DETAILS_LEXER_H

#include <variant>
#include <span>
#include "ez/cli/parameter.h"
#include "ez/c_string_view.h"
#include "ez/mp.h"
#include "ez/static_string.h"
#include "ez/utils/generator.h"

// TODO: Move to separate class
namespace ez::cli {
struct Error {
    struct Unknown_parameter {
    };

    template<concepts::Parameter, utils::Static_string>
    struct Parameter_misuse {};

    Error(Unknown_parameter) {};

    template<concepts::Parameter P, utils::Static_string msg>
    Error(Parameter_misuse<P, msg>) {}
};
} // namespace ez::cli

namespace ez::cli::details_ {

template<typename P>
concept Nonpositional_parameter =
    concepts::Regular_parameter<P> ||
    concepts::Boolean_parameter<P>;

template<typename...>
struct Type_list {};

template<concepts::Parameter P>
using Is_named_param = std::bool_constant<Nonpositional_parameter<P>>;

template<concepts::Parameter P>
using Is_positional_param = std::bool_constant<concepts::Positional_parameter<P>>;

template<concepts::Parameter>
class Token {
    utils::C_string_view value_lexeme;
};

template<concepts::Boolean_parameter P>
class Token<P> {};

template<typename It, concepts::Parameter... P, Nonpositional_parameter... Named_p>
consteval auto get_named_funcs_impl(Type_list<Named_p...>)
{
    using Token_variant = std::variant<Token<P>..., Error>;

    using R = std::pair<Token_variant, It>;
    using F = R(It, const char*, It);

    return std::array<F*, sizeof...(Named_p)>{
        //            -param      aaa ... zzz
        //            -param=aaa  bbb ... zzz
        //            ~~~~~~^~~~              ^______
        //             /     \___________            |
        //            /                  \           |
        [](It args_begin, const char* p_end, It args_end) -> R {
            assert(args_begin != args_end);
            assert(*p_end == '\0' || *p_end == '=');

            if constexpr (!concepts::details_::Has_value_parser<Named_p>) {
                if (*p_end == '=') {
                    return R{
                        Error::Parameter_misuse<Named_p, "Parameter doesn't accept values">{},
                        args_begin
                    };
                }

                return R{Token<Named_p>{}, std::next(args_begin)};
            }
            else if (*p_end == '=') {
                return R{Token<Named_p>{std::next(p_end)}, std::next(args_begin)};
            }
            else if (auto v = std::next(args_begin); v != args_end){
                return R{Token<Named_p>{*v}, std::next(v)};
            }
            else {
                return R{
                    Error::Parameter_misuse<Named_p, "Parameter requires value">{},
                    args_begin
                };
            }
        }...
    };
}

template<typename It, concepts::Parameter... P>
consteval auto get_named_funcs_()
{
    using Named_param_types =
        utils::mp::Apply<Type_list, utils::mp::Filter<Is_named_param, std::tuple<P...>>>;

    return get_named_funcs_impl<It, P...>(Named_param_types{});
}

template<concepts::Parameter... P, concepts::Positional_parameter... Positional_p>
consteval auto get_positional_funcs_impl(Type_list<Positional_p...>)
{
    using Token_variant = std::variant<Token<P>..., Error>;

    using F = Token_variant(const char*);

    return std::array<F*, sizeof...(Positional_p)>{
        [](const char* p) -> Token_variant {
            return Token<Positional_p>{*p};
        }...
    };
}

template<concepts::Parameter... P>
consteval auto get_positional_funcs_()
{
    using Positional_params_types =
        utils::mp::Apply<Type_list, utils::mp::Filter<Is_positional_param, std::tuple<P...>>>;

    return get_positional_funcs_impl<P...>(Positional_params_types{});
}

template<Nonpositional_parameter... P>
consteval auto get_named_param_tokens_()
{
    struct Param_token {
        std::size_t parameter_index;
        const char* lexeme_begin;
    };

    using concepts::details_::Has_short_name;
    using concepts::details_::Has_long_name;
    constexpr auto num_of_tokens = ((Has_short_name<P> + Has_long_name<P>) + ... + 0);

    std::size_t token_index = 0;
    std::array<Param_token, num_of_tokens> tokens;
    std::size_t parameter_index = 0;
    ([&token_index, &tokens, &parameter_index]() {
        if constexpr (Has_short_name<P>) {
            tokens[token_index] = Param_token{parameter_index, std::data(P::short_name)};
            ++token_index;
        }
        if constexpr (Has_long_name<P>) {
            tokens[token_index] = Param_token{parameter_index, std::data(P::long_name)};
            ++token_index;
        }
        ++parameter_index;
    }(), ...);

    return tokens;
}

template<Nonpositional_parameter... P>
    requires (sizeof...(P) > 0)
constexpr std::pair<std::optional<std::size_t>, const char*> recognize_(auto p, Type_list<P...>) noexcept
{
    auto ts = get_named_param_tokens_<P...>();

    auto matched_cnt = sizeof...(P);
    auto matched_parameter_index = sizeof...(P);
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

    if (matched_cnt == 1 && *matched_lexeme_end == '\0') {
        return std::pair{matched_parameter_index, p};
    }

    return std::pair{std::nullopt, p};
}

template<concepts::Parameter... P>
    requires (sizeof...(P) > 0)
constexpr std::pair<std::optional<std::size_t>, const char*> recognize_(auto lexeme) noexcept
{
    using Named_params_types =
        utils::mp::Apply<Type_list, utils::mp::Filter<Is_named_param, std::tuple<P...>>>;

    return recognize_<P...>(lexeme, Named_params_types{});
}

template<concepts::Parameter... P>
    requires (sizeof...(P) > 0)
constexpr utils::Generator<std::variant<Token<P>..., Error>> tokenize(std::span<const char*> args)
{
    auto p = args.begin();
    const auto e = args.end();

    constexpr auto named_funcs = get_named_funcs_<decltype(p), P...>();
    constexpr auto positional_funcs = get_positional_funcs_<P...>();

    std::size_t next_positional_param_idx = 0;
    using namespace std::string_view_literals;
    while (p != e && *p != "--"sv) {
        if (auto [param_idx, param_lexeme_end] = recognize_<P...>(*p); param_idx.has_value()) {
            assert(*param_idx < std::size(named_funcs));
            auto [r, next] = named_funcs[*param_idx](p, param_lexeme_end, e);
            co_yield r;
            p = next;
        }
        else if (next_positional_param_idx < positional_funcs.size()) {
            co_yield positional_funcs[next_positional_param_idx](*p);
            ++next_positional_param_idx;
            ++p;
        }
        else {
            co_yield Error::Unknown_parameter{};
            co_return; // FIXME co_return Error::Unknown_parameter{};
        }
    }

    // Handle rest of positional arguments (if any) following '--' in the comand line.
    for (; p != e; ++p, ++next_positional_param_idx) {
        assert(next_positional_param_idx < std::size(positional_funcs));
        co_yield positional_funcs[next_positional_param_idx](*p);
    }
}

} // namespace ez::cli::details_

#endif // EZ_CLI_DETAILS_LEXER_H
