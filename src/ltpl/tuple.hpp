// Copyright (c) 2022 Dennis Hezel
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef LTPL_LTPL_TUPLE_HPP
#define LTPL_LTPL_TUPLE_HPP

#include <cstddef>
#include <type_traits>
#include <utility>

namespace ltpl
{
template <class... T>
class Tuple;

namespace detail
{
template <std::size_t>
struct Anything
{
    template <class T>
    constexpr Anything(T&&) noexcept
    {
    }
};

struct Access
{
    template <class... T>
    static constexpr auto& lambda(ltpl::Tuple<T...>& tuple) noexcept
    {
        return tuple.lambda;
    }
};

template <class T, class U>
concept ConvertibleTo = std::is_convertible_v<T, U>;

template <class T, class U>
concept EqualityComparableWith = requires(const std::remove_reference_t<T>& t, const std::remove_reference_t<U>& u)
{
    {
        t == u
        } -> ConvertibleTo<bool>;
    {
        t != u
        } -> ConvertibleTo<bool>;
    {
        u == t
        } -> ConvertibleTo<bool>;
    {
        u != t
        } -> ConvertibleTo<bool>;
};

template <class, class, class...>
inline constexpr bool is_not_exactly_v = true;

template <class T, class U>
inline constexpr bool is_not_exactly_v<T, U> = !std::is_same_v<T, std::remove_cvref_t<U>>;

template <class T, class U>
concept ConvertingCopyConstructor = std::is_same_v<T, U> || std::is_constructible_v < T,
        const ltpl::Tuple<U>
& > || std::is_convertible_v<const ltpl::Tuple<U>&, T>;

template <class, class...>
inline constexpr bool is_converting_copy_constructor_v = true;

template <class T, class U>
inline constexpr bool is_converting_copy_constructor_v<ltpl::Tuple<T>, U> = !ConvertingCopyConstructor<T, U>;

template <class T, class U>
concept ConvertingMoveConstructor =
    std::is_same_v<T, U> || std::is_constructible_v<T, ltpl::Tuple<U>> || std::is_convertible_v<ltpl::Tuple<U>, T>;

template <class, class...>
inline constexpr bool is_converting_move_constructor_v = true;

template <class T, class U>
inline constexpr bool is_converting_move_constructor_v<ltpl::Tuple<T>, U> = !ConvertingMoveConstructor<T, U>;

template <class T>
struct RefWrapper
{
    using Value = std::remove_cvref_t<T>;
    using TRef = std::remove_reference_t<T>&;

    T v;

    constexpr explicit RefWrapper(T v) noexcept : v(static_cast<T>(v)) {}

    RefWrapper(const RefWrapper&) = default;

    RefWrapper(RefWrapper&&) = default;

    ~RefWrapper() = default;

    RefWrapper& operator=(const Value& other)
    {
        v = other;
        return *this;
    }

    RefWrapper& operator=(Value&& other)
    {
        v = static_cast<Value&&>(other);
        return *this;
    }

    constexpr explicit operator TRef() const noexcept { return static_cast<TRef>(v); }

    constexpr explicit operator Value&&() const noexcept { return static_cast<Value&&>(v); }
};

template <class T>
struct Unwrap;

template <class T>
struct Unwrap<T&>
{
    using Type = T;
};

template <class T>
struct Unwrap<RefWrapper<T>&>
{
    using Type = T;
};

template <class T>
using UnwrapT = typename Unwrap<T>::Type;

template <class T>
struct Wrap
{
    using Type = T&&;

    template <class U>
    static constexpr T wrap(U&& v)
    {
        return T(static_cast<U&&>(v));
    }

    static constexpr Type wrap(Type v) noexcept { return static_cast<Type>(v); }
};

template <class T>
struct Wrap<const T> : Wrap<T>
{
};

template <class T>
struct Wrap<T&>
{
    using Type = RefWrapper<T&>;

    static constexpr Type wrap(T& v) noexcept { return Type(v); }
};

template <class T>
struct Wrap<T&&>
{
    using Type = RefWrapper<T&&>;

