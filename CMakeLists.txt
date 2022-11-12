# Copyright (c) 2022 Dennis Hezel
#
# This software is released under the MIT License.
# https://opensource.org/licenses/MIT

cmake_minimum_required(VERSION 3.14...3.24)

project(
    lambda-tuple
    VERSION 1.0.0
    DESCRIPTION "An implementation of `std::tuple` based on variadic lambda capture"
    LANGUAGES CXX)

include(GNUInstallDirs)

# user options
option(LAMBDA_TUPLE_INSTALL "Create the install target" on)
set(LAMBDA_TUPLE_CMAKE_CONFIG_INSTALL_DIR
    "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
    CACHE STRING "Install directory for CMake config files")

# maintainer options
option(LAMBDA_TUPLE_BUILD_TESTS "Build tests and examples" off)
option(LAMBDA_TUPLE_TEST_COVERAGE "Compile tests with --coverage" off)
set(LAMBDA_TUPLE_COVERAGE_OUTPUT_FILE
    "${CMAKE_CURRENT_BINARY_DIR}/sonarqube-coverage.xml"
    CACHE FILEPATH "Output file for SonarQube XML coverage files")
option(LAMBDA_TUPLE_ENABLE_DYNAMIC_ANALYSIS "Compile tests with -fsanitize=undefined,address or /fsanitize=address" off)

set(LAMBDA_TUPLE_PROJECT_ROOT "${CMAKE_CURRENT_LIST_DIR}")

if(LAMBDA_TUPLE_BUILD_TESTS)
    message(STATUS "CMake version: ${CMAKE_VERSION}")
    message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")

    # include CMake modules
    include("${CMAKE_CURRENT_LIST_DIR}/cmake/LambdaTupleCompileOptions.cmake")
    include("${CMAKE_CURRENT_LIST_DIR}/cmake/LambdaTupleInstallGitHooks.cmake")
    include("${CMAKE_CURRENT_LIST_DIR}/cmake/LambdaTupleCoverage.cmake")
endif()

add_subdirectory(src)

if(LAMBDA_TUPLE_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

if(LAMBDA_TUPLE_INSTALL)
    include("${CMAKE_CURRENT_LIST_DIR}/cmake/LambdaTupleInstallation.cmake")
endif()
