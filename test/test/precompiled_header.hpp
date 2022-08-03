// Copyright (c) 2022 Dennis Hezel
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <version>

#if !defined(_MSC_VER) && defined(__cpp_lib_source_location) && __cpp_lib_source_location >= 201907L
#include <source_location>
#endif