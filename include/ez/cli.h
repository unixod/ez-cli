#ifndef EZ_CLI_H
#define EZ_CLI_H

#include <concepts>
#include <ranges>
#include <type_traits>
#include <variant>
#include "ez/utils/match.h"
#include "ez/utils/type-conversion.h"
#include "ez/support/std23.h"
#include "mp.h"  // tmporary
#include "gsl.h" // temporary
#include "ez/cli/details_/lexer.h"
#include "ez/cli/parameter.h"
#include "ez/cli/parameter_traits.h"
#include "ez/c_string_view.h"
#include "ez/cli/details_/uninitialized.h"
#include "ez/utils/generator.h"
#include "ez/old.h"

namespace ez::cli::details_ {

template<typename T, typename... Option>
concept One_of = (std::same_as<T, Option> || ...);

} // namespace ez::cli::details_


template<typename, auto...>
struct Show;

namespace ez {

template<typename...>
class Cli;

namespace details_ {
template<typename>
struct Single_version_cli : std::false_type {};

template<cli::api::Parameter... P>
struct Single_version_cli<Cli<P...>> : std::true_type {};
} // namespace details_

template<typename T>
concept Single_version_cli = details_::Single_version_cli<T>::value;


template<Single_version_cli... S_cli>
class Cli<S_cli...> {
    template<typename...>
    struct Type_list{};

    struct Multi_cli_error {};

public:
    template<typename T>
        requires std::same_as<std::remove_const_t<T>, char>
    static auto parse(std::integral auto argc, T** argv)
    {
        return parse(argc, argv, [](auto error){
            throw error;
        });
    }

    template<typename T>
        requires std::same_as<std::remove_const_t<T>, char>
    static auto parse(std::integral auto argc, T** argv, auto&& error_handler)
    {
        using Err_handler_ret_type = decltype(error_handler(std::declval<cli::Error&&>()));

        using Result_type = std::conditional_t<
            std::same_as<Err_handler_ret_type, void>,
            std::variant<S_cli...>,
            std::variant<S_cli..., Err_handler_ret_type>
        >;

        auto cli_or_error = parse_(Type_list<S_cli...>{}, argc, argv);

        return utils::match(cli_or_error,
            [](auto& cli) -> Result_type {
                return std::move(cli);
            },
            [&error_handler](cli::Error& error) -> Result_type {
                if constexpr (std::same_as<Err_handler_ret_type, void>) {
                    const auto error_copy = error;
                    error_handler(error);

                    // User provided error_handler is supposed to throw exception as its return
                    // value type is void, however since we can't always check at compile-time
                    // if error_handler fulfills this requirement we need to somehow handle a
                    // situation wherein error_handler doens't throw an exception. The current
                    // strategy is to throw exception on our own.
                    throw error_copy;
                }
                else {
                    return error_handler(std::move(error));
                }
            }
        );
    }

private:
    template<typename T, typename C, typename... Rest_clis>
        requires std::same_as<std::remove_const_t<T>, char>
    static auto parse_(Type_list<C, Rest_clis...>,
                       std::integral auto argc, T** argv,
                       Multi_cli_error multi_error = {})
    {
        using Result_type = std::variant<S_cli..., cli::Error>;

        auto cli_or_error = C::parse(argc, argv, [](auto error){ return error; });

        return utils::match(cli_or_error,
            [](C& cli) -> Result_type {
                // Command line is successfully parsed, return an instance of Cli.
                return std::move(cli);
            },
            [argc, argv, &multi_error](auto& error) -> Result_type {
                // Try to parse cli according to the rest of cli specs.
                multi_error += std::move(error);
                return parse_(Type_list<Rest_clis...>{}, argc, argv, std::move(multi_error));
            }
        );
    }

    static auto parse_(Type_list<>, auto /*argc*/, auto /*argv*/, Multi_cli_error multi_error = {})
    {
        return std::variant<S_cli..., cli::Error>{multi_error};
    }
};

// TODO:
// - implement parameter_info  to provide enought info for exception throwing
// - eimplement throw exception or class
// - implement assignment operators
// - think about returning from lexer std::optional instead of -1
//
template<cli::api::Parameter... P>
class Cli<P...> {    
//    template<typename T>
//    using Is_positional_param = std::bool_constant<cli::Positional_parameter<T>>;

//    template<typename T>
//    using Is_named_parameter = std::negation<Is_positional_param<T>>;

//    using Named_params_tuple = utils::mp::Filter<Is_named_parameter, std::tuple<P...>>;
//    using Positional_params_tuple = utils::mp::Filter<Is_positional_param, std::tuple<P...>>;

