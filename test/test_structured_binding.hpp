// Copyright (c) 2022 Dennis Hezel
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef LTPL_TEST_TEST_STRUCTURED_BINDING_HPP
#define LTPL_TEST_TEST_STRUCTURED_BINDING_HPP

#include <ltpl/tuple.hpp>
#include <test/factory.hpp>
#include <test/framework.hpp>
#include <test/utility.hpp>

namespace test
{
template <class T>
void test_value_structured_binding()
{
    TupleT<T, int, double> tuple{42, 12.};
    auto [a, b] = tuple;
    a = 1;
    CHECK_EQ(42, T::template get<0>(tuple));
}

template <class T>
void test_lref_structured_binding()
{
    TupleT<T, int, double> tuple{42, 12.};
    auto& [a, b] = tuple;
    b = 10.;
    CHECK_EQ(10., b);
}

template <class T>
void test_lef_structured_binding_from_const()
{
    Immovable c{1};
    TupleT<T, Immovable&, double> tuple{c, 12.};
    auto& [a, b] = std::as_const(tuple);
    a.v = 12;
    CHECK_EQ(12, a.v);
    CHECK(std::is_same_v<Immovable&, decltype(a)>);
    CHECK(std::is_same_v<Immovable&, std::tuple_element_t<0, decltype(tuple)>>);
    CHECK(std::is_same_v<Immovable&, decltype(T::template get<0>(std::as_const(tuple)))>);
    CHECK(std::is_same_v<const double, decltype(b)>);
    CHECK(std::is_same_v<const double&, decltype(T::template get<1>(std::as_const(tuple)))>);
}

template <class T>
void test_rref_structured_binding()
{
    Immovable c{1};
    TupleT<T, Immovable&, double> tuple{c, 12.};
    auto&& [a, b] = tuple;
    CHECK_EQ(c, a);
    CHECK(std::is_same_v<Immovable&, decltype(a)>);
}

template <class T>
void test_rref_structured_binding_from_move()
{
    MoveOnly c{1};
    TupleT<T, MoveOnly&, double> tuple{c, 12.};
    auto&& [a, b] = std::move(tuple);
    a = MoveOnly{2};
    CHECK_EQ(2, c.v);
    CHECK(std::is_same_v<MoveOnly&, decltype(a)>);
}
}  // namespace test

#endif  // LTPL_TEST_TEST_STRUCTURED_BINDING_HPP
