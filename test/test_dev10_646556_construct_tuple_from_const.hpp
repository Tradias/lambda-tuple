// Copyright (c) Microsoft Corporation.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

// https://github.com/microsoft/STL/blob/main/tests/std/tests/Dev10_646556_construct_tuple_from_const/test.cpp

#ifndef LTPL_TEST_TEST_DEV10_646556_CONSTRUCT_TUPLE_FROM_CONST_HPP
#define LTPL_TEST_TEST_DEV10_646556_CONSTRUCT_TUPLE_FROM_CONST_HPP

#include <ltpl/tuple.hpp>
#include <test/framework.hpp>

#include <string>

namespace test
{
void test_Dev10_646556_construct_tuple_from_const()
{
    int a = 10;
    std::string b = "twenty";
    const int c = 30;
    const std::string d = "forty";

    ltpl::Tuple<int, std::string, int, std::string> t(a, b, c, d);

    CHECK(ltpl::get<0>(t) == 10);
    CHECK(ltpl::get<1>(t) == "twenty");
    CHECK(ltpl::get<2>(t) == 30);
    CHECK(ltpl::get<3>(t) == "forty");
}
}  // namespace test

#endif  // LTPL_TEST_TEST_DEV10_646556_CONSTRUCT_TUPLE_FROM_CONST_HPP
