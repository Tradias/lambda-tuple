// Copyright (c) 2022 Dennis Hezel
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef LTPL_TEST_TEST_MOVE_ASSIGNMENT_HPP
#define LTPL_TEST_TEST_MOVE_ASSIGNMENT_HPP

#include <ltpl/tuple.hpp>
#include <test/factory.hpp>
#include <test/framework.hpp>
#include <test/utility.hpp>

namespace test
{
template <class T>
void test_move_assignment_value_value()
{
    TupleT<T, MoveOnly> tuple(MoveOnly{1});
    TupleT<T, MoveOnly> tuple2(MoveOnly{2});
    tuple2 = std::move(tuple);
    CHECK(T::template get<0>(tuple).is_moved_from);
    CHECK_EQ(MoveOnly{1}, T::template get<0>(tuple2));
}

template <class T>
void test_move_assignment_const_value_value()
{
    TupleT<T, const MoveOnly> tuple(MoveOnly{});
    TupleT<T, MoveOnly> tuple2(MoveOnly{});
    CHECK_FALSE(std::is_assignable_v<decltype(tuple), decltype(std::move(tuple2))>);
    CHECK_FALSE(std::is_assignable_v<decltype(tuple2), decltype(std::move(tuple))>);
}

template <class T>
void test_move_assignment_lref_value()
{
    MoveOnly c{1};
    TupleT<T, MoveOnly&> tuple(c);
    TupleT<T, MoveOnly> tuple2(MoveOnly{2});
    CHECK_FALSE(std::is_assignable_v<decltype(tuple2), decltype(std::move(tuple))>);
}

template <class T>
void test_move_assignment_const_lref_value()
{
    TupleT<T, const MoveOnly&> tuple(MoveOnly{1});
    TupleT<T, MoveOnly> tuple2(MoveOnly{2});
    CHECK_FALSE(std::is_assignable_v<decltype(tuple), decltype(std::move(tuple2))>);
    CHECK_FALSE(std::is_assignable_v<decltype(tuple2), decltype(std::move(tuple))>);
}

template <class T>
void test_move_assignment_value_lref()
{
    TupleT<T, MoveOnly> tuple(MoveOnly{1});
    MoveOnly c{2};
    TupleT<T, MoveOnly&> tuple2(c);
    tuple2 = std::move(tuple);
    CHECK(T::template get<0>(tuple).is_moved_from);
    CHECK_EQ(MoveOnly{1}, c);
    CHECK_EQ(MoveOnly{1}, T::template get<0>(tuple2));
}

template <class T>
void test_move_assignment_const_value_lref()
{
    TupleT<T, const MoveOnly> tuple(MoveOnly{1});
    MoveOnly c{2};
    TupleT<T, MoveOnly&> tuple2(c);
    CHECK_FALSE(std::is_assignable_v<decltype(tuple), decltype(std::move(tuple2))>);
    CHECK_FALSE(std::is_assignable_v<decltype(tuple2), decltype(std::move(tuple))>);
}

template <class T>
void test_move_assignment_lref_lref()
{
    MoveOnly c{1};
    TupleT<T, MoveOnly&> tuple(c);
    MoveOnly c2{2};
    TupleT<T, MoveOnly&> tuple2(c2);
    CHECK_FALSE(std::is_assignable_v<decltype(tuple), decltype(std::move(tuple2))>);
    CHECK_FALSE(std::is_assignable_v<decltype(tuple2), decltype(std::move(tuple))>);
}

template <class T>
void test_move_assignment_const_ref_ref()
{
    TupleT<T, const MoveOnly&> tuple(MoveOnly{1});
    MoveOnly c2{2};
    TupleT<T, MoveOnly&> tuple2(c2);
    CHECK_FALSE(std::is_assignable_v<decltype(tuple), decltype(std::move(tuple2))>);
    CHECK_FALSE(std::is_assignable_v<decltype(tuple2), decltype(std::move(tuple))>);
}

template <class T>
void test_move_assignment_rref_lref()
{
    MoveOnly c{1};
    TupleT<T, MoveOnly&&> tuple(std::move(c));
    MoveOnly c2{2};
    TupleT<T, MoveOnly&> tuple2(c2);
    tuple2 = std::move(tuple);
    CHECK(c.is_moved_from);
    CHECK(T::template get<0>(tuple).is_moved_from);
    CHECK_EQ(MoveOnly{1}, c2);
    CHECK_EQ(MoveOnly{1}, T::template get<0>(tuple2));
}

template <class T>
void test_move_assignment_rref_value()
{
    TupleT<T, MoveOnly> tuple(MoveOnly{1});
    MoveOnly c2{2};
    TupleT<T, MoveOnly&&> tuple2(std::move(c2));
    tuple2 = std::move(tuple);
    CHECK(T::template get<0>(tuple).is_moved_from);
    CHECK_EQ(MoveOnly{1}, c2);
    CHECK_EQ(MoveOnly{1}, T::template get<0>(tuple2));
}

template <class T>
void test_move_assignment_lref_rref()
{
    MoveOnly c{1};
    TupleT<T, MoveOnly&> tuple(c);
    MoveOnly c2{2};
    TupleT<T, MoveOnly&&> tuple2(std::move(c2));
    CHECK_FALSE(std::is_assignable_v<decltype(tuple2), decltype(std::move(tuple))>);
}
}  // namespace test

#endif  // LTPL_TEST_TEST_MOVE_ASSIGNMENT_HPP
