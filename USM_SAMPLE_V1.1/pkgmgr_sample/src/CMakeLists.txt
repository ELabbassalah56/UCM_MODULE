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

set(APP_NAME pkgmgr_sample)
set(APP_EXECUTABLE ${APP_NAME})

set(GEN_DEST ${PROJECT_BINARY_DIR}/gen/${APP_NAME})

set(UCM_ARXMLS_SEARCH_PATHS /usr/${CMAKE_INSTALL_DATAROOTDIR})

find_file(UCM_DEPLOYMENT_MANIFEST
  ucm_deployment_manifest.arxml
	PATHS
    ${UCM_ARXMLS_SEARCH_PATHS}
  PATH_SUFFIXES
    ucm
	ONLY_CMAKE_FIND_ROOT_PATH
)

set(UCM_CLIENT_ARXML_MANIFESTS
  ${PROJECT_SOURCE_DIR}/${APP_NAME}/files/ucm_client_instance_manifest.arxml
  ${PROJECT_SOURCE_DIR}/${APP_NAME}/files/ucm_client_mapping_manifest.arxml
  ${PROJECT_SOURCE_DIR}/${APP_NAME}/files/ucm_client_timing_manifest.arxml
  ${PROJECT_SOURCE_DIR}/${APP_NAME}/files/ucm_client.arxml
)

add_aragen(
    DESTINATION ${GEN_DEST}
    TARGET pmMiddlewareInterfaces
    SWC /ara/apd/ucm_client/software_components/PackageManagerSample
    ARXMLS
        ${ARA_ARXMLS_DIR}/stdcpptypes.arxml
        ${ARA_ARXMLS_DIR}/package_management.arxml
        ${UCM_DEPLOYMENT_MANIFEST}
        ${UCM_CLIENT_ARXML_MANIFESTS}
)

set(MAIN_src
	${PROJECT_SOURCE_DIR}/${APP_NAME}/src/main.cpp)


set(PROJECT_SOURCES
	data_transfer.cpp
	find_service.cpp
	package_management_app.cpp
)

list(APPEND PROJECT_SOURCES ${pmMiddlewareInterfaces_SOURCES})

set(MACHINE /ara/apd/machines/DevelopmentMachine)

# vsomeip config
add_jsongen(
    DESTINATION ${GEN_DEST}
    TARGET ${APP_NAME}JSONGenerated
    MACHINE ${MACHINE}
    ROUTING "vsomeipd"
    LOG_LEVEL "debug"
    LOG_DLT
    LOG_CONSOLE
    ARXMLS
        ${ARA_ARXMLS_DIR}/stdcpptypes.arxml
        ${ARA_ARXMLS_DIR}/machine_manifest.arxml
        ${ARA_ARXMLS_DIR}/network_manifest.arxml
        ${ARA_ARXMLS_DIR}/package_management.arxml

        ${UCM_DEPLOYMENT_MANIFEST}

        ${UCM_CLIENT_ARXML_MANIFESTS}
)

add_executable(${APP_EXECUTABLE}
	${PROJECT_SOURCES}
	${MAIN_src})

target_include_directories(${APP_NAME}
	PRIVATE ${PROJECT_SOURCE_DIR}/${APP_NAME}/src/
	PRIVATE ${GEN_DEST}/includes/
	PRIVATE ${GEN_DEST}/vsomeip/
	PRIVATE ${VSOMEIP_INCLUDE_DIRS}
	)

target_link_libraries (${APP_EXECUTABLE} ${CMAKE_THREAD_LIBS_INIT} ${ARA_LOGGING_LIBRARIES} ara::exec_execution_client ara_com ara_vsomeipbinding ara::log)

add_dependencies(${APP_NAME}JSONGenerated pmMiddlewareInterfaces)
add_dependencies(${APP_EXECUTABLE} pmMiddlewareInterfaces ${APP_NAME}JSONGenerated)

install(TARGETS ${APP_EXECUTABLE} RUNTIME DESTINATION ${PKGMGRSAMPLE_DIR}/bin)

install(FILES ${${APP_NAME}JSONGenerated_OUTPUTS} PERMISSIONS OWNER_READ GROUP_READ WORLD_READ DESTINATION ${PKGMGRSAMPLE_DIR}/etc RENAME vsomeip.json)
install(FILES ${PROJECT_SOURCE_DIR}/${APP_NAME}/files/MANIFEST.json PERMISSIONS OWNER_READ GROUP_READ WORLD_READ DESTINATION ${PKGMGRSAMPLE_DIR}/etc)
install(FILES ../files/kvsdatabase.json PERMISSIONS OWNER_READ GROUP_READ WORLD_READ DESTINATION ${PKGMGRSAMPLE_DIR}/var)
