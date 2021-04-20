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

# add_adaptive_library <library_name>
#     [LIBS <link_libraries ...>]
#     [PRIVATE_LIBS <link_libraries ...>]
#     [COMPILE_OPTIONS <compile_options ...>]
#     [COMPILE_DEFINITIONS <compile_definitions>]
#     [HEADERS <headers ...>]
#     [INCLUDE_DIRECTORIES <includedirs ...>]
#     [NAMESPACE <namespace>]
#     [OUTPUT_NAME <outputname>]
#     [ALLOW_UNDEFINED]
#     SOURCES <sources ...>
# )
#
# Adds a library and create all the modern cmake targets to import.
#
# Optional target specific compile flags can be passed with COMPILE_OPTIONS.

include(AUTOSAR/get_git_info)

function(add_adaptive_library library_name)
    include(GNUInstallDirs)
    include(CMakeParseArguments)
    include(GenerateExportHeader)
    include(CMakePackageConfigHelpers)

    set(multiValueArgs
        LIBS
        PRIVATE_LIBS
        COMPILE_OPTIONS
        COMPILE_DEFINITIONS
        SOURCES
        HEADERS
        INCLUDE_DIRECTORIES
        )
    set(oneValueArgs NAMESPACE OUTPUT_NAME)
    cmake_parse_arguments(_lib "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(NOT _lib_SOURCES)
        message(FATAL_ERROR "Error adding library ${library_name}: No SOURCES specified.")
    endif()

    set(library_namespace ara)
    if(_lib_NAMESPACE)
        set(library_namespace ${_lib_NAMESPACE})
    endif()

    # Not allow use library names prefixed with lib
    string(REPLACE "lib" "" library_name ${library_name})
    # Concat namespace on internal_library_name)
    string(CONCAT real_library_name ${library_namespace} "_" ${library_name})
    set(output_name ${real_library_name})
    if(_lib_OUTPUT_NAME)
        string(REPLACE "lib" "" output_name ${_lib_OUTPUT_NAME})
    endif()

    # Proper set the export name
    set(cmake_export_name ${library_namespace}-${library_name})

    add_library(${library_name} ${_lib_SOURCES})
    target_link_libraries(${library_name}
        PUBLIC
            ${_lib_LIBS}
        PRIVATE
            ${_lib_PRIVATE_LIBS}
        )
    # We need iteract over the multiple directory list
    foreach(include ${_lib_INCLUDE_DIRECTORIES})
        list(APPEND build_interface_includes "$<BUILD_INTERFACE:${include}>")
    endforeach()

    # We use the namespace equaly as well on installed exported directories
    # so we need to replace the colons with proper slashes
    string(REPLACE "::" "/" library_namespace ${library_namespace})

    target_include_directories(${library_name}
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
            $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>
            ${build_interface_includes}
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${library_namespace}>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${library_namespace}/${headers_directory}>
        )
    target_compile_options(${library_name} PRIVATE
        ${_lib_COMPILE_OPTIONS}
        )
    if(_lib_COMPILE_DEFINITIONS)
        target_compile_definitions(${library_name} PRIVATE
            ${_lib_COMPILE_DEFINITIONS}
            )
    endif()

    ######## Version header setup ########
    get_git_info()
    if(GIT_HASH)
        set(PROJECT_VERSION ${PROJECT_VERSION}_${GIT_HASH})
    endif()

    set_target_properties(${library_name} PROPERTIES
        SOVERSION ${PROJECT_VERSION_MAJOR}
        VERSION ${PROJECT_VERSION}
        CXX_STANDARD 14
        LINK_FLAGS "-Wl,--as-needed -Wl,--no-undefined"
        OUTPUT_NAME ${output_name}
        EXPORT_NAME ${library_name}
        )
    add_library(${library_namespace}::${library_name} ALIAS ${library_name})

    generate_export_header(${library_name} EXPORT_FILE_NAME ${real_library_name}_export.h)

    install(TARGETS ${library_name} EXPORT ${library_name}Targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT ${library_name}-staticdev
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT ${library_name}-dev
        )

    foreach(header IN LISTS _lib_HEADERS)
        if(IS_DIRECTORY ${header})
            list(APPEND directory_headers ${header})
        else()
            list(APPEND file_headers ${header})
        endif()
    endforeach()

    if(DEFINED directory_headers)
        install(DIRECTORY
            ${directory_headers}
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${library_namespace}/${library_name}
            COMPONENT ${library_name}-dev
            FILES_MATCHING PATTERN "*.h"
            )
    endif()

    if(DEFINED file_headers)
        install(FILES
            ${_lib_HEADERS}
            ${export_HEADERS}
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${library_namespace}/${library_name}
            COMPONENT ${library_name}-dev
            )
    endif()

    write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/${library_name}/${cmake_export_name}ConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion
        )

    export(EXPORT ${library_name}Targets
        FILE ${cmake_export_name}Targets.cmake
        TARGETS ${library_name}
        NAMESPACE ${_lib_NAMESPACE}::
        EXPORT_LINK_INTERFACE_LIBRARIES
)

    # Export the aliases
    install(EXPORT ${library_name}Targets
        FILE ${cmake_export_name}Targets.cmake
        NAMESPACE ${_lib_NAMESPACE}::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${cmake_export_name}
        COMPONENT ${library_name}-dev
    )

    write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/${cmake_export_name}ConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion
    )

    string(TOUPPER ${cmake_export_name} upper_library_name)
    string(REPLACE "-" "_" upper_library_name ${upper_library_name})
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/${cmake_export_name}Config.cmake
        "include(\${CMAKE_CURRENT_LIST_DIR}/${cmake_export_name}Targets.cmake)"
        "\n\n# Old compat to previos cmake usage without aliases"
        "\nfind_package(Threads REQUIRED)"
        "\nset(${upper_library_name}_LIBRARIES ${_lib_NAMESPACE}::${library_name})"
    )

    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/${cmake_export_name}Config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${cmake_export_name}ConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${cmake_export_name}
        COMPONENT ${library_name}-dev
    )

    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/${cmake_export_name}-${PROJECT_VERSION}.pc
            "prefix=${CMAKE_INSTALL_PREFIX}
exec_prefix=${CMAKE_INSTALL_PREFIX}
libdir=${CMAKE_INSTALL_FULL_LIBDIR}
includedir=${CMAKE_INSTALL_FULL_INCLUDEDIR}/${library_namespace}/${library_name}
libname=${real_library_name}-${PROJECT_VERSION}

Name=${library_name}
Description:=${real_library_name} library
Version=${PROJECT_VERSION}
Libs: -L\${libdir} -l${real_library_name}
Cflags: -I\${includedir}
"
        )

    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${cmake_export_name}-${PROJECT_VERSION}.pc
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
        COMPONENT dev
        )
endfunction()
