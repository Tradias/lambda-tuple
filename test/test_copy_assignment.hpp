// Copyright (c) 2022 Dennis Hezel
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef LTPL_TEST_TEST_COPY_ASSIGNMENT_HPP
#define LTPL_TEST_TEST_COPY_ASSIGNMENT_HPP

#include <ltpl/tuple.hpp>
#include <test/factory.hpp>
#include <test/framework.hpp>
#include <test/utility.hpp>

namespace test
{
static constexpr CopyOnly COPY_ONLY_1{1};
static constexpr CopyOnly COPY_ONLY_2{2};

template <class T>
void test_copy_assignment_value_value()
{
    TupleT<T, CopyOnly> tuple(COPY_ONLY_1);
    TupleT<T, CopyOnly> tuple2(COPY_ONLY_2);
    tuple2 = tuple;
    CHECK_EQ(COPY_ONLY_1, T::template get<0>(tuple));
    CHECK_EQ(COPY_ONLY_1, T::template get<0>(tuple2));
}

template <class T>
void test_copy_assignment_const_value_value()
{
    TupleT<T, const CopyOnly> tuple(COPY_ONLY_1);
    TupleT<T, CopyOnly> tuple2(COPY_ONLY_2);
    tuple2 = tuple;
    CHECK_EQ(COPY_ONLY_1, T::template get<0>(tuple));
    CHECK_EQ(COPY_ONLY_1, T::template get<0>(tuple2));
    CHECK_FALSE(std::is_assignable_v<decltype(tuple), decltype(tuple2)>);
}

template <class T>
void test_copy_assignment_lref_value()
{
    CopyOnly c{1};
    TupleT<T, CopyOnly&> tuple(c);
    TupleT<T, CopyOnly> tuple2(COPY_ONLY_2);
    tuple2 = tuple;
    CHECK_EQ(COPY_ONLY_1, c);
    CHECK_EQ(COPY_ONLY_1, T::template get<0>(tuple));
    CHECK_EQ(COPY_ONLY_1, T::template get<0>(tuple2));
}

template <class T>
void test_copy_assignment_const_lref_value()
{
    TupleT<T, const CopyOnly&> tuple(COPY_ONLY_1);
    TupleT<T, CopyOnly> tuple2(COPY_ONLY_2);
    tuple2 = tuple;
    CHECK_EQ(COPY_ONLY_1, T::template get<0>(tuple));
    CHECK_EQ(COPY_ONLY_1, T::template get<0>(tuple2));
    CHECK_FALSE(std::is_assignable_v<decltype(tuple), decltype(tuple2)>);
}

template <class T>
void test_copy_assignment_value_lref()
{
    TupleT<T, CopyOnly> tuple(COPY_ONLY_1);
    CopyOnly c{2};
    TupleT<T, CopyOnly&> tuple2(c);
    tuple2 = tuple;
    CHECK_EQ(COPY_ONLY_1, T::template get<0>(tuple));
    CHECK_EQ(COPY_ONLY_1, c);
    CHECK_EQ(COPY_ONLY_1, T::template get<0>(tuple2));
}

template <class T>
void test_copy_assignment_const_value_lref()
{
    TupleT<T, const CopyOnly> tuple(COPY_ONLY_1);
    CopyOnly c{2};
    TupleT<T, CopyOnly&> tuple2(c);
    tuple2 = tuple;
    CHECK_EQ(COPY_ONLY_1, T::template get<0>(tuple));
    CHECK_EQ(COPY_ONLY_1, c);
    CHECK_EQ(COPY_ONLY_1, T::template get<0>(tuple2));
    CHECK_FALSE(std::is_assignable_v<decltype(tuple), decltype(tuple2)>);
}

template <class T>
void test_copy_assignment_lref_lref()
{
    CopyOnly c{1};
    TupleT<T, CopyOnly&> tuple(c);
    CopyOnly c2{2};
    TupleT<T, CopyOnly&> tuple2(c2);
    tuple2 = tuple;
    CHECK_EQ(COPY_ONLY_1, c);
    CHECK_EQ(COPY_ONLY_1, T::template get<0>(tuple));
    CHECK_EQ(COPY_ONLY_1, c2);
    CHECK_EQ(COPY_ONLY_1, T::template get<0>(tuple2));
}

template <class T>
void test_copy_assignment_const_ref_ref()
{
    TupleT<T, const CopyOnly&> tuple(COPY_ONLY_1);
    CopyOnly c2{2};
    TupleT<T, CopyOnly&> tuple2(c2);
    tuple2 = tuple;
    CHECK_EQ(COPY_ONLY_1, T::template get<0>(tuple));
    CHECK_EQ(COPY_ONLY_1, c2);
    CHECK_EQ(COPY_ONLY_1, T::template get<0>(tuple2));
    CHECK_FALSE(std::is_assignable_v<decltype(tuple), decltype(tuple2)>);
}

template <class T>
void test_copy_assignment_rref_lref()
{
    CopyOnly c{1};
    TupleT<T, CopyOnly&&> tuple(std::move(c));
    CopyOnly c2{2};
    TupleT<T, CopyOnly&> tuple2(c2);
    tuple2 = tuple;
    CHECK_EQ(COPY_ONLY_1, c);
    CHECK_EQ(COPY_ONLY_1, T::template get<0>(tuple));
    CHECK_EQ(COPY_ONLY_1, c2);
    CHECK_EQ(COPY_ONLY_1, T::template get<0>(tuple2));
}

template <class T>
void test_copy_assignment_rref_value()
{
    TupleT<T, CopyOnly> tuple(COPY_ONLY_1);
    CopyOnly c2{2};
    TupleT<T, CopyOnly&&> tuple2(std::move(c2));
    tuple2 = tuple;
    CHECK_EQ(COPY_ONLY_1, T::template get<0>(tuple));
    CHECK_EQ(COPY_ONLY_1, c2);
    CHECK_EQ(COPY_ONLY_1, T::template get<0>(tuple2));
}

template <class T>
void test_copy_assignment_lref_rref()
{
    CopyOnly c{1};
    TupleT<T, CopyOnly&> tuple(c);
    CopyOnly c2{2};
    TupleT<T, CopyOnly&&> tuple2(std::move(c2));
    tuple2 = tuple;
    CHECK_EQ(COPY_ONLY_1, T::template get<0>(tuple));
    CHECK_EQ(COPY_ONLY_1, c2);
    CHECK_EQ(COPY_ONLY_1, T::template get<0>(tuple2));
}
}  // namespace test

#endif  // LTPL_TEST_TEST_COPY_ASSIGNMENT_HPP
