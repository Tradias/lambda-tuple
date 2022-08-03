# Copyright (c) 2022 Dennis Hezel
# 
# This software is released under the MIT License.
# https://opensource.org/licenses/MIT

# common compile options
add_library(lambda-tuple-compile-options INTERFACE)

if(LAMBDA_TUPLE_ENABLE_DYNAMIC_ANALYSIS)
    target_compile_options(
        lambda-tuple-compile-options
        INTERFACE
            $<$<OR:$<CXX_COMPILER_ID:GNU>,$<STREQUAL:${CMAKE_CXX_COMPILER_FRONTEND_VARIANT},GNU>>:-fsanitize=undefined,address
            -fno-omit-frame-pointer>
            $<$<OR:$<CXX_COMPILER_ID:MSVC>,$<STREQUAL:${CMAKE_CXX_COMPILER_FRONTEND_VARIANT},MSVC>>:/fsanitize=address>)

    target_link_options(
        lambda-tuple-compile-options
        INTERFACE
        $<$<OR:$<CXX_COMPILER_ID:GNU>,$<STREQUAL:${CMAKE_CXX_COMPILER_FRONTEND_VARIANT},GNU>>:-fsanitize=undefined,address>
    )
endif()

target_compile_options(
    lambda-tuple-compile-options
    INTERFACE $<$<OR:$<CXX_COMPILER_ID:MSVC>,$<STREQUAL:${CMAKE_CXX_COMPILER_FRONTEND_VARIANT},MSVC>>:
              /W4
              /WX
              /permissive-
              /Zc:__cplusplus
              /Zc:inline
              /Zc:sizedDealloc>
              $<$<CXX_COMPILER_ID:MSVC>:
              /Zc:preprocessor
              /Zc:externConstexpr
              /Zc:lambda
              /Zc:throwingNew>
              $<$<OR:$<CXX_COMPILER_ID:GNU>,$<STREQUAL:${CMAKE_CXX_COMPILER_FRONTEND_VARIANT},GNU>>:
              -Werror
              -Wall
              -Wextra
              -pedantic-errors>)

if("${CMAKE_GENERATOR}" STRGREATER_EQUAL "Visual Studio")
    target_compile_options(lambda-tuple-compile-options INTERFACE /MP)
endif()

target_compile_features(lambda-tuple-compile-options INTERFACE cxx_std_20)