    template<typename Tag, typename Value_type>
    struct Tagged_value {
        Value_type value;
    };

    template<cli::api::Parameter T>
    using Param_tagged_value = Tagged_value<T, cli::api::Parameter_value_t<T>>;

public:
    template<cli::details_::One_of<P...> T>
    constexpr auto& get() noexcept
    {
        return get_<Param_tagged_value<T>>(*this).value;
    }

    template<cli::details_::One_of<P...> T>
    constexpr const auto& get() const noexcept
    {
        return get_<Param_tagged_value<T>>(*this).value;
    }

    template<typename T>
        requires std::same_as<std::remove_const_t<T>, char>
    static constexpr auto parse(std::integral auto argc, T** argv)
    {
        return parse(argc, argv, [](auto error){
            throw error;
        });
    }

    template<typename T, typename H>
        requires std::same_as<std::remove_const_t<T>, char>
    static constexpr auto parse(std::integral auto argc, T** argv, H&& error_handler)
    {
//        using Err_handler_ret_type = decltype(error_handler(std::declval<cli::Error&&>()));

//        using Result_type = std::conditional_t<
//            std::same_as<Err_handler_ret_type, void>,
//            Cli,
//            std::variant<Cli, Err_handler_ret_type>
//        >;

        auto args =
            std::views::counted(argv, gsl::narrow_cast<std::ptrdiff_t>(argc))
            | std::views::drop(1)
            | std::views::transform([](auto arg){ return utils::C_string_view{arg}; });

        auto cli_or_error = parse_(argc, argv);

        return parse_(args, std::forward<H>(error_handler));
    }

private:
    static constexpr std::variant<Cli, cli::Error> parse_(std::ranges::view auto args)
    {
        std::tuple<std::optional<cli::api::Parameter_value_t<P>>...> arg_values;

        for (auto tok: tokenize<P...>(args)) {
            auto err = utils::match(tok,
                [&arg_values]<typename Param_type>(cli::details_::Token<Param_type> t) {
                    auto& v = std::get<std::optional<Param_type>>(arg_values);
                    if (v.has_value()) {
//                        if constexpr (!cli::Repeatable_parameter<Param_type>) {

//                        }
                    }
                },
                [](cli::Error error) {
                    return std::make_optional(error);
                }
            );

            if (err) {
                return err;
            }
        }
        return Cli{};
//            if (tok_id < named_arg_detached_dtrs.size()) {
//                auto argv_tail = std::ranges::subrange(std::next(p), e);

//                if (!named_arg_detached_dtrs[k].has_value()) {
//                    auto [skip, dtr] = init_named_arg_(named_param_index, lexeme_end, argv_tail);
//                    p += skip;
//                    named_arg_detached_dtrs[k] = std::move(dtr);
//                }
//                else {
//                    p += append_named_arg_(named_param_index, lexeme_end, argv_tail);
//                }
//            }
//            else if (positional_arg_idx < utils::to_signed(std::tuple_size_v<Positional_params_tuple>)) {
//                handle_positional_arg_(positional_arg_idx, *p);
//                ++positional_arg_idx;
//                ++p;
//            }
//            else {
//                return cli::Error::Unknown_parameter{std::distance(p, e)};
//            }

//        return Cli{};
    }
#if 0
    constexpr Cli(std::ranges::view auto args)
    try {
        std::array<std::optional<cli::details_::Detached_destructor>,
                   std::tuple_size_v<Named_params_tuple>> named_arg_detached_dtrs;

        auto positional_arg_idx = 0;

        // Set to true if '--' (two dashes) occurs in command line.
        bool treat_rest_as_positional_params = false;

        for (auto p = args.begin(), e = args.end(); p != e;) {
            assert(*p != nullptr);

            [[maybe_unused]] const auto old_p = p;

            using namespace std::string_view_literals;
            if (!treat_rest_as_positional_params && *p == "--"sv) {
                treat_rest_as_positional_params = true;
                continue;
            }

            if (!treat_rest_as_positional_params && **p == '-') {
                using Lexer = utils::mp::Apply<cli::details_::Lexer, Named_params_tuple>;

                auto [named_param_index, lexeme_end] = Lexer::recognize(*p);

                if (named_param_index == -1) {
                    throw cli::details_::Unknown_parameter{std::distance(p, e)};
                }

                const auto k = utils::to_unsigned(named_param_index);
                assert(k < named_arg_detached_dtrs.size());

                auto argv_tail = std::ranges::subrange(std::next(p), e);

                if (!named_arg_detached_dtrs[k].has_value()) {
                    auto [skip, dtr] = init_named_arg_(named_param_index, lexeme_end, argv_tail);
                    p += skip;
                    named_arg_detached_dtrs[k] = std::move(dtr);
                }
                else {
                    p += append_named_arg_(named_param_index, lexeme_end, argv_tail);
                }
            }
            else if (positional_arg_idx >= utils::to_signed(std::tuple_size_v<Positional_params_tuple>)) {
                throw_error_(args, p, "Too much positional parameters.");
            }
            else {
                handle_positional_arg_(positional_arg_idx, *p);
                ++p;
            }

            assert(old_p < p && "Ensure progress of parsing");
        }

        // Initialize unspecified parameters with default values (if parameter doesn't have
        // default value exception is thrown).
        for (std::size_t i = 0; i < named_arg_detached_dtrs.size(); ++i) {
            if (!named_arg_detached_dtrs[i]) {
                named_arg_detached_dtrs[i] = initialize_with_default_(i);
            }
        }
    }
    catch(ez::cli::details_::Unknown_parameter ex) {
        throw_error_(args, ex, "Unknown parameter");
    }
    catch(ez::cli::details_::Parameter_misuse ex) {
        throw_error_(args, ex, ex.problem);
    }

