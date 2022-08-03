// Copyright (c) 2022 Dennis Hezel
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef LTPL_TEST_FRAMEWORK_HPP
#define LTPL_TEST_FRAMEWORK_HPP

#include <test/source_location.hpp>
#include <test/type_name.hpp>

#include <cstdio>
#include <utility>

#ifndef TEST_LTPL_NOINLINE
#ifdef _MSC_VER
#define TEST_LTPL_NOINLINE __declspec(noinline)
#elif defined(__GNUC__) || defined(__clang__)
#define TEST_LTPL_NOINLINE __attribute__((noinline))
#else
#define TEST_LTPL_NOINLINE
#endif
#endif

namespace test
{
struct Expression
{
    const char* name;
    const char* fragment0;
    const char* fragment1{""};
    const char* fragment2{""};
};

struct FailedCheck
{
    Expression expression;
    SourceLocation location;
};

struct Context
{
    int tests_run;
    int failed_tests;
    int current_failed_assertions;
    FailedCheck failed_checks[128];
};

static Context context{};

TEST_LTPL_NOINLINE inline void run_test_impl(void (*test)(), StringView test_name)
{
    context.current_failed_assertions = 0;
    test();
    if (context.current_failed_assertions > 0)
    {
        ++context.failed_tests;
        for (int i = 0; i < context.current_failed_assertions; ++i)
        {
            auto& [expression, location] = context.failed_checks[i];
            ::printf("Failure: %s:(%u) in \"%.*s\"\n  %s(%s%s%s)\n\n", location.file, location.line, test_name.size,
                     test_name.data, expression.name, expression.fragment0, expression.fragment1, expression.fragment2);
        }
    }
    ++context.tests_run;
}

template <auto Test>
void run_test()
{
    test::run_test_impl(Test, test::function_pointer_name_v<Test>);
}

TEST_LTPL_NOINLINE inline void print_test_results()
{
    if (context.failed_tests == 0)
    {
        ::printf("All tests succeeded: %i", context.tests_run);
    }
    else
    {
        ::printf("======================================\nFailed tests: %i/%i", context.failed_tests,
                 context.tests_run);
    }
}

TEST_LTPL_NOINLINE inline void check(bool result, Expression expression,
                                     SourceLocation location = TEST_LTPL_CURRENT_LOCATION)
{
    if (!result)
    {
        context.failed_checks[context.current_failed_assertions] = {expression, location};
        ++context.current_failed_assertions;
    }
}
}  // namespace test

#define CHECK(...) ::test::check(static_cast<bool>(__VA_ARGS__), {"CHECK", #__VA_ARGS__})

#define CHECK_FALSE(...) ::test::check(!static_cast<bool>(__VA_ARGS__), {"CHECK_FALSE", #__VA_ARGS__})

#define CHECK_EQ(lhs, rhs) ::test::check(static_cast<bool>((lhs) == (rhs)), {"CHECK_EQ", #lhs, ", ", #rhs})

#define CHECK_NE(lhs, rhs) ::test::check(!static_cast<bool>((lhs) == (rhs)), {"CHECK_NE", #lhs, ", ", #rhs})

#endif  // LTPL_TEST_FRAMEWORK_HPP
