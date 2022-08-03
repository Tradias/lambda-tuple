// Copyright (c) Microsoft Corporation.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

// https://github.com/microsoft/STL/blob/main/tests/std/tests/Dev11_0607540_pair_tuple_rvalue_references/test.cpp

#ifndef LTPL_TEST_TEST_DEV11_0607540_PAIR_TUPLE_RVALUE_REFERENCES_HPP
#define LTPL_TEST_TEST_DEV11_0607540_PAIR_TUPLE_RVALUE_REFERENCES_HPP

#include <ltpl/tuple.hpp>
#include <test/framework.hpp>

namespace test
{
struct XX
{
    int&& rv;

    explicit XX(int&& i) : rv(std::move(i)) {}
};

void test_Dev11_0607540_pair_tuple_rvalue_references()
{
    {
        int k = 1701;
        int n = 1729;

        XX x1(std::move(n));
        XX x3(std::move(x1));

        CHECK(&x1.rv == &n);
        CHECK(&x3.rv == &n);

        ltpl::Tuple<int&&, int&&> t1(std::move(k), std::move(n));
        ltpl::Tuple<int&&, int&&> t3(std::move(t1));

        CHECK(&ltpl::get<0>(t1) == &k);
        CHECK(&ltpl::get<1>(t1) == &n);
        CHECK(&ltpl::get<0>(t3) == &k);
        CHECK(&ltpl::get<1>(t3) == &n);

        auto&& f1(ltpl::forward_as_tuple(std::move(k), std::move(n)));
        auto f3(std::move(f1));

        CHECK(&ltpl::get<0>(f1) == &k);
        CHECK(&ltpl::get<1>(f1) == &n);
        CHECK(&ltpl::get<0>(f3) == &k);
        CHECK(&ltpl::get<1>(f3) == &n);
    }

    // Also test ltpl::get<I> and ltpl::get<T> on pairs of objects, lvalue references, and rvalue references.

    {
        ltpl::Tuple<int, const char*> p(1729, "meow");
        const ltpl::Tuple<int, const char*> c(1701, "NCC");
        ltpl::Tuple<int, const char*> m(1024, "kilo");

        CHECK(std::is_same_v<decltype(ltpl::get<0>(p)), int&>);
        CHECK(std::is_same_v<decltype(ltpl::get<1>(p)), const char*&>);
        CHECK(std::is_same_v<decltype(ltpl::get<0>(c)), const int&>);
        CHECK(std::is_same_v<decltype(ltpl::get<1>(c)), const char* const&>);
        CHECK(std::is_same_v<decltype(ltpl::get<0>(std::move(m))), int&&>);
        CHECK(std::is_same_v<decltype(ltpl::get<1>(std::move(m))), const char*&&>);
    }

    {
        int x = 11;
        char y = 'a';

        ltpl::Tuple<int&, char&> p(x, y);
        const ltpl::Tuple<int&, char&> c(x, y);

        CHECK(std::is_same_v<decltype(ltpl::get<0>(p)), int&>);
        CHECK(std::is_same_v<decltype(ltpl::get<1>(p)), char&>);
        CHECK(std::is_same_v<decltype(ltpl::get<0>(c)), int&>);
        CHECK(std::is_same_v<decltype(ltpl::get<1>(c)), char&>);
        CHECK(std::is_same_v<decltype(ltpl::get<0>(std::move(p))), int&>);
        CHECK(std::is_same_v<decltype(ltpl::get<1>(std::move(p))), char&>);

        CHECK(&ltpl::get<0>(p) == &x);
        CHECK(&ltpl::get<1>(p) == &y);
        CHECK(&ltpl::get<0>(c) == &x);
        CHECK(&ltpl::get<1>(c) == &y);
        CHECK(&ltpl::get<0>(std::move(p)) == &x);
        CHECK(&ltpl::get<1>(std::move(p)) == &y);
    }

    {
        int x = 11;
        char y = 'a';

        ltpl::Tuple<int&&, char&&> p(std::move(x), std::move(y));
        const ltpl::Tuple<int&&, char&&> c(std::move(x), std::move(y));

        CHECK(std::is_same_v<decltype(ltpl::get<0>(p)), int&>);
        CHECK(std::is_same_v<decltype(ltpl::get<1>(p)), char&>);
        CHECK(std::is_same_v<decltype(ltpl::get<0>(c)), int&>);
        CHECK(std::is_same_v<decltype(ltpl::get<1>(c)), char&>);
        CHECK(std::is_same_v<decltype(ltpl::get<0>(std::move(p))), int&&>);
        CHECK(std::is_same_v<decltype(ltpl::get<1>(std::move(p))), char&&>);

        CHECK(&ltpl::get<0>(p) == &x);
        CHECK(&ltpl::get<1>(p) == &y);
        CHECK(&ltpl::get<0>(c) == &x);
        CHECK(&ltpl::get<1>(c) == &y);
        CHECK(ltpl::get<0>(std::move(p)) == x);
        CHECK(ltpl::get<1>(std::move(p)) == y);
    }
}
}  // namespace test

#endif  // LTPL_TEST_TEST_DEV11_0607540_PAIR_TUPLE_RVALUE_REFERENCES_HPP
