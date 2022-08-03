// Copyright (c) 2022 Dennis Hezel
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef LTPL_TEST_FACTORY_HPP
#define LTPL_TEST_FACTORY_HPP

#include <ltpl/tuple.hpp>

#include <tuple>

namespace test
{
struct LambdaTuple
{
    template <std::size_t I, class T>
    static constexpr decltype(auto) get(T&& t) noexcept(noexcept(ltpl::get<I>(std::forward<T>(t))))
    {
        return ltpl::get<I>(std::forward<T>(t));
    }
};

struct StdTuple
{
    template <std::size_t I, class T>
    static constexpr decltype(auto) get(T&& t) noexcept(noexcept(std::get<I>(std::forward<T>(t))))
    {
        return std::get<I>(std::forward<T>(t));
    }
};

template <class>
struct TupleType
{
    template <class... T>
    using Type = ltpl::Tuple<T...>;
};

template <>
struct TupleType<StdTuple>
{
    template <class... T>
    using Type = std::tuple<T...>;
};

template <class U, class... T>
using TupleT = typename TupleType<U>::template Type<T...>;
}  // namespace test

#endif  // LTPL_TEST_FACTORY_HPP
