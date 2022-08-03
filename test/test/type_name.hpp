// Copyright (c) 2022 Dennis Hezel
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef LTPL_TEST_NAMEOF_HPP
#define LTPL_TEST_NAMEOF_HPP

#include <cstddef>

namespace test
{
struct StringView
{
    const char* data;
    int size;
};

template <auto T>
constexpr auto n() noexcept
{
#if defined(__clang__) || defined(__GNUC__)
    return StringView{__PRETTY_FUNCTION__ + 27, sizeof(__PRETTY_FUNCTION__) - 29};
#elif defined(_MSC_VER)
    return StringView{__FUNCSIG__ + 40 + (__FUNCSIG__[21] == '&' ? 1 : 0),
                      sizeof(__FUNCSIG__) - 23 - 40 - (__FUNCSIG__[21] == '&' ? 1 : 0)};
#else
    return StringView{};
#endif
}

template <auto T>
inline constexpr StringView function_pointer_name_v = test::n<T>();
}  // namespace test

#endif  // LTPL_TEST_NAMEOF_HPP
