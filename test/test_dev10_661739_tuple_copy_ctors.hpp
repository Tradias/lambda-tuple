// Copyright (c) Microsoft Corporation.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

// https://github.com/microsoft/STL/blob/main/tests/std/tests/Dev10_661739_tuple_copy_ctors/test.cpp

#ifndef LTPL_TEST_TEST_DEV10_661739_TUPLE_COPY_CTORS_HPP
#define LTPL_TEST_TEST_DEV10_661739_TUPLE_COPY_CTORS_HPP

#include <ltpl/tuple.hpp>
#include <test/framework.hpp>

namespace test
{
void test_Dev10_661739_tuple_copy_ctors()
{
    {
        ltpl::Tuple<> src;
        ltpl::Tuple<> dest(src);
        CHECK(src == dest);
    }

    {
        ltpl::Tuple<int> src(1729);
        ltpl::Tuple<int> dest(src);
        CHECK(src == dest);
    }

    {
        ltpl::Tuple<int, int> src(17, 29);
        ltpl::Tuple<int, int> dest(src);
        CHECK(src == dest);
    }

    {
        ltpl::Tuple<int> src(1729);
        ltpl::Tuple<long> dest(src);
        CHECK(src == dest);
    }

    {
        ltpl::Tuple<int, int> src(17, 29);
        ltpl::Tuple<long, long> dest(src);
        CHECK(src == dest);
    }
}
}

#endif // LTPL_TEST_TEST_DEV10_661739_TUPLE_COPY_CTORS_HPP