    static constexpr Type wrap(T&& v) noexcept { return Type(static_cast<T&&>(v)); }
};

template <class T>
using WrapT = typename Wrap<T>::Type;

template <class... T>
constexpr auto make_lambda(WrapT<T>... v)
{
    return [... v = static_cast<WrapT<T>&&>(v)](auto f) mutable -> decltype(auto)
    {
        return f(v...);
    };
}

template <std::size_t I, class... T>
constexpr decltype(auto) get(ltpl::Tuple<T...>& tuple) noexcept
{
    return [&]<std::size_t... Ns>(std::index_sequence<Ns...>)->decltype(auto)
    {
        return Access::lambda(tuple)(
            []<class Nth>(Anything<Ns>..., Nth& nth, auto&...) -> Nth&
            {
                return nth;
            });
    }
    (std::make_index_sequence<I>{});
}
}  // namespace detail

template <class... T>
class Tuple
{
  private:
    using Lambda = decltype(detail::make_lambda<T...>(std::declval<detail::WrapT<T>>()...));

  public:
    Tuple() = default;

    Tuple(const Tuple&) = default;

    Tuple(Tuple&&) = default;

    ~Tuple() = default;

    constexpr Tuple()  //
        noexcept(std::conjunction_v<std::conjunction<std::is_nothrow_default_constructible<T>,
                                                     std::is_nothrow_move_constructible<T>>...>)  //
        requires(sizeof...(T) > 0)
        : lambda(detail::make_lambda<T...>(T()...))
    {
    }

    template <class... U>
    constexpr explicit(!std::conjunction_v<std::is_convertible<U, T>...>)     //
        Tuple(U&&... v)                                                       //
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T, U>...>)  //
        requires(sizeof...(T) == sizeof...(U) && sizeof...(T) >= 1 &&
                 std::conjunction_v<std::is_constructible<T, U>...> && detail::is_not_exactly_v<Tuple, U...>)
        : lambda(detail::make_lambda<T...>(detail::Wrap<T>::wrap(std::forward<U>(v))...))
    {
    }

    template <class... U>
    constexpr explicit(!std::conjunction_v<std::is_convertible<const U&, T>...>)     //
        Tuple(const Tuple<U...>& other)                                              //
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T, const U&>...>)  //
        requires(sizeof...(T) == sizeof...(U) && std::conjunction_v<std::is_constructible<T, const U&>...> &&
                 detail::is_converting_copy_constructor_v<Tuple, U...>)
        : lambda(const_cast<Tuple<U...>&>(other).lambda(
              [&](const detail::WrapT<U>&... v_other)
              {
                  return detail::make_lambda<T...>(detail::Wrap<T>::wrap(static_cast<const U&>(v_other))...);
              }))
    {
    }

    template <class... U>
    constexpr explicit(!std::conjunction_v<std::is_convertible<U, T>...>)            //
        Tuple(Tuple<U...>&& other)                                                   //
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T, const U&>...>)  //
        requires(sizeof...(T) == sizeof...(U) && std::conjunction_v<std::is_constructible<T, U>...> &&
                 detail::is_converting_move_constructor_v<Tuple, U...>)
        : lambda(other.lambda(
              [&](detail::WrapT<U>&... v_other)
              {
                  return detail::make_lambda<T...>(detail::Wrap<T>::wrap(static_cast<U&&>(v_other))...);
              }))
    {
    }

    Tuple& operator=(const Tuple& other) requires(sizeof...(T) == 0) = default;

    Tuple& operator=(Tuple&& other) requires(sizeof...(T) == 0) = default;

    template <class... U>
    constexpr Tuple& operator=(const Tuple<U...>& other)                           //
        noexcept(std::conjunction_v<std::is_nothrow_assignable<T&, const U&>...>)  //
        requires(sizeof...(T) == sizeof...(U) && std::conjunction_v<std::is_assignable<T&, const U&>...>)
    {
        lambda(
            [&](detail::WrapT<T>&... t)
            {
                const_cast<Tuple<U...>&>(other).lambda(
                    [&](const detail::WrapT<U>&... v_other)
                    {
                        (void(t = static_cast<const U&>(v_other)), ...);
                    });
            });
        return *this;
    }

    template <class... U>
    constexpr Tuple& operator=(Tuple<U...>&& other)                         //
        noexcept(std::conjunction_v<std::is_nothrow_assignable<T&, U>...>)  //
        requires(sizeof...(T) == sizeof...(U) && std::conjunction_v<std::is_assignable<T&, U>...>)
    {
        lambda(
            [&](detail::WrapT<T>&... t)
            {
                other.lambda(
                    [&](detail::WrapT<U>&... v_other)
                    {
                        (void(t = static_cast<U&&>(v_other)), ...);
                    });
            });
        return *this;
    }

    template <class... U>
    [[nodiscard]] friend constexpr bool operator==(const Tuple& lhs, const Tuple<U...>& rhs)  //
        requires(sizeof...(T) == sizeof...(U) && (true && ... && detail::EqualityComparableWith<T, U>))
    {
        return const_cast<Tuple&>(lhs).lambda(
            [&](const detail::WrapT<T>&... v_lhs)
            {
                return detail::Access::lambda(const_cast<Tuple<U...>&>(rhs))(
                    [&](const detail::WrapT<U>&... v_rhs)
                    {
                        return (true && ... && (static_cast<const T&>(v_lhs) == static_cast<const U&>(v_rhs)));
                    });
            });
    }

  private:
    friend detail::Access;

    template <class...>
    friend class Tuple;

    Lambda lambda;
};

