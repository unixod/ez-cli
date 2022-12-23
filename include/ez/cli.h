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


namespace ez::cli::details_ {

template<typename T, typename... Option>
concept One_of = (std::same_as<T, Option> || ...);

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

struct Unknown_parameter {
    std::ptrdiff_t argv_tail_size;
};

struct Parameter_misuse {
    std::string problem;
};

struct Parameter_value_init_error {};


template<Parameter P>
struct Algo;

template<Named_parameter_without_value P>
struct Algo<P> {
    /// Return a number of elements to skip from the begining of given @a argv_tail.
    static auto pop_arg(utils::C_string_view arg_lexeme_end, std::ranges::view auto argv_tail) {
        // Detect a situation when a flag was given a value (i.e. -a=123 while -a
        // is flag not a named parameter).
        if (!arg_lexeme_end.empty()) {
            throw cli::details_::Unknown_parameter{std::ranges::ssize(argv_tail)};
        }

        return std::pair{1, ""};
    }
};

template<Named_parameter_with_value P>
struct Algo<P> {
    /// Return a number of elements to skip from the begining of given @a argv_tail.
    static auto pop_arg(utils::C_string_view arg_lexeme_end, std::ranges::view auto argv_tail)
    {   
        if (arg_lexeme_end.empty()) { // if --param arg
            if (argv_tail.empty()) {
                throw cli::details_::Parameter_misuse{"Parameter requires argument."};
            }

            return std::pair{2, argv_tail[0]};
        }
        else if (arg_lexeme_end[0] == '=') { // if --param=arg
            return std::pair{1, std::data(arg_lexeme_end) + 1};
        }

        throw cli::details_::Unknown_parameter{std::ranges::ssize(argv_tail)};
    }
};

template<Positional_parameter P>
struct Algo<P> {
};

template<Parameter P>
class Parameter_value {
public:
    using Parameter_type = P;

public:
    template<typename T>
    auto gon(T&& t)
    {
        return value_.construct(std::forward<T>(t));
    }

    /// Return a number of elements to skip from the begining of given @a argv_tail.
    auto init_value(auto... args)
    {
        auto [skip, arg] = Algo<P>::pop_arg(args...);

        if constexpr (Has_value_parser<P>) {
            return std::pair{skip, value_.construct(P::value(arg))};
        }
        else {
            return std::pair{skip, value_.construct(P::value())};
        }

        support::std23::unreachable();
    }

    /// Return a number of elements to skip from the begining of given @a argv_tail.
    int append_value(auto... args)
        requires (!Positional_parameter<P>)
    {
        if constexpr (Has_append_value<P>) {
            auto [skip, arg] = Algo<P>::pop_arg(args...);
            P::append_value(value_.get(), arg);
            return skip;
        }

        throw Parameter_misuse{"The parameter doesn't allow repetition."};
    }

    auto init_with_default() -> Detached_destructor
        requires (!Positional_parameter<P>)
    {
        if constexpr (details_::Has_default_value<P>) {
            return value_.construct(P::default_value());
        }

        throw Parameter_misuse{"The required parameter isn't specified"};
    }

    auto& value() {
        return value_.get();
    }

    auto& value() const {
        return value_.get();
    }

    void destruct_value() noexcept
    {
        value_.destruct();
    }

private:
    using Value = std::conditional_t<std::is_trivial_v<Parameter_value_type<P>>,
        Uninitialized_like<Parameter_value_type<P>>,
        Uninitialized<Parameter_value_type<P>>
    >;

