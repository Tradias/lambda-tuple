// Copyright (c) 2022 Dennis Hezel
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef LTPL_TEST_UTILITY_HPP
#define LTPL_TEST_UTILITY_HPP

#include <type_traits>
#include <utility>

namespace test
{
struct Empty
{
};

struct ImplicitTag
{
    int v{};
};

struct CopyOnly
{
    int v{};

    CopyOnly() = default;

    constexpr explicit CopyOnly(int v) noexcept : v(v) {}

    constexpr CopyOnly(const ImplicitTag& t) noexcept : v(t.v) {}

    CopyOnly(const CopyOnly&) = default;

    CopyOnly(CopyOnly&&) = default;

    CopyOnly& operator=(const CopyOnly&) = default;

    CopyOnly& operator=(CopyOnly&&) = delete;

    friend bool operator==(const CopyOnly&, const CopyOnly&) = default;

    friend void swap(const CopyOnly&, const CopyOnly&) {}
};

struct MoveOnly
{
    int v{};
    bool is_moved_from{};
    bool is_swapped{};

    MoveOnly() = default;

    constexpr explicit MoveOnly(int v) noexcept : v(v) {}

    constexpr MoveOnly(ImplicitTag&& t) noexcept : v(t.v) {}

    MoveOnly(const MoveOnly&) = delete;

    constexpr MoveOnly(MoveOnly&& other) noexcept : v(other.v), is_moved_from(std::exchange(other.is_moved_from, true))
    {
    }

    MoveOnly& operator=(const MoveOnly&) = delete;

    constexpr MoveOnly& operator=(MoveOnly&& other) noexcept
    {
        v = other.v;
        is_moved_from = std::exchange(other.is_moved_from, true);
        return *this;
    }

    friend bool operator==(const MoveOnly&, const MoveOnly&) = default;

    friend void swap(MoveOnly& lhs, MoveOnly& rhs)
    {
        lhs.is_swapped = true;
        rhs.is_swapped = true;
    }
};

struct Immovable
{
    int v{};

    Immovable() = default;

    constexpr explicit Immovable(int v) : v(v){};

    Immovable(const Immovable&) = delete;

    Immovable(Immovable&&) = delete;

    Immovable& operator=(const Immovable&) = delete;

    Immovable& operator=(Immovable&&) = delete;

    friend bool operator==(const Immovable&, const Immovable&) = default;
};

template <class T, class U>
concept ConvertibleTo = std::is_convertible_v<T, U>;

template <class T, class U>
concept WeaklyEqualityComparableWith = requires(const std::remove_reference_t<T>& t,
                                                const std::remove_reference_t<U>& u)
{
    {
        t == u
        } -> ConvertibleTo<bool>;
    {
        t != u
        } -> ConvertibleTo<bool>;
    {
        u == t
        } -> ConvertibleTo<bool>;
    {
        u != t
        } -> ConvertibleTo<bool>;
};
}  // namespace test

#endif  // LTPL_TEST_UTILITY_HPP
