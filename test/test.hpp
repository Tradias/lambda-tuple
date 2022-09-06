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
void test_move_assignment()
{
    ltpl::Tuple tuple(1, MoveOnly{});
    ltpl::Tuple tuple2(2, MoveOnly{});
    tuple2 = std::move(tuple);
    CHECK(noexcept(tuple2 = std::move(tuple)));
    CHECK_EQ(1, ltpl::get<0>(tuple2));
    MoveOnly move_only{};
    ltpl::Tuple<int, MoveOnly&&> tuple3(2, std::move(move_only));
    tuple = std::move(tuple3);
    CHECK_EQ(MoveOnly{}, ltpl::get<1>(tuple));
}

void test_structured_binding()
{
    ltpl::Tuple<int, double> this_tuple{42, 12.};
    auto& [i, d] = this_tuple;
    d = 10.;
    CHECK_EQ(10., d);
}

void test_ref()
{
    Immovable immovable{1};
    ltpl::Tuple<Immovable&, double> tuple{immovable, 12.};
    auto&& [i, d] = tuple;
    CHECK_EQ(immovable, i);
    auto& [ii, dd] = std::as_const(tuple);
    ii.v = 12;
    CHECK_EQ(12, ii.v);
    // CHECK(std::is_same_v<Immovable&, decltype(ii)>);
    // CHECK(std::is_same_v<Immovable&, std::tuple_element_t<0, decltype(tuple)>>);
    // CHECK(std::is_same_v<Immovable&, decltype(ltpl::get<0>(std::as_const(tuple)))>);
    // CHECK(std::is_same_v<const double&, decltype(dd)>);
    // CHECK(std::is_same_v<const double&, decltype(ltpl::get<0>(std::as_const(tuple)))>);
    std::tuple<Immovable&, double> std_tuple{immovable, 12.};
    auto& [iii, ddd] = std::as_const(std_tuple);
    CHECK(std::is_same_v<Immovable&, decltype(iii)>);
    CHECK(std::is_same_v<Immovable&, std::tuple_element_t<0, decltype(std_tuple)>>);
    // CHECK(std::is_same_v<const double&, decltype(ddd)>);
    // CHECK(std::is_same_v<const double&, decltype(std::get<0>(std::as_const(std_tuple)))>);
}

void test_const_ref()
{
    MoveOnly move_only{};
    const ltpl::Tuple<MoveOnly, double> tuple{std::move(move_only), 12.};
    auto& [i, d] = tuple;
    CHECK_EQ(MoveOnly{}, i);
    CHECK(std::is_same_v<const MoveOnly&, decltype(ltpl::get<0>(tuple))>);
}

void test_move()
{
    MoveOnly move_only{};
    ltpl::Tuple<MoveOnly, double> tuple{std::move(move_only), 12.};
    auto&& [i, d] = std::move(tuple);
    CHECK_EQ(MoveOnly{}, i);
}

void test_forward_as_tuple()
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

    MoveOnly move_only3{};
    std::tuple<MoveOnly&&, double> std_tuple{std::move(move_only3), 12.};
    std::tuple<MoveOnly&&, double> std_tuple2{std::move(move_only3), 12.};
    std_tuple2 = std::move(std_tuple);

    auto&& [iii, ddd] = std_tuple;
    CHECK(std::is_same_v<MoveOnly&&, decltype(iii)>);
    CHECK(std::is_same_v<MoveOnly&&, std::tuple_element_t<0, decltype(std_tuple)>>);
    CHECK(std::is_same_v<MoveOnly&, decltype(std::get<0>(std_tuple))>);

    auto&& [iiii, dddd] = std::move(std_tuple);
    CHECK(std::is_same_v<MoveOnly&&, decltype(iiii)>);
    CHECK(std::is_same_v<MoveOnly&&, decltype(std::get<0>(std::move(std_tuple)))>);
}

void test_std_tuple_forward_as_tuple()
{
    MoveOnly move_only{};
    std::tuple<MoveOnly&&, double> tuple{std::move(move_only), 12.};
    auto&& [i, d] = tuple;
    CHECK_EQ(move_only, i);
    auto&& [ii, dd] = std::move(tuple);
    CHECK_EQ(move_only, ii);
}

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

void test_std_tuple_int_Empty()
{
    using StdTuple = std::tuple<std::string, Empty>;
    using Tuple = ltpl::Tuple<std::string, Empty>;
    auto aa = "aa";
    StdTuple std_tuple{aa, {}};
    Tuple tuple{aa, Empty{}};
}
}  // namespace test

#endif  // LTPL_TEST_TEST_HPP
