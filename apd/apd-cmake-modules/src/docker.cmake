#  --------------------------------------------------------------------------
#  |              _    _ _______     .----.      _____         _____        |
#  |         /\  | |  | |__   __|  .  ____ .    / ____|  /\   |  __ \       |
#  |        /  \ | |  | |  | |    .  / __ \ .  | (___   /  \  | |__) |      |
#  |       / /\ \| |  | |  | |   .  / / / / v   \___ \ / /\ \ |  _  /       |
#  |      / /__\ \ |__| |  | |   . / /_/ /  .   ____) / /__\ \| | \ \       |
#  |     /________\____/   |_|   ^ \____/  .   |_____/________\_|  \_\      |
#  |                              . _ _  .                                  |
#  --------------------------------------------------------------------------
#
#  All Rights Reserved.
#  Any use of this source code is subject to a license agreement with the
#  AUTOSAR development cooperation.
#  More information is available at www.autosar.org.
#
#  Disclaimer
#
#  This work (specification and/or software implementation) and the material
#  contained in it, as released by AUTOSAR, is for the purpose of information
#  only. AUTOSAR and the companies that have contributed to it shall not be
#  liable for any use of the work.
#
#  The material contained in this work is protected by copyright and other
#  types of intellectual property rights. The commercial exploitation of the
#  material contained in this work requires a license to such intellectual
#  property rights.
#
#  This work may be utilized or reproduced without any modification, in any
#  form or by any means, for informational purposes only. For any other
#  purpose, no part of the work may be utilized or reproduced, in any form
#  or by any means, without permission in writing from the publisher.
#
#  The work has been developed for automotive applications only. It has
#  neither been developed, nor tested for non-automotive applications.
#
#  The word AUTOSAR and the AUTOSAR logo are registered trademarks.
#  --------------------------------------------------------------------------

include(CMakeParseArguments)

find_program(DOCKER docker REQUIRED)

set(DOCKER_IMAGE_NAME "ara-apd:test" CACHE STRING
    "Name of the docker image to use" FORCE)

set(DOCKER_CONTAINER_NAME "unit_test" CACHE STRING
    "Name of the docker container" FORCE)

mark_as_advanced(DOCKER_IMAGE_NAME
                 DOCKER_CONTAINER_NAME)

# Internal function to convert host path to docker path
# param TEST path to binary
# param LOCATION stores the result of the conversion
function(_docker_path_to_mount TEST LOCATION)
    string(REPLACE ${CMAKE_BINARY_DIR} "/build" MOUNT_PATH ${TEST})
    set(${LOCATION} ${MOUNT_PATH} PARENT_SCOPE)
endfunction(_docker_path_to_mount)

# Enable a docker container for running a binary
function(docker_setup_container)
    set(options NONE)
    set(oneValueArgs NONE)
    set(multiValueArgs NONE)
    cmake_parse_arguments(Docker
                          "${options}"
                          "${oneValueArgs}"
                          "${multiValueArgs}"
                          ${ARGN})

    set(DOCKER_ARGS
        --rm
        --interactive
        --tty
        --detach
        --volume ${CMAKE_BINARY_DIR}:/build
        --name ${DOCKER_CONTAINER_NAME})
    if(ARA_ENABLE_DEBUG)
        if (CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
            set(DOCKER_ARGS
                ${DOCKER_ARGS}
                --publish 127.0.0.1:10000:10000
                --security-opt="apparmor=unconfined"
                --security-opt="seccomp=unconfined"
                --cap-add=SYS_PTRACE)
            add_custom_target(gdbserver
                            COMMAND
                            ${DOCKER} container exec
                            ${DOCKER_CONTAINER_NAME}
                            gdbserver localhost:10000 \${DEBUG_BINARY})
        else()
            message(WARNING "Enabled debugging but CMAKE_BUILD_TYPE=" ${CMAKE_BUILD_TYPE} " (need Debug or RelWithDebInfo), disabling gdbserver target.")
        endif()
    endif()
    add_custom_target(dock
                      COMMAND
                          ${DOCKER} run
                          ${DOCKER_ARGS}
                          ${DOCKER_IMAGE_NAME} /bin/ash)

    add_custom_target(rundlt
                      COMMAND
                          ${DOCKER} exec
                          --detach
                          ${DOCKER_CONTAINER_NAME} dlt-daemon)

    add_custom_target(undock
                      COMMAND
                      ${DOCKER} kill ${DOCKER_CONTAINER_NAME})
endfunction(docker_setup_container)

# Enable running a test binary in docker via ctest [-V]
# param test_name Name of the test binary
# param test_location Path to the binary
function(docker_exec_test test_name test_location)
    _docker_path_to_mount(${test_location} LOCATION)
    add_test(${test_name}
             ${DOCKER} exec ${DOCKER_CONTAINER_NAME} ${LOCATION}/${test_name} ${ARA_TEST_ARGS})
endfunction(docker_exec_test)
