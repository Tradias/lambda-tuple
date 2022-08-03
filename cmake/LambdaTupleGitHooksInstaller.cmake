# Copyright (c) 2022 Dennis Hezel
# 
# This software is released under the MIT License.
# https://opensource.org/licenses/MIT

file(
    COPY "${GIT_HOOKS_SOURCE_DIR}/pre-commit" "${GIT_HOOKS_SOURCE_DIR}/LambdaTuplePreCommit.cmake"
    DESTINATION "${GIT_HOOKS_TARGET_DIR}"
    FILE_PERMISSIONS
        OWNER_READ
        GROUP_READ
        WORLD_READ
        OWNER_WRITE
        OWNER_EXECUTE
        GROUP_EXECUTE
        WORLD_EXECUTE)
