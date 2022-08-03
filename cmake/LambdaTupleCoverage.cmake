# Copyright (c) 2022 Dennis Hezel
# 
# This software is released under the MIT License.
# https://opensource.org/licenses/MIT

add_library(lambda-tuple-coverage-options INTERFACE)

if(LAMBDA_TUPLE_TEST_COVERAGE)
    target_compile_options(lambda-tuple-coverage-options INTERFACE --coverage
                                                                   $<$<CXX_COMPILER_ID:GNU>:-fprofile-abs-path>)

    target_link_options(lambda-tuple-coverage-options INTERFACE --coverage)

    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        find_program(LAMBDA_TUPLE_GCOV_PROGRAM gcov)
    endif()
    if(NOT LAMBDA_TUPLE_GCOV_PROGRAM)
        find_program(LAMBDA_TUPLE_LLVM_COV_PROGRAM NAMES llvm-cov llvm-cov-10 llvm-cov-11 llvm-cov-12 llvm-cov-13
                                                         llvm-cov-14)
        set(_LAMBDA_TUPLE_GCOV_COMMAND "${LAMBDA_TUPLE_LLVM_COV_PROGRAM} gcov")
    else()
        set(_LAMBDA_TUPLE_GCOV_COMMAND "${LAMBDA_TUPLE_GCOV_PROGRAM}")
    endif()
    find_program(LAMBDA_TUPLE_GCOVR_PROGRAM gcovr REQUIRED)
    add_custom_target(
        lambda-tuple-test-coverage
        COMMAND "${LAMBDA_TUPLE_GCOVR_PROGRAM}" --gcov-executable "${_LAMBDA_TUPLE_GCOV_COMMAND}" --sonarqube --output
                "${LAMBDA_TUPLE_COVERAGE_OUTPUT_FILE}" --root "${LAMBDA_TUPLE_PROJECT_ROOT}"
        WORKING_DIRECTORY "${LAMBDA_TUPLE_PROJECT_ROOT}"
        VERBATIM)
endif()