template <class... T>
Tuple(T...) -> Tuple<T...>;

template <std::size_t I, class... T>
[[nodiscard]] constexpr std::tuple_element_t<I, Tuple<T...>>& get(Tuple<T...>& tuple) noexcept
{
    auto& v = detail::get<I>(tuple);
    return static_cast<detail::UnwrapT<decltype(v)>&>(v);
}

template <std::size_t I, class... T>
[[nodiscard]] constexpr const std::tuple_element_t<I, Tuple<T...>>& get(const Tuple<T...>& tuple) noexcept
{
    auto& v = detail::get<I>(const_cast<Tuple<T...>&>(tuple));
    return static_cast<const detail::UnwrapT<decltype(v)>&>(v);
}

template <std::size_t I, class... T>
[[nodiscard]] constexpr std::tuple_element_t<I, Tuple<T...>>&& get(Tuple<T...>&& tuple) noexcept
{
    auto& v = detail::get<I>(tuple);
    return static_cast<detail::UnwrapT<decltype(v)>&&>(v);
}

template <std::size_t I, class... T>
[[nodiscard]] constexpr const std::tuple_element_t<I, Tuple<T...>>&& get(const Tuple<T...>&& tuple) noexcept
{
    auto& v = detail::get<I>(const_cast<Tuple<T...>&>(tuple));
    return static_cast<const detail::UnwrapT<decltype(v)>&&>(v);
}

template <class... T>
constexpr Tuple<std::unwrap_ref_decay_t<T>...> make_tuple(T&&... v)
{
    return Tuple<std::unwrap_ref_decay_t<T>...>(std::forward<T>(v)...);
}

template <class... T>
[[nodiscard]] constexpr Tuple<T&...> tie(T&... v) noexcept
{
    return Tuple<T&...>(v...);
}

template <class... T>
[[nodiscard]] constexpr Tuple<T&&...> forward_as_tuple(T&&... v) noexcept
{
    return Tuple<T&&...>(std::forward<T>(v)...);
}
}  // namespace ltpl

template <std::size_t I, class... T>
struct std::tuple_element<I, ltpl::Tuple<T...>>
{
    using type = ltpl::detail::UnwrapT<decltype(ltpl::detail::get<I>(std::declval<ltpl::Tuple<T...>&>()))>;
};

template <class... T>
struct std::tuple_size<ltpl::Tuple<T...>> : std::integral_constant<std::size_t, sizeof...(T)>
{
};

#endif  // LTPL_LTPL_TUPLE_HPP
