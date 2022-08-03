# Copyright (c) 2022 Dennis Hezel
#
# This software is released under the MIT License.
# https://opensource.org/licenses/MIT

# lambda-tuple
add_library(lambda-tuple INTERFACE)

add_library(${PROJECT_NAME}::lambda-tuple ALIAS lambda-tuple)

target_compile_features(lambda-tuple INTERFACE cxx_std_20)

target_include_directories(lambda-tuple INTERFACE "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
                                                  $<INSTALL_INTERFACE:include>)

# lambda-tuple sources
if(LAMBDA_TUPLE_BUILD_TESTS)
    add_library(lambda-tuple-sources OBJECT)

    target_sources(lambda-tuple-sources PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/tuple.cpp")

    target_link_libraries(lambda-tuple-sources PUBLIC lambda-tuple)
endif()