    constexpr auto init_named_arg_(std::integral auto named_param_index,
                                   utils::C_string_view arg_lexeme_end,
                                   std::ranges::view auto argv_tail)
    {
        auto set_value = []<typename... Args>(auto& v, Args&&... args) {
            return v.init_value(std::forward<Args>(args)...);
        };

        return cli::details_::apply_tuple_element(
            set_value, named_param_index, named_args_, arg_lexeme_end, argv_tail);
    }

    constexpr auto append_named_arg_(std::integral auto named_param_index,
                                     utils::C_string_view arg_lexeme_end,
                                     std::ranges::view auto argv_tail)
    {
        auto set_value = []<typename... Args>(auto& v, Args&&... args) {
            return v.append_value(std::forward<Args>(args)...);
        };

        return cli::details_::apply_tuple_element(
            set_value, named_param_index, named_args_, arg_lexeme_end, argv_tail);
    }

    constexpr auto initialize_with_default_(std::integral auto param_index)
    {
        auto init_with_default = [](auto& v) {
            return v.init_with_default();
        };

        return cli::details_::apply_tuple_element(init_with_default, param_index, named_args_);
    }

    constexpr void handle_positional_arg_(std::integral auto param_index, auto arg)
    {
        auto assign = [](auto& v, auto a) { return v.assign(a); };
        return cli::details_::apply_tuple_element(assign, param_index, positional_args_, *arg);
    }

    [[noreturn]] void throw_error_(std::ranges::view auto cli_args, auto /*argv_curr*/, std::string_view msg)
    {
        for (auto arg : cli_args) {
            std::cerr << arg << ' ';
        }
        std::cerr << '\n';
        std::cerr << msg << '\n';

        throw std::runtime_error{"Not implemented yet."};;
    }

    template<cli::details_::One_of<P...> T>
    static decltype(auto) get_(auto& this_cli)
    {
        return std::get<cli::details_::Parameter_value<T>>(this_cli.named_args_).value();
    }

    template<cli::details_::One_of<P...> T>
        requires cli::Positional_parameter<T>
    static decltype(auto) get_(auto& this_cli)
    {
        return std::get<cli::details_::Parameter_value<T>>(this_cli.positional_args_).value();
    }
#endif

private:
//    template<typename T>
//    struct To_parameter_value_ {
//        using type = cli::details_::Parameter_value<T>;
//    };

//    using Named_param_values_tuple = utils::mp::Transform<To_parameter_value_, Named_params_tuple>;
//    using Positional_param_values_tuple = utils::mp::Transform<To_parameter_value_, Positional_params_tuple>;

//    Named_param_values_tuple named_args_;
//    Positional_param_values_tuple positional_args_;
    std::tuple<Param_tagged_value<P>...> values_;
};


} // namespace ez

#endif // EZ_CLI_H
