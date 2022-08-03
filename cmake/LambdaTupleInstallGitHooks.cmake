# Copyright (c) 2022 Dennis Hezel
# 
# This software is released under the MIT License.
# https://opensource.org/licenses/MIT

find_package(Git)

function(lambda_tuple_create_init_git_hooks_target)
    if(TARGET lambda-tuple-init-git-hooks)
        return()
    endif()

    set(LAMBDA_TUPLE_GIT_HOOKS_TARGET_DIR "${CMAKE_SOURCE_DIR}/.git/hooks")
    set(LAMBDA_TUPLE_GIT_HOOKS_SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/git-hooks/")

    if(NOT EXISTS "${LAMBDA_TUPLE_GIT_HOOKS_TARGET_DIR}/pre-commit"
       OR NOT EXISTS "${LAMBDA_TUPLE_GIT_HOOKS_TARGET_DIR}/LambdaTuplePreCommit.cmake")
        message(
            AUTHOR_WARNING
                "Initialize clang-format and cmake-format pre-commit hooks by building the CMake target lambda-tuple-init-git-hooks."
        )
    endif()

    find_program(LAMBDA_TUPLE_CMAKE_FORMAT_PROGRAM cmake-format)
    find_program(LAMBDA_TUPLE_CLANG_FORMAT_PROGRAM clang-format)

    if(NOT LAMBDA_TUPLE_CMAKE_FORMAT_PROGRAM OR NOT LAMBDA_TUPLE_CLANG_FORMAT_PROGRAM)
        message(
            AUTHOR_WARNING
                "Cannot create init-git-hooks target with\ncmake-format: ${LAMBDA_TUPLE_CMAKE_FORMAT_PROGRAM}\nclang-format: ${LAMBDA_TUPLE_CLANG_FORMAT_PROGRAM}"
        )
        return()
    endif()

    set(LAMBDA_TUPLE_INIT_GIT_HOOKS_SOURCES "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/hooks/pre-commit.in"
                                            "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/hooks/LambdaTuplePreCommit.cmake.in")
    configure_file("${CMAKE_CURRENT_FUNCTION_LIST_DIR}/hooks/pre-commit.in"
                   "${LAMBDA_TUPLE_GIT_HOOKS_SOURCE_DIR}/pre-commit" @ONLY NEWLINE_STYLE UNIX)
    configure_file("${CMAKE_CURRENT_FUNCTION_LIST_DIR}/hooks/LambdaTuplePreCommit.cmake.in"
                   "${LAMBDA_TUPLE_GIT_HOOKS_SOURCE_DIR}/LambdaTuplePreCommit.cmake" @ONLY NEWLINE_STYLE UNIX)

    set(_lambda_tuple_command_arguments
        "-DGIT_HOOKS_TARGET_DIR=${LAMBDA_TUPLE_GIT_HOOKS_TARGET_DIR}"
        "-DGIT_HOOKS_SOURCE_DIR=${LAMBDA_TUPLE_GIT_HOOKS_SOURCE_DIR}" -P
        "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/LambdaTupleGitHooksInstaller.cmake")
    string(REPLACE ";" " " _lambda_tuple_pretty_command_arguments "${_lambda_tuple_command_arguments}")
    add_custom_target(
        lambda-tuple-init-git-hooks
        DEPENDS ${LAMBDA_TUPLE_INIT_GIT_HOOKS_SOURCES}
        SOURCES ${LAMBDA_TUPLE_INIT_GIT_HOOKS_SOURCES}
        COMMAND ${CMAKE_COMMAND} ${_lambda_tuple_command_arguments}
        COMMENT "cmake ${_lambda_tuple_pretty_command_arguments}"
        VERBATIM)
endfunction()

if(GIT_FOUND)
    lambda_tuple_create_init_git_hooks_target()
endif()
