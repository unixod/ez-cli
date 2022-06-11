#ifndef EZ_CLI_DETAILS_LEXER_H
#define EZ_CLI_DETAILS_LEXER_H

#include "ez/cli/parameter.h"

namespace ez::cli::details_ {

template<ez::cli::Named_parameter... P>
consteval auto get_tokens()
{
    struct Token {
        int parameter_index;
        const char* lexeme_begin;
    };

    constexpr auto num_of_tokens = ((Has_short_name<P> + Has_long_name<P>) + ... + 0);

    std::array<Token, num_of_tokens> tokens;

    std::size_t token_index = 0;
    int parameter_index = 0;

    ([&token_index, &tokens, &parameter_index]() {
        if constexpr (Has_short_name<P>) {
            tokens[token_index] = Token{parameter_index, std::data(P::short_name)};
            ++token_index;
        }
        if constexpr (Has_long_name<P>) {
            tokens[token_index] = Token{parameter_index, std::data(P::long_name)};
            ++token_index;
        }
        ++parameter_index;
    }(), ...);

    return tokens;
}

template<cli::Parameter... P>
struct Lexer {
    // TODO: make use cstring_view instead of raw pointer to const char.
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

#endif // EZ_CLI_DETAILS_LEXER_H
