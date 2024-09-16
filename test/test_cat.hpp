// Copyright (c) 2022 Dennis Hezel
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef LTPL_TEST_TEST_CAT_HPP
#define LTPL_TEST_TEST_CAT_HPP

#include <ltpl/tuple.hpp>
#include <test/factory.hpp>
#include <test/framework.hpp>
#include <test/utility.hpp>

namespace test
{
void test_tuple_cat()
{
    static constexpr Immovable move_only{1};
    static constexpr ltpl::Tuple<const Immovable&, double> tuple{move_only, 12.};
    static constexpr auto tuple2 = ltpl::tuple_cat(tuple, ltpl::Tuple<int>{});
    CHECK(std::is_same_v<const ltpl::Tuple<const Immovable&, double, int>, decltype(tuple2)>);
    int i = 42;
    auto tuple3 = ltpl::tuple_cat(tuple, ltpl::Tuple<int&&>{std::move(i)});
    CHECK(std::is_same_v<ltpl::Tuple<const Immovable&, double, int&&>, decltype(tuple3)>);
    CHECK_EQ(12., ltpl::get<1>(tuple3));
    CHECK_EQ(42, ltpl::get<2>(tuple3));
}
}  // namespace test

#endif  // LTPL_TEST_TEST_CAT_HPP
