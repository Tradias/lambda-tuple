# Copyright (c) 2022 Dennis Hezel
#
# This software is released under the MIT License.
# https://opensource.org/licenses/MIT

# tests
add_executable(lambda-tuple-test)

target_sources(lambda-tuple-test PRIVATE "main.cpp")

target_include_directories(lambda-tuple-test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(lambda-tuple-test PRIVATE lambda-tuple-compile-options lambda-tuple-coverage-options
                                                lambda-tuple-sources)

target_precompile_headers(lambda-tuple-test PRIVATE "test/precompiled_header.hpp")
