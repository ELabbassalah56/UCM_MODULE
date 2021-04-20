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


# Define default settings for compiler options, build type, etc.
include(CMakeDependentOption)

macro(apd_defaults)

    # Set our preferred CMAKE_BUILD_TYPE.
    #
    # You can not simply use set(CMAKE_BUILD_TYPE ... CACHE ...) as CMake actually defaults
    # to a defined (but empty) value for CMAKE_BUILD_TYPE. Thus the override never takes
    # effect.
    #
    if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE "Debug" CACHE STRING
            "Choose the type of build, one of: Debug, Release, RelWithDebInfo, MinSizeRel"
            FORCE)
    endif()

    # Set the preferred C++ compile options for the selected build type
    string(TOUPPER "${CMAKE_BUILD_TYPE}" _apd_ucase_build_type)

    # We always want these flags enabled
    list(APPEND _apd_common_cxx_flags
        -Wall
        -Wctor-dtor-privacy
        -Wdeprecated
        -Wextra
        -Wfloat-equal
        -Winit-self
        -Wnon-virtual-dtor
        -Wold-style-cast
        -Woverloaded-virtual
        -Wpedantic
        -Wpointer-arith
        -Wshadow
        -Wsign-promo
        -Wuninitialized
        -Wunused
        -fno-strict-aliasing
        )

    # Now append the flags to the build-type-specific variable. This way, we always
    # enable the flags regardless of the build type selected (giving consistency).
    #
    # Use APPEND to allow other flags to be added in the toolchain file or from
    # the command line.
    #
    # Set CXX_FLAGS SHOULD NOT BE USED in modern CMake. The proper way to use definitions
    # and options would be target_compile_options and target_compile_definitions, which then avoid
    # polute non intended code build
    # Since the current status of demonstrator cmake files are not using this modern method
    # we need to keep this, but should be considered deprecated.

    string(REPLACE ";" " " _apd_common_cxx_flags "${_apd_common_cxx_flags}")
    string(APPEND CMAKE_CXX_FLAGS_${_apd_ucase_build_type} " ${_apd_common_cxx_flags}")
    unset(_apd_ucase_build_type)
    unset(_apd_common_cxx_flags)

    # Standard settings
    #
    #   Disable GCC extensions (this is consistent with specifying -pedantic above)
    #   Define our preferred language variant
    #
    # We do not use the CACHE for these, as we do not want them overridden from the
    # command line.

    set(CMAKE_CXX_EXTENSIONS OFF)
    set(CMAKE_CXX_STANDARD 14)

    # Introduce option for building the unit test executable by default.
    #
    # However, running the unit tests using docker or building
    # the documentation make target shall be user-defined
    option(ARA_ENABLE_TESTS "Enable unit testing" ON)
    option(ARA_RUN_TESTS "Enable running unit testing" OFF)
    option(ARA_ENABLE_DOXYGEN "Enables the documentation target" OFF)

    # Testing / Debugging
    cmake_dependent_option(ARA_ENABLE_DEBUG "Enable debug for docker target" OFF
        "ARA_RUN_TESTS" OFF)
    message(STATUS "option ARA_RUN_TESTS=" ${ARA_RUN_TESTS})
    message(STATUS "option ARA_ENABLE_DEBUG=" ${ARA_ENABLE_DEBUG} " (depends on ARA_RUN_TESTS)")

    if(ARA_RUN_TESTS)
        # Enable running tests inside docker container
        # If ARA_ENABLE_DEBUG is ON and CMAKE_BUILD_TYPE is Debug or RelWithDebInfo
        # a gdbserver target will be created
        include(AUTOSAR/docker)
        docker_setup_container()
    endif()
endmacro()


# Everyone needs the default settings

apd_defaults()
