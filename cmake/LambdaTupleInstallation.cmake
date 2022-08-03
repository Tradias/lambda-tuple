# Copyright (c) 2022 Dennis Hezel
#
# This software is released under the MIT License.
# https://opensource.org/licenses/MIT

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/generated/${PROJECT_NAME}ConfigVersion.cmake" ARCH_INDEPENDENT
    VERSION "${PROJECT_VERSION}"
    COMPATIBILITY SameMajorVersion)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/generated/${PROJECT_NAME}ConfigVersion.cmake"
        DESTINATION "${LAMBDA_TUPLE_CMAKE_CONFIG_INSTALL_DIR}")

install(TARGETS lambda-tuple EXPORT ${PROJECT_NAME}Targets)

install(
    EXPORT ${PROJECT_NAME}Targets
    NAMESPACE ${PROJECT_NAME}::
    FILE ${PROJECT_NAME}Config.cmake
    DESTINATION "${LAMBDA_TUPLE_CMAKE_CONFIG_INSTALL_DIR}")

install(
    DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src/ltpl"
    TYPE INCLUDE
    FILES_MATCHING
    PATTERN "*.hpp")
