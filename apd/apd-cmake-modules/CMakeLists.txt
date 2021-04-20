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

cmake_minimum_required(VERSION 3.4)
project(apd-cmake-modules VERSION 1.0.0 LANGUAGES NONE)

# As a sanity test, include our CMake module to ensure it is syntax error free
# and display the results

include(src/apd.cmake)
include(src/docker.cmake)

message(STATUS "Preferred Build Type:     ${CMAKE_BUILD_TYPE}")
message(STATUS "CXX_FLAGS_RELEASE:        ${CMAKE_CXX_FLAGS_RELEASE}")
message(STATUS "CXX_FLAGS_DEBUG:          ${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "CXX_FLAGS_RELWITHDEBINFO: ${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
message(STATUS "VERBOSE_MAKEFILE:         ${CMAKE_VERBOSE_MAKEFILE}")
message(STATUS "CXX_EXTENSIONS:           ${CMAKE_CXX_EXTENSIONS}")
message(STATUS "CXX_STANDARD:             ${CMAKE_CXX_STANDARD}")

include(GNUInstallDirs)

#
# The somewhat bizarre install location is needed to allow this module
# to work both as a -native and nativesdk- package in Yocto. CMAKE_ROOT
# includes the full absolute path to the native CMake built by Yocto,
# and this does not get stripped properly when building for the SDK.
#
# This approach works for both.
#

set(APD_CMAKE_SOURCES
    src/add_adaptive_library.cmake
    src/apd.cmake
    src/docker.cmake
    src/get_git_info.cmake
    )

install(FILES ${APD_CMAKE_SOURCES}
        DESTINATION ${CMAKE_INSTALL_DATADIR}/cmake-${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}/Modules/AUTOSAR
)
