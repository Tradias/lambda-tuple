// Copyright (c) 2022 Dennis Hezel
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef LTPL_TEST_TEST_HPP
#define LTPL_TEST_TEST_HPP

#include <ltpl/tuple.hpp>
#include <test/framework.hpp>
#include <test/utility.hpp>

#include <string>
#include <tuple>

namespace test
{
void test_constexpr()
{
    static constexpr Immovable move_only{1};
    static constexpr ltpl::Tuple<const Immovable&, double> tuple{move_only, 12.};
    static constexpr auto tuple2{tuple};
    static constexpr auto& v0 = ltpl::get<0>(tuple2);
    CHECK_EQ(move_only, v0);
}

void test_concepts()
{
    using EmptyTuple = ltpl::Tuple<>;
    CHECK(std::is_trivially_default_constructible_v<EmptyTuple>);
    CHECK(std::is_trivially_copy_constructible_v<EmptyTuple>);
    CHECK(std::is_trivially_move_constructible_v<EmptyTuple>);
    CHECK(std::is_trivially_copy_assignable_v<EmptyTuple>);
    CHECK(std::is_trivially_move_assignable_v<EmptyTuple>);
    CHECK(std::is_trivially_destructible_v<EmptyTuple>);

    using Tuple = ltpl::Tuple<int, double>;
    CHECK(std::is_nothrow_constructible_v<Tuple, int, double>);
    CHECK(std::is_trivially_copy_constructible_v<Tuple>);
    CHECK(std::is_trivially_move_constructible_v<Tuple>);
    CHECK(std::is_trivially_destructible_v<Tuple>);
}

void test_sizeof()
{
    CHECK_EQ(sizeof(ltpl::Tuple<int, Empty>), sizeof(ltpl::Tuple<Empty, int>));
    CHECK_EQ(alignof(int), alignof(ltpl::Tuple<int, Empty>));
}

void test_tuple_element_rref()
{
    MoveOnly move_only{};
    ltpl::Tuple<MoveOnly&&, double> tuple{std::move(move_only), 12.};
    ltpl::Tuple<MoveOnly&&, double> tuple2{std::move(move_only), 12.};
    tuple2 = std::move(tuple);
    auto&& [i, d] = tuple;
    CHECK_EQ(move_only, i);
    CHECK(std::is_same_v<MoveOnly&&, decltype(i)>);
    CHECK(std::is_same_v<MoveOnly&&, std::tuple_element_t<0, decltype(tuple)>>);
    CHECK(std::is_same_v<MoveOnly&, decltype(ltpl::get<0>(tuple))>);

    auto&& [ii, dd] = std::move(tuple);
    MoveOnly move_only2{std::move(ii)};
    CHECK_EQ(MoveOnly{}, move_only2);
    CHECK(std::is_same_v<MoveOnly&&, decltype(ii)>);
    CHECK(std::is_same_v<MoveOnly&&, decltype(ltpl::get<0>(std::move(tuple)))>);
}

void test_forward_as_tuple()
{
    MoveOnly c{1};
    auto tuple = ltpl::forward_as_tuple(42, c);
    CHECK(std::is_same_v<ltpl::Tuple<int&&, MoveOnly&>, decltype(tuple)>);
}

void test_tie()
{
    int a{42};
    MoveOnly c{1};
    auto tuple = ltpl::tie(a, c);
    CHECK(std::is_same_v<ltpl::Tuple<int&, MoveOnly&>, decltype(tuple)>);
}

void test_make_tuple()
{
    int a{42};
    MoveOnly c{1};
    auto tuple = ltpl::make_tuple(std::ref(a), std::move(c));
    CHECK_EQ(42, ltpl::get<0>(tuple));
    CHECK_EQ(1, ltpl::get<1>(tuple).v);
    CHECK(std::is_same_v<ltpl::Tuple<int&, MoveOnly>, decltype(tuple)>);
}
}  // namespace test

#endif  // LTPL_TEST_TEST_HPP
