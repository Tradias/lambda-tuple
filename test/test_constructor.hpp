// Copyright (c) 2022 Dennis Hezel
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef LTPL_TEST_TEST_CONSTRUCTOR_HPP
#define LTPL_TEST_TEST_CONSTRUCTOR_HPP

#include <ltpl/tuple.hpp>
#include <test/factory.hpp>
#include <test/framework.hpp>
#include <test/utility.hpp>

namespace test
{
template <class T>
void test_empty_tuple_default_constructor()
{
    TupleT<T> tuple{};
    (void)tuple;
}

template <class T>
void test_default_constructor()
{
    TupleT<T, CopyOnly, MoveOnly> tuple{};
    CHECK_EQ(CopyOnly{}, T::template get<0>(tuple));
    CHECK_EQ(MoveOnly{}, T::template get<1>(tuple));
}

template <class T>
void test_lref_arg_constructor()
{
    CopyOnly c{1};
    TupleT<T, int, CopyOnly> tuple{1, c};
    CHECK_EQ(c, T::template get<1>(tuple));
}

template <class T>
void test_const_lref_arg_constructor()
{
    const CopyOnly c{1};
    TupleT<T, int, CopyOnly> tuple{1, c};
    CHECK_EQ(c, T::template get<1>(tuple));
}

template <class T>
void test_rref_arg_constructor()
{
    MoveOnly c{1};
    TupleT<T, int, MoveOnly> tuple{1, std::move(c)};
    CHECK_EQ(MoveOnly{1}, T::template get<1>(tuple));
}

template <class T>
void test_converting_lref_arg_constructor()
{
    ImplicitTag c{1};
    TupleT<T, CopyOnly, int> tuple{c, 2};
    CHECK_EQ(CopyOnly{1}, T::template get<0>(tuple));
}

template <class T>
void test_converting_const_lref_arg_constructor()
{
    const ImplicitTag c{1};
    TupleT<T, CopyOnly, int> tuple{c, 2};
    CHECK_EQ(CopyOnly{1}, T::template get<0>(tuple));
}

template <class T>
void test_converting_rref_arg_constructor()
{
    ImplicitTag c{1};
    TupleT<T, MoveOnly, int> tuple{std::move(c), 2};
    CHECK_EQ(MoveOnly{1}, T::template get<0>(tuple));
}

template <class T>
void test_copy_constructor()
{
    TupleT<T, int, CopyOnly> tuple{2, CopyOnly{1}};
    auto tuple2{tuple};
    CHECK_EQ(tuple, tuple2);
}

template <class T>
void test_const_lref_copy_constructor()
{
    const CopyOnly c;
    TupleT<T, int, const CopyOnly&> tuple{2, c};
    auto tuple2{tuple};
    CHECK_EQ(tuple, tuple2);
}

template <class T>
void test_rref_copy_constructor()
{
    CHECK_FALSE(std::is_constructible_v<TupleT<T, int, MoveOnly>, const TupleT<T, int, MoveOnly&&>&>);
}

template <class T>
void test_rref_copy_constructor_does_not_move()
{
    BasicMoveOnly<true> c;
    TupleT<T, int, BasicMoveOnly<true>&&> tuple{2, std::move(c)};
    TupleT<T, int, BasicMoveOnly<true>> tuple2{tuple};
    CHECK_FALSE(c.is_moved_from);
}

template <class T>
CopyOnly convert_copy(TupleT<T, CopyOnly> t)
{
    return T::template get<0>(t);
}

template <class T>
void test_implicit_converting_copy_constructor()
{
    TupleT<T, ImplicitTag> tuple{ImplicitTag{1}};
    CHECK_EQ(CopyOnly{1}, convert_copy<T>(tuple));
    CHECK_FALSE(std::is_convertible_v<const TupleT<T, int>&, TupleT<T, CopyOnly>>);
}

template <class T>
void test_implicit_converting_lref__arg_constructor()
{
    ImplicitTag c{1};
    CHECK_EQ(CopyOnly{1}, convert_copy<T>(c));
}

template <class T>
void test_implicit_converting_const_lref_arg_constructor()
{
    const ImplicitTag c{1};
    CHECK_EQ(CopyOnly{1}, convert_copy<T>(c));
}

template <class T>
void test_implicit_converting_tuple_of_tuple_copy_constructor()
{
    TupleT<T, CopyOnly> tuple(CopyOnly{1});
    TupleT<T, TupleT<T, CopyOnly>> tuple2(tuple);
    CHECK_EQ(CopyOnly{1}, T::template get<0>(T::template get<0>(tuple2)));
    CHECK_FALSE(std::is_convertible_v<const TupleT<T, int>&, TupleT<T, TupleT<T, CopyOnly>>>);
}

template <class T>
void test_implicit_converting_tuple_of_tuple_lref_arg_constructor()
{
    ImplicitTag c{1};
    TupleT<T, ImplicitTag&> tuple(c);
    TupleT<T, TupleT<T, CopyOnly>> tuple2(tuple);
    CHECK_EQ(CopyOnly{1}, T::template get<0>(T::template get<0>(tuple2)));
}

template <class T>
void test_implicit_converting_tuple_of_tuple_const_lref_arg_constructor()
{
    const ImplicitTag c{1};
    TupleT<T, const ImplicitTag&> tuple(c);
    TupleT<T, TupleT<T, CopyOnly>> tuple2(tuple);
    CHECK_EQ(CopyOnly{1}, T::template get<0>(T::template get<0>(tuple2)));
}

template <class T>
MoveOnly convert_move(TupleT<T, MoveOnly> t)
{
    return std::move(T::template get<0>(t));
}

template <class T>
void test_implicit_converting_move_constructor()
{
    TupleT<T, ImplicitTag> tuple{ImplicitTag{1}};
    CHECK_EQ(MoveOnly{1}, convert_move<T>(std::move(tuple)));
    CHECK_FALSE(std::is_convertible_v<const TupleT<T, ImplicitTag>&, TupleT<T, MoveOnly>>);
    CHECK_FALSE(std::is_convertible_v<TupleT<T, int>, TupleT<T, MoveOnly>>);
}

template <class T>
void test_implicit_converting_rref_arg_constructor()
{
    ImplicitTag c{1};
    CHECK_EQ(MoveOnly{1}, convert_move<T>(std::move(c)));
    CHECK_FALSE(std::is_convertible_v<const ImplicitTag&, TupleT<T, MoveOnly>>);
}

template <class T>
void test_implicit_converting_tuple_of_tuple_move_constructor()
{
    TupleT<T, MoveOnly> tuple(MoveOnly{1});
    TupleT<T, TupleT<T, MoveOnly>> tuple2(std::move(tuple));
    CHECK_EQ(MoveOnly{1}, T::template get<0>(T::template get<0>(tuple2)));
    CHECK_FALSE(std::is_constructible_v<TupleT<T, TupleT<T, MoveOnly>>, const TupleT<T, MoveOnly>&>);
    CHECK_FALSE(std::is_convertible_v<TupleT<T, int>, TupleT<T, TupleT<T, MoveOnly>>>);
}

template <class T>
void test_implicit_converting_tuple_of_tuple_value_arg_constructor()
{
    TupleT<T, ImplicitTag> tuple(ImplicitTag{1});
    TupleT<T, TupleT<T, MoveOnly>> tuple2(std::move(tuple));
    CHECK_EQ(MoveOnly{1}, T::template get<0>(T::template get<0>(tuple2)));
    CHECK_FALSE(std::is_constructible_v<TupleT<T, MoveOnly>, TupleT<T, TupleT<T, ImplicitTag>>>);
}

template <class T>
void test_implicit_converting_tuple_of_tuple_rref_arg_constructor()
{
    ImplicitTag c{1};
    TupleT<T, ImplicitTag&&> tuple(std::move(c));
    TupleT<T, TupleT<T, MoveOnly>> tuple2(std::move(tuple));
    CHECK_EQ(MoveOnly{1}, T::template get<0>(T::template get<0>(tuple2)));
    CHECK_FALSE(std::is_constructible_v<TupleT<T, MoveOnly>, TupleT<T, TupleT<T, ImplicitTag&&>>>);
}
}  // namespace test

#endif  // LTPL_TEST_TEST_CONSTRUCTOR_HPP
