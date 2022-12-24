namespace ez::cli::details_ {

#if 0
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

struct Unknown_parameter {
    std::ptrdiff_t argv_tail_size;
};

struct Parameter_misuse {
    std::string problem;
};

struct Parameter_value_init_error {};


template<api::Parameter P>
struct Algo;

template<api::Named_parameter_without_value P>
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
#endif
} // namespace ez::cli::details_

#if 0
TEST_CASE("tuple_cat_view")
{
    SECTION("concat with xvalue tuple of all refs") {
        int i = 3;
        std::tuple<int&> k{i};
        [[maybe_unused]]
        auto ttt = ez::cli::details_::tuple_cat_view(std::move(k));
        REQUIRE(&std::get<0>(ttt) == &i);
    }

    SECTION("concat with lvalue tuple without refs") {
        int i = 3;
        std::tuple<int> k{i};
        [[maybe_unused]]
        auto ttt = ez::cli::details_::tuple_cat_view(k);
        //    auto ttt = ez::cli::details_::tuple_cat_view(std::move(k)); // Shouldn't compile
        REQUIRE(&std::get<0>(ttt) == &std::get<0>(k));
    }

    SECTION("mix concat lvalue, xvalue tuples") {
        int i = 3;
        int k = -23;
        std::tuple<int> a{i};
        std::tuple<int&> b{k};
        [[maybe_unused]]
        auto ttt = ez::cli::details_::tuple_cat_view(a, b, std::move(b));
        //    auto ttt = ez::cli::details_::tuple_cat_view(std::move(k)); // Shouldn't compile
        REQUIRE(ttt == std::tie(i, k, k));
        REQUIRE(&std::get<0>(ttt) == &std::get<0>(a));
        REQUIRE(&std::get<1>(ttt) == &k);
        REQUIRE(&std::get<2>(ttt) == &k);
    }
}


TEST_CASE("tuple_zip")
{
    char i = 'd';
    std::tuple<int, char&> x{3, i};
    std::tuple<double, float> y;
    auto tt = ez::cli::details_::tuple_zip(x,y);
    STATIC_REQUIRE(std::is_same_v<decltype(tt), std::tuple<std::tuple<int, double>, std::tuple<char&, float>>>);
    REQUIRE(std::get<0>(std::get<0>(tt)) == 3);
    REQUIRE(&std::get<0>(std::get<1>(tt)) == &i);
}
#endif
