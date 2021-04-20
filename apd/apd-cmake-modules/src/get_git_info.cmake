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

# get_git_info()
#
# This function will fetch information from the source code git repository
# and set the ${GIT_BRANCH} and ${GIT_HASH} and ${GIT_TAG} variables.
function(get_git_info)
    find_program(GIT_EXE git)

    if(GIT_EXE)
        # Get branch name
        execute_process(COMMAND ${GIT_EXE} rev-parse --abbrev-ref HEAD
                        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                        OUTPUT_VARIABLE git_branch
                        OUTPUT_STRIP_TRAILING_WHITESPACE
                        ERROR_QUIET
        )
        # Get commit hash
        execute_process(COMMAND ${GIT_EXE} rev-parse --short HEAD
                        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                        OUTPUT_VARIABLE git_hash
                        OUTPUT_STRIP_TRAILING_WHITESPACE
                        ERROR_QUIET
        )
        # Get annotated tag
        execute_process(COMMAND ${GIT_EXE} describe --tags
                        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                        OUTPUT_VARIABLE git_tag
                        OUTPUT_STRIP_TRAILING_WHITESPACE
                        ERROR_QUIET
        )
        if(NOT git_tag)
            # Get simple tag
            execute_process(COMMAND ${GIT_EXE} tag
                            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                            OUTPUT_VARIABLE git_tag
                            OUTPUT_STRIP_TRAILING_WHITESPACE
                            ERROR_QUIET
            )
        endif()
    else()
        message(WARNING "Git executable not found. Commit information won't be available.")
    endif()

    set(GIT_BRANCH "${git_branch}" PARENT_SCOPE)
    set(GIT_HASH "${git_hash}" PARENT_SCOPE)
    set(GIT_TAG "${git_tag}" PARENT_SCOPE)
endfunction()
