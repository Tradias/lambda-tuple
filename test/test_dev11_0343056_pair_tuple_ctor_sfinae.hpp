// Copyright (c) Microsoft Corporation.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

// https://github.com/microsoft/STL/blob/main/tests/std/tests/Dev11_0343056_pair_tuple_ctor_sfinae/test.cpp

// lambda-tuple note: Implicit conversions from int to T* tests have been removed

#ifndef LTPL_TEST_TEST_DEV11_0343056_PAIR_TUPLE_CTOR_SFINAE_HPP
#define LTPL_TEST_TEST_DEV11_0343056_PAIR_TUPLE_CTOR_SFINAE_HPP

#include <ltpl/tuple.hpp>
#include <test/framework.hpp>

namespace test
{
struct A
{
};
struct B : public A
{
};

struct X
{
};
struct Y : public X
{
};

void dog(ltpl::Tuple<A*, A*>) {}
void dog(ltpl::Tuple<A*, X*>) {}
void dog(ltpl::Tuple<X*, A*>) {}
void dog(ltpl::Tuple<X*, X*>) {}

struct Quark
{
};

struct Proton
{
    Proton(Quark&) {}
};

struct Neutron
{
    Neutron(const Quark&) {}
};

struct Meow
{
    Meow(const ltpl::Tuple<int>&) {}
};

void takes_tuple(ltpl::Tuple<Proton, Proton>) {}
void takes_tuple(ltpl::Tuple<Proton, Neutron>) {}
void takes_tuple(ltpl::Tuple<Neutron, Proton>) {}
void takes_tuple(ltpl::Tuple<Neutron, Neutron>) {}

void test_Dev11_0343056_pair_tuple_ctor_sfinae()
{
    {
        B* b = nullptr;
        Y* y = nullptr;

        ltpl::Tuple<B*, B*> tbb(b, b);
        ltpl::Tuple<B*, Y*> tby(b, y);
        ltpl::Tuple<Y*, B*> tyb(y, b);
        ltpl::Tuple<Y*, Y*> tyy(y, y);

        // template <class... UTypes> explicit tuple(UTypes&&... u);
        ltpl::Tuple<A*, X*> t1(b, y);
        ltpl::Tuple<A*, X*> t2(b, nullptr);
        ltpl::Tuple<A*, X*> t3(nullptr, y);
        ltpl::Tuple<A*, X*> t4(nullptr, nullptr);
        (void)t4;

        // template <class... UTypes> tuple(const ltpl::Tuple<UTypes...>& u);
        dog(tbb);
        dog(tby);
        dog(tyb);
        dog(tyy);

        // template <class... UTypes> tuple(ltpl::Tuple<UTypes...>&& u);
        dog(std::move(tbb));
        dog(std::move(tby));
        dog(std::move(tyb));
        dog(std::move(tyy));

        const ltpl::Tuple<Quark, Quark> purr;

        // template <class... UTypes> tuple(const ltpl::Tuple<UTypes...>& u);
        takes_tuple(purr);
    }

    {
        ltpl::Tuple<A*> t1;
        (void)t1;

        ltpl::Tuple<A*> t2(nullptr);

        ltpl::Tuple<A*> t4(t2);
        (void)t4;

        ltpl::Tuple<A*> t5(std::move(t2));
        (void)t5;

        ltpl::Tuple<B*> b(nullptr);

        ltpl::Tuple<A*> t6(b);

        ltpl::Tuple<A*> t7(std::move(b));

        ltpl::Tuple<int> ti(0);
        ltpl::Tuple<Meow> t10(ti);
        ltpl::Tuple<Meow> t11(std::move(ti));
    }
}
}  // namespace test

#endif  // LTPL_TEST_TEST_DEV11_0343056_PAIR_TUPLE_CTOR_SFINAE_HPP
