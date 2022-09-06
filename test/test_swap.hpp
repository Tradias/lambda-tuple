// Copyright (c) 2022 Dennis Hezel
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef LTPL_TEST_TEST_SWAP_HPP
#define LTPL_TEST_TEST_SWAP_HPP

#include <ltpl/tuple.hpp>
#include <test/factory.hpp>
#include <test/framework.hpp>
#include <test/utility.hpp>

namespace test
{
template <class T>
void test_swap_value()
{
    std::tuple<TupleT<T, MoveOnly>> tuple(1);
    std::tuple<TupleT<T, MoveOnly>> tuple2(2);
    std::swap(tuple, tuple2);
    CHECK(T::template get<0>(std::get<0>(tuple)).is_swapped);
    CHECK(T::template get<0>(std::get<0>(tuple2)).is_swapped);
}

template <class T>
void test_swap_lref()
{
    MoveOnly c{1};
    std::tuple<TupleT<T, MoveOnly&>> tuple(c);
    MoveOnly c2{1};
    std::tuple<TupleT<T, MoveOnly&>> tuple2(c2);
    std::swap(tuple, tuple2);
    CHECK(c.is_swapped);
    CHECK(c2.is_swapped);
}

template <class T>
void test_swap_const_lref()
{
    CopyOnly c{1};
    const ltpl::Tuple<CopyOnly&> tuple(c);
    CopyOnly c2{1};
    const ltpl::Tuple<CopyOnly&> tuple2(c2);
    swap(tuple, tuple2);
}

template <class T>
void test_unswappable_const_lref()
{
    CHECK_FALSE(std::is_swappable_v<TupleT<T, const int&>>);
}

template <class T>
void test_unswappable_immovable()
{
    CHECK_FALSE(std::is_swappable_v<TupleT<T, Immovable&>>);
}
}  // namespace test

#endif  // LTPL_TEST_TEST_SWAP_HPP
