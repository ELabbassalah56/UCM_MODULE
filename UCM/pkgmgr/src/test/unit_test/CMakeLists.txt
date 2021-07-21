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

set(TEST_SOURCES
	package_manager_impl/package_manager_impl_test.cpp
	package_manager_impl/transfer_instance_test.cpp
    

	extraction/software_package_extractor_test.cpp
	package_manager/package_manager_test.cpp

	interruptible_thread/interrupt_token_test.cpp
	interruptible_thread/interruptible_thread_test.cpp

	transfer/software_package_data_test.cpp
	filesystem_test.cpp
	synchronized_storage_test.cpp
	id_generator_test.cpp
	manifest_samples.cpp

	parsing/application_list_builder_test.cpp
	parsing/software_package_test.cpp
	parsing/software_cluster_test.cpp
	parsing/application_test.cpp
	parsing/software_cluster_manifest_parser_test.cpp
	parsing/software_package_manifest_parser_test.cpp
	parsing/manifest_parser_test.cpp
	parsing/version_report_test.cpp
	parsing/software_cluster_list_builder_test.cpp

	storage/action_generator_test.cpp
	storage/install_action_test.cpp
	storage/remove_action_test.cpp
	storage/update_action_test.cpp
	storage/filesystem_swcl_manager_test.cpp
	storage/processes_list_manager_test.cpp)

set(TEST_TARGET apd-ucm-testpackage-manager)

add_executable(${TEST_TARGET} ${TEST_SOURCES} $<TARGET_OBJECTS:pm_common>)

target_link_libraries(${TEST_TARGET} ${GTEST_BOTH_LIBRARIES} ${GMOCK} ${ARA_LIBRARIES})
target_link_libraries(${TEST_TARGET} ara::exec_execution_client)
target_link_libraries(${TEST_TARGET} Boost::thread)

target_include_directories(${TEST_TARGET}
	PRIVATE
	  ${CMAKE_CURRENT_SOURCE_DIR}
	  ${pm_object_include_dirs}
)

if (ARA_RUN_TESTS)
	docker_exec_test(${TEST_TARGET} /build/src/test/unit_test)
endif()

add_dependencies(${TEST_TARGET} pmMiddlewareInterfaces)

if(NOT CMAKE_INSTALL_TESTDIR)
  set(CMAKE_INSTALL_TESTDIR ${CMAKE_INSTALL_PREFIX}/bintest)
endif()

message(STATUS "option CMAKE_INSTALL_TESTDIR=" ${CMAKE_INSTALL_TESTDIR})

install(TARGETS ${TEST_TARGET} RUNTIME DESTINATION ${CMAKE_INSTALL_TESTDIR})
