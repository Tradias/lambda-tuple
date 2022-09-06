// Copyright (c) 2022 Dennis Hezel
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef LTPL_TEST_TEST_COMPARISON_HPP
#define LTPL_TEST_TEST_COMPARISON_HPP

#include <ltpl/tuple.hpp>
#include <test/factory.hpp>
#include <test/framework.hpp>
#include <test/utility.hpp>

namespace test
{
template <class T>
void test_empty_tuple_equality_compare()
{
    TupleT<T> tuple;
    TupleT<T> tuple2;
    CHECK_EQ(tuple, tuple2);
}

template <class T>
void test_equality_compare_value_value()
{
    TupleT<T, int> tuple(1);
    TupleT<T, int> tuple2(2);
    TupleT<T, long> tuple3(1);
    CHECK_EQ(tuple, tuple);
    CHECK_NE(tuple, tuple2);
    CHECK_EQ(tuple, tuple3);
}

template <class T>
void test_equality_compare_value_lref()
{
    TupleT<T, int> tuple(1);
    int integer{2};
    TupleT<T, int&> tuple2(integer);
    short uinteger{1};
    TupleT<T, short&> tuple3(uinteger);
    CHECK_NE(tuple, tuple2);
    CHECK_NE(tuple2, tuple);
    CHECK_EQ(tuple, tuple3);
    CHECK_EQ(tuple3, tuple);
}

template <class T>
void test_equality_compare_value_const_lref()
{
    TupleT<T, int> tuple(1);
    const int integer{2};
    TupleT<T, const int&> tuple2(integer);
    const short uinteger{1};
    TupleT<T, const short&> tuple3(uinteger);
    CHECK_NE(tuple, tuple2);
    CHECK_NE(tuple2, tuple);
    CHECK_EQ(tuple, tuple3);
    CHECK_EQ(tuple3, tuple);
}

template <class T>
void test_equality_compare_value_rref()
{
    TupleT<T, int> tuple(1);
    int integer{2};
    TupleT<T, int&&> tuple2(std::move(integer));
    short uinteger{1};
    TupleT<T, short&&> tuple3(std::move(uinteger));
    CHECK_NE(tuple, tuple2);
    CHECK_NE(tuple2, tuple);
    CHECK_EQ(tuple, tuple3);
    CHECK_EQ(tuple3, tuple);
}

void test_not_equality_comparable()
{
    CHECK_FALSE(WeaklyEqualityComparableWith<ltpl::Tuple<>, ltpl::Tuple<const char*>>);
    CHECK_FALSE(WeaklyEqualityComparableWith<ltpl::Tuple<int>, ltpl::Tuple<const char*>>);
}
}  // namespace test

#endif  // LTPL_TEST_TEST_COMPARISON_HPP
