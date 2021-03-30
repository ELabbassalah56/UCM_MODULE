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

set(UCM_ARXMLS_SEARCH_PATHS /usr/${CMAKE_INSTALL_DATAROOTDIR})

set(MACHINE  /ara/apd/machines/DevelopmentMachine)
set(GEN_OUTPUT_DIR ${PROJECT_BINARY_DIR}/gen)

set(VSOMEIP_CONFIG_FILENAME ${GEN_OUTPUT_DIR}/vsomeip/developmentmachine_vsomeip.json)
set(EM_CONFIG_FILENAME ${GEN_OUTPUT_DIR}/processes/package_manager_MANIFEST.json)

set(BINARY_MAKE_TARGET pmMiddlewareInterfaces)
set(VSOMEIP_CONFIG_MAKE_TARGET ${PROJECT_NAME}JSONGenerated)
set(EM_CONFIG_MAKE_TARGET ${PROJECT_NAME}_EM_configs)

find_path(UCM_ARXMLS_DIR
  ucm.arxml
	PATHS
    ${UCM_ARXMLS_SEARCH_PATHS}
  PATH_SUFFIXES
    ucm
	ONLY_CMAKE_FIND_ROOT_PATH
)

set(UCM_ARXML_MANIFESTS
  ${UCM_ARXMLS_DIR}/ucm_deployment_manifest.arxml
  ${UCM_ARXMLS_DIR}/ucm_instance_manifest.arxml
  ${UCM_ARXMLS_DIR}/ucm_mapping_manifest.arxml
  ${UCM_ARXMLS_DIR}/ucm_timing_manifest.arxml
  ${UCM_ARXMLS_DIR}/ucm.arxml
)

set(FULL_SET_OF_ARXML
  ${ARA_ARXMLS_DIR}/stdtypes.arxml
  ${ARA_ARXMLS_DIR}/stdcpptypes.arxml
  ${ARA_ARXMLS_DIR}/machine_manifest.arxml
  ${ARA_ARXMLS_DIR}/network_manifest.arxml
  ${ARA_ARXMLS_DIR}/package_management.arxml
  ${ARA_ARXMLS_DIR}/system_components.arxml
  ${UCM_ARXML_MANIFESTS}
)

message(STATUS "UCM_ARXMLS_DIR = ${UCM_ARXMLS_DIR}")

add_aragen(
	DESTINATION ${GEN_OUTPUT_DIR}
	TARGET ${BINARY_MAKE_TARGET}
	SWC /ara/apd/ucm/software_components/package_manager_swc
	ARXMLS ${FULL_SET_OF_ARXML}
)

set(MAIN_src
	main.cpp
)

add_library(pm_common OBJECT
  adaptive_application.cpp
  package_management_application.cpp
	filesystem.cpp

	interruptible_thread/interrupt_token.cpp
	interruptible_thread/interruptible_thread.cpp

	package_manager/package_manager.cpp
	package_manager_impl/package_manager_impl.cpp
	package_manager_impl/package_manager_impl_factory.cpp

	transfer/software_package_data.cpp
	transfer/transfer_instance.cpp

	package_manager_state/package_manager_state.cpp
	package_manager_state/package_manager_ready_state.cpp
	package_manager_state/package_manager_processing_state.cpp
	package_manager_state/package_manager_cleaningup_state.cpp
	package_manager_state/package_manager_activated_state.cpp
	package_manager_state/package_manager_rollingback_state.cpp
	package_manager_state/package_manager_rolledback_state.cpp
	package_manager_state/package_manager_verifying_state.cpp
	package_manager_state/package_manager_activating_state.cpp
	package_manager_state/package_manager_idle_state.cpp

	extraction/lib_poco_zip_extractor.cpp

	storage/action_generator.cpp
	storage/execution_manager_stub.cpp
	storage/install_action.cpp
	storage/remove_action.cpp
	storage/update_action.cpp
	storage/filesystem_swcl_manager.cpp
	storage/processes_list_manager.cpp

	parsing/software_package_parser_impl.cpp
	parsing/software_cluster_manifest.cpp
	parsing/software_cluster_manifest_parser.cpp
	parsing/software_package_manifest_parser.cpp
	parsing/application.cpp
	parsing/application_list_builder.cpp
	parsing/software_cluster.cpp
	parsing/software_package_manifest.cpp
	parsing/software_package.cpp
	parsing/software_cluster_list_builder.cpp
	parsing/version_report_impl.cpp
	parsing/version.cpp
	parsing/ucm_parsing_info.cpp
	parsing/ucm_info.cpp
    

	${pmMiddlewareInterfaces_SOURCES}
)

add_jsongen(
	DESTINATION ${GEN_OUTPUT_DIR}
	TARGET ${VSOMEIP_CONFIG_MAKE_TARGET}
	MACHINE ${MACHINE}
	ROUTING "vsomeipd"
	LOG_LEVEL "debug"
	LOG_DLT
	LOG_CONSOLE
	ARXMLS ${FULL_SET_OF_ARXML}
)

add_em_gen(
	DESTINATION ${GEN_OUTPUT_DIR}
	TARGET ${EM_CONFIG_MAKE_TARGET}
	MACHINE ${MACHINE}
	ARXMLS ${FULL_SET_OF_ARXML}
)

add_executable(${PROJECT_NAME}
	${MAIN_src}
	$<TARGET_OBJECTS:pm_common>
)

target_include_directories(pm_common
  PUBLIC
	  ${CMAKE_CURRENT_SOURCE_DIR}
	  ${GEN_OUTPUT_DIR}/includes/
	  ${GEN_OUTPUT_DIR}/vsomeip/
	  ${VSOMEIP_INCLUDE_DIRS}
)

get_property(pm_object_include_dirs TARGET pm_common PROPERTY INCLUDE_DIRECTORIES)

target_compile_options(${PROJECT_NAME} PRIVATE -pthread)
target_include_directories(${PROJECT_NAME} PRIVATE ${pm_object_include_dirs})

add_dependencies(pm_common ${BINARY_MAKE_TARGET})

add_dependencies(${PROJECT_NAME} ${BINARY_MAKE_TARGET} ${VSOMEIP_CONFIG_MAKE_TARGET} ${EM_CONFIG_MAKE_TARGET})

target_link_libraries (${PROJECT_NAME} ${ARA_LIBRARIES})
target_link_libraries (${PROJECT_NAME} ara::exec_execution_client)
target_link_libraries(${PROJECT_NAME} Boost::filesystem)
target_link_libraries(${PROJECT_NAME} Boost::thread)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION /opt/${PROJECT_NAME}/bin)

install(FILES
	${VSOMEIP_CONFIG_FILENAME} PERMISSIONS OWNER_READ GROUP_READ WORLD_READ DESTINATION /opt/${PROJECT_NAME}/etc RENAME vsomeip.json)
install(FILES
	${EM_CONFIG_FILENAME} PERMISSIONS OWNER_READ GROUP_READ WORLD_READ DESTINATION /opt/${PROJECT_NAME}/etc RENAME MANIFEST.json)

install(FILES
	${PROJECT_SOURCE_DIR}/files/etc/PROCESSES.json
	PERMISSIONS OWNER_READ GROUP_READ WORLD_READ DESTINATION ${UCM_SWCL_DIR}/sample)

install(DIRECTORY DESTINATION ${UCM_SWCL_DIR}/tmp)

if (ARA_ENABLE_TESTS)
	add_subdirectory(test)
endif()
