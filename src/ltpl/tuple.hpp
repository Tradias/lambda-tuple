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
// A type that can be constructed from anything, useful for extracting the nth-element of a type list later.
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

// libc++ does not provide `std::equality_comparable<T>`.
template <class T, class U>
concept WeaklyEqualityComparableWith =
    requires(const std::remove_reference_t<T>& t, const std::remove_reference_t<U>& u) {
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

// Test that the types list does not contain just one element that is decay-equal to T.
template <class T, class, class...>
inline constexpr bool is_not_exactly_v = true;

template <class T, class U>
inline constexpr bool is_not_exactly_v<T, U> = !std::is_same_v<T, std::remove_cvref_t<U>>;

// Standard-library compatible tests for the converting copy/move constructor of this Tuple.
template <class T, class U>
concept TupleCopyConversion = std::is_same_v<T, U> || std::is_constructible_v<T, const ltpl::Tuple<U>&> ||
                              std::is_convertible_v<const ltpl::Tuple<U>&, T>;

template <class, class...>
inline constexpr bool is_converting_copy_constructor_v = true;

template <class T, class U>
inline constexpr bool is_converting_copy_constructor_v<ltpl::Tuple<T>, U> = !TupleCopyConversion<T, U>;

template <class T, class U>
concept TupleMoveConversion =
    std::is_same_v<T, U> || std::is_constructible_v<T, ltpl::Tuple<U>> || std::is_convertible_v<ltpl::Tuple<U>, T>;

template <class, class...>
inline constexpr bool is_converting_move_constructor_v = true;

template <class T, class U>
inline constexpr bool is_converting_move_constructor_v<ltpl::Tuple<T>, U> = !TupleMoveConversion<T, U>;

// Since we can only capture all variadic arguments in a lambda by value or by reference we decide to capture them
// by-value and wrap references into this class. T is either an lvalue or rvalue reference.
template <class T>
class RefWrapper
{
  public:
    using Value = std::remove_cvref_t<T>;
    using TRef = std::remove_reference_t<T>&;

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

    // This allows us to treat Tuple<int> and Tuple<int&> with the same static_cast without having to explicitly unwrap
    // this RefWrapper first, e.g.:
    // static_cast<int&>(get<0>(Tuple<int>())) -> int&
    // static_cast<int&>(/*RefWrapper<int&>*/ get<0>(Tuple<int&>())) -> int&
    constexpr explicit operator TRef() const noexcept { return static_cast<TRef>(v); }

    constexpr explicit operator Value&&() const noexcept { return static_cast<Value&&>(v); }

  private:
    T v;
};

template <class T>
struct Unwrap;

// For elements stored by-value in the Tuple, get<I> will return an lvalue reference that we unwrap here.
template <class T>
struct Unwrap<T&>
{
    using Type = T;
};

// For elements stored by-reference in the Tuple, get<I> will return an lvalue reference to a RefWrapper that we unwrap
// here. We retain the T& or T&& nature of the type, so that tuple_element provides the correct type.
template <class T>
struct Unwrap<RefWrapper<T>&>
{
    using Type = T;
};

template <class T>
using UnwrapT = typename Unwrap<T>::Type;

// If the Tuple stores the element by-value then forward arguments from the constructor as rvalue references.
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

// If the Tuple stores the element by lvalue reference then wrap it into a RefWrapper<T&>.
template <class T>
struct Wrap<T&>
{
    using Type = RefWrapper<T&>;

    static constexpr Type wrap(T& v) noexcept { return Type(v); }
};

// If the Tuple stores the element by rvalue reference then wrap it into a RefWrapper<T&&>.
template <class T>
struct Wrap<T&&>
{
    using Type = RefWrapper<T&&>;

    static constexpr Type wrap(T&& v) noexcept { return Type(static_cast<T&&>(v)); }
};

template <class T>
using WrapT = typename Wrap<T>::Type;

// The wrapping is necessary to ensure that Tuple<T...> instantiates this function only once, independent from the
// arguments passed to its constructor.
template <class... T>
constexpr auto make_lambda(WrapT<T>... v)
{
    // This lambda is the storage type of the Tuple. The argument `f` is used to access elements.
    return [... v = static_cast<WrapT<T>&&>(v)](auto f) mutable -> decltype(auto)
    {
        return f(v...);
    };
}

// An implementation of nth-element similar to the `Concept expansion` described by Kris Jusiak in his talk `The Nth
// Element: A Case Study - CppNow 2022` but compatible with every C++20 compiler and easily backportable to C++14.
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
    // An empty Tuple is trivial.
    Tuple() = default;

    Tuple(const Tuple&) = default;

    Tuple(Tuple&&) = default;

    ~Tuple() = default;

    // Non-empty Tuple, default construct all elements.
    constexpr Tuple()  //
        noexcept(std::conjunction_v<std::conjunction<std::is_nothrow_default_constructible<T>,
                                                     std::is_nothrow_move_constructible<T>>...>)  //
    requires(sizeof...(T) > 0)
        : lambda(detail::make_lambda<T...>(T()...))
    {
    }

    // Forwarding-constructor. Note that perfect forwarding is not possible because the template parameter of
    // `make_lambda` must not depend on the arguments passed to this function, otherwise a different type of lambda
    // would be returned.
    // We ensure that this constructor does not hide the default copy/move constructors through `is_not_exaclty_v`.
    // If every element of the Tuple can be implicitly constructed from the arguments then this constructor is also
    // implicit.
    template <class... U>
    constexpr explicit(!std::conjunction_v<std::is_convertible<U, T>...>)     //
        Tuple(U&&... v)                                                       //
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T, U>...>)  //
    requires(sizeof...(T) == sizeof...(U) && sizeof...(T) >= 1 && std::conjunction_v<std::is_constructible<T, U>...> &&
             detail::is_not_exactly_v<Tuple, U...>)
        : lambda(detail::make_lambda<T...>(detail::Wrap<T>::wrap(static_cast<U&&>(v))...))
    {
    }

    // Converting copy constructor
    template <class... U>
    constexpr explicit(!std::conjunction_v<std::is_convertible<const U&, T>...>)     //
        Tuple(const Tuple<U...>& other)                                              //
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T, const U&>...>)  //
    requires(sizeof...(T) == sizeof...(U) && std::conjunction_v<std::is_constructible<T, const U&>...> &&
             detail::is_converting_copy_constructor_v<Tuple, U...>)
        // We have to const_cast because the lambda's operator() is mutable. But since we cast each element to const& no
        // UB can occur.
        : lambda(const_cast<Tuple<U...>&>(other).lambda(
              [](const detail::WrapT<U>&... v_other)
              {
                  return detail::make_lambda<T...>(detail::Wrap<T>::wrap(static_cast<const U&>(v_other))...);
              }))
    {
    }

    // Converting move constructor
    template <class... U>
    constexpr explicit(!std::conjunction_v<std::is_convertible<U, T>...>)            //
        Tuple(Tuple<U...>&& other)                                                   //
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T, const U&>...>)  //
    requires(sizeof...(T) == sizeof...(U) && std::conjunction_v<std::is_constructible<T, U>...> &&
             detail::is_converting_move_constructor_v<Tuple, U...>)
        : lambda(other.lambda(
              [](detail::WrapT<U>&... v_other)
              {
                  return detail::make_lambda<T...>(detail::Wrap<T>::wrap(static_cast<U&&>(v_other))...);
              }))
    {
    }

    // An empty Tuple is trivial.
    Tuple& operator=(const Tuple& other)
    requires(sizeof...(T) == 0)
    = default;

    Tuple& operator=(Tuple&& other)
    requires(sizeof...(T) == 0)
    = default;

    // Converting copy-assignment operator.
    template <class... U>
    constexpr Tuple& operator=(const Tuple<U...>& other)                           //
        noexcept(std::conjunction_v<std::is_nothrow_assignable<T&, const U&>...>)  //
    requires(sizeof...(T) == sizeof...(U) && std::conjunction_v<std::is_assignable<T&, const U&>...>)
    {
        lambda(
            [&other](detail::WrapT<T>&... t)
            {
                const_cast<Tuple<U...>&>(other).lambda(
                    [&](const detail::WrapT<U>&... v_other)
                    {
                        // Thanks to the assignment and conversion operators of RefWrapper, we can treat by-value
                        // elements just like RefWrapped elements.
                        (void(t = static_cast<const U&>(v_other)), ...);
                    });
            });
        return *this;
    }

    // Converting move-assignment operator.
    template <class... U>
    constexpr Tuple& operator=(Tuple<U...>&& other)                         //
        noexcept(std::conjunction_v<std::is_nothrow_assignable<T&, U>...>)  //
    requires(sizeof...(T) == sizeof...(U) && std::conjunction_v<std::is_assignable<T&, U>...>)
    {
        lambda(
            [&other](detail::WrapT<T>&... t)
            {
                other.lambda(
                    [&](detail::WrapT<U>&... v_other)
                    {
                        (void(t = static_cast<U&&>(v_other)), ...);
                    });
            });
        return *this;
    }

    // This comparison operator is SFINAE friendly, which is not required by the C++20 standard.
    template <class... U>
    [[nodiscard]] friend constexpr bool operator==(const Tuple& lhs, const Tuple<U...>& rhs)  //
    requires(sizeof...(T) == sizeof...(U) && (true && ... && detail::WeaklyEqualityComparableWith<T, U>))
    {
        return const_cast<Tuple&>(lhs).lambda(
            [&rhs](const detail::WrapT<T>&... v_lhs)
            {
                return detail::Access::lambda(const_cast<Tuple<U...>&>(rhs))(
                    [&](const detail::WrapT<U>&... v_rhs)
                    {
                        return (true && ... && (static_cast<const T&>(v_lhs) == static_cast<const U&>(v_rhs)));
                    });
            });
    }

    template <class... U>
    friend constexpr void swap(Tuple& lhs, Tuple& rhs)                 //
        noexcept(std::conjunction_v<std::is_nothrow_swappable<T>...>)  //
    requires(std::conjunction_v<std::is_swappable<T>...>)
    {
        return lhs.lambda(
            [&rhs](detail::WrapT<T>&... v_lhs)
            {
                return rhs.lambda(
                    [&](detail::WrapT<T>&... v_rhs)
                    {
                        using std::swap;
                        (void(swap(static_cast<T&>(v_lhs), static_cast<T&>(v_rhs))), ...);
                    });
            });
    }

    // C++23 overload of swap.
    template <class... U>
    friend constexpr void swap(const Tuple& lhs, const Tuple& rhs)           //
        noexcept(std::conjunction_v<std::is_nothrow_swappable<const T>...>)  //
    requires(std::conjunction_v<std::is_swappable<const T>...>)
    {
        return const_cast<Tuple&>(lhs).lambda(
            [&rhs](const detail::WrapT<T>&... v_lhs)
            {
                return const_cast<Tuple&>(rhs).lambda(
                    [&](const detail::WrapT<T>&... v_rhs)
                    {
                        using std::swap;
                        (void(swap(static_cast<const T&>(v_lhs), static_cast<const T&>(v_rhs))), ...);
                    });
            });
    }

  private:
    friend detail::Access;

    template <class...>
    friend class ltpl::Tuple;

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
constexpr Tuple<typename std::unwrap_ref_decay<T>::type...> make_tuple(T&&... v)
{
    return Tuple<typename std::unwrap_ref_decay<T>::type...>(static_cast<T&&>(v)...);
}

template <class... T>
[[nodiscard]] constexpr Tuple<T&...> tie(T&... v) noexcept
{
    return Tuple<T&...>(v...);
}

template <class... T>
[[nodiscard]] constexpr Tuple<T&&...> forward_as_tuple(T&&... v) noexcept
{
    return Tuple<T&&...>(static_cast<T&&>(v)...);
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