    Value value_;
};


///// Find index of first occurance of T in List...
/////
///// Example:
///// @code
/////   constexpr int_index = first_index_of<int, double, int, char>;
/////   static_assert(ind_index == 1); // OK
/////
/////   // Foo isn't within list of types
/////   constexpr foo_index = first_index_of<Foo, double, int, char>; // FAIL
///// @endcode
//template<typename T, typename... List>
//constexpr auto first_index_of = []() consteval {
//    static_assert((std::is_same_v<T, List> || ...), "Looked up type (T) must be in search list (List...)");
//    constexpr auto index = std::array{std::is_same_v<T, List>...};
//    constexpr auto filter = std::views::take_while([](auto elt){return elt == false;});
//    return std::ranges::distance(index | filter);
//}();

// tuple_cat_view
template<typename Tuple>
struct Is_tuple_of_refs
    : Is_tuple_of_refs<std::remove_cvref_t<Tuple>> {};

template<typename... T>
struct Is_tuple_of_refs<std::tuple<T...>>
    : std::conjunction<std::is_reference<T>...> {};

template<typename Tuple>
concept Lvalue_or_tuple_of_refs =
    std::is_lvalue_reference_v<Tuple> ||
    Is_tuple_of_refs<std::remove_cvref_t<Tuple>>::value;

[[nodiscard]] constexpr auto tuple_cat_view(Lvalue_or_tuple_of_refs auto&&... tuple)
{
    auto to_tuple_of_refs = []<typename... T>(T&&... a) {
        return std::forward_as_tuple(std::forward<T>(a)...);
    };

    return std::tuple_cat(std::apply(to_tuple_of_refs, tuple)...);
}


template<typename Tuple, typename... Rest_tuples>
    requires (
        (std::tuple_size_v<std::remove_cvref_t<Tuple>> ==
            std::tuple_size_v<std::remove_cvref_t<Rest_tuples>>
        ) && ...
    )
[[nodiscard]] constexpr auto tuple_zip(Tuple&& first, Rest_tuples&&... rest)
{
    auto ith_elt_cat = []<auto i, typename... T>(std::index_sequence<i>, T&&... tuples) {
        using R = std::tuple<std::tuple_element_t<i, std::remove_cvref_t<T>>...>;
        return R{std::get<i>(std::forward<T>(tuples))...};
    };

    auto zip = [ith_elt_cat]<auto... i, typename... T>(std::index_sequence<i...>, T&&... tuples) {
        return std::make_tuple(ith_elt_cat(std::index_sequence<i>{}, std::forward<T>(tuples)...)...);
    };

    constexpr auto tuple_elt_idices =
        std::make_index_sequence<std::tuple_size_v<std::remove_cvref_t<Tuple>>>{};

    return zip(tuple_elt_idices, std::forward<Tuple>(first), std::forward<Rest_tuples>(rest)...);
}

} // namespace ez::cli::details_




template<typename, auto...>
struct Show;

namespace ez {

template<typename...>
class Cli;

namespace details_ {
template<typename>
struct Single_version_cli : std::false_type {};

template<cli::Parameter... P>
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
template<cli::Parameter... P>
class Cli<P...> {    
    template<typename T>
    using Is_positional_param = std::bool_constant<cli::Positional_parameter<T>>;

    template<typename T>
    using Is_named_parameter = std::negation<Is_positional_param<T>>;

    using Named_params_tuple = utils::mp::Filter<Is_named_parameter, std::tuple<P...>>;
    using Positional_params_tuple = utils::mp::Filter<Is_positional_param, std::tuple<P...>>;

public:
    template<cli::details_::One_of<P...> T>
    constexpr decltype(auto) get()
    {
        return get_<T>(*this);
    }

    template<cli::details_::One_of<P...> T>
    constexpr decltype(auto) get() const
    {
        return get_<T>(*this);
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
        std::tuple<std::optional<cli::Parameter_value_type<P>>...> arg_values;

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

        return Cli{};
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
#endif
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

private:
    template<typename T>
    struct To_parameter_value_ {
        using type = cli::details_::Parameter_value<T>;
    };

    using Named_param_values_tuple = utils::mp::Transform<To_parameter_value_, Named_params_tuple>;
    using Positional_param_values_tuple = utils::mp::Transform<To_parameter_value_, Positional_params_tuple>;

    Named_param_values_tuple named_args_;
    Positional_param_values_tuple positional_args_;
};


} // namespace ez

#endif // EZ_CLI_H
