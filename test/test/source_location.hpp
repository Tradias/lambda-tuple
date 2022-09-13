// Copyright (c) 2022 Dennis Hezel
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef LTPL_TEST_SOURCE_LOCATION_HPP
#define LTPL_TEST_SOURCE_LOCATION_HPP

#include <cstdint>
#include <version>

#if !defined(_MSC_VER) && defined(__cpp_lib_source_location) && __cpp_lib_source_location >= 201907L
#include <source_location>
#endif

namespace test
{
struct SourceLocation
{
    constexpr SourceLocation(char const* file, std::uint_least32_t ln, char const* function,
                             std::uint_least32_t col = 0) noexcept
        : file(file), function(function), line(ln), column(col)
    {
    }

#if !defined(_MSC_VER) && defined(__cpp_lib_source_location) && __cpp_lib_source_location >= 201907L
    constexpr SourceLocation(const std::source_location& loc) noexcept
        : file(loc.file_name()), function(loc.function_name()), line(loc.line()), column(loc.column())
    {
    }
#endif

    const char* file;
    const char* function;
    std::uint_least32_t line;
    std::uint_least32_t column;
};
}  // namespace test

#if _MSC_VER >= 1926
#define TEST_LTPL_CURRENT_LOCATION \
    ::test::SourceLocation(__builtin_FILE(), __builtin_LINE(), __builtin_FUNCTION(), __builtin_COLUMN())
#elif defined(__cpp_lib_source_location) && __cpp_lib_source_location >= 201907L
#define TEST_LTPL_CURRENT_LOCATION ::test::SourceLocation(::std::source_location::current())
#elif defined(__clang__)
#define TEST_LTPL_CURRENT_LOCATION \
    ::test::SourceLocation(__builtin_FILE(), __builtin_LINE(), __builtin_FUNCTION(), __builtin_COLUMN())
#elif defined(__GNUC__)
#define TEST_LTPL_CURRENT_LOCATION ::test::SourceLocation(__builtin_FILE(), __builtin_LINE(), __builtin_FUNCTION())
#else
#define TEST_LTPL_CURRENT_LOCATION ::test::SourceLocation(__FILE__, __LINE__, "")
#endif

#endif  // LTPL_TEST_SOURCE_LOCATION_HPP
