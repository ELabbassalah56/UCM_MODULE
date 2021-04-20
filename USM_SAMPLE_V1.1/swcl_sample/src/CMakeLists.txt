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

set(APP_NAME swcl_sample)
set(APP_EXECUTABLE ${APP_NAME})

set(MAIN_src
	${PROJECT_SOURCE_DIR}/${APP_NAME}/src/main.cpp)

add_executable(${APP_EXECUTABLE}
	${MAIN_src})

target_include_directories(${APP_NAME}
	PRIVATE ${PROJECT_SOURCE_DIR}/${APP_NAME}/src/
	)

target_link_libraries (${APP_EXECUTABLE} ${CMAKE_THREAD_LIBS_INIT} ${ARA_LOGGING_LIBRARIES} ara::exec_execution_client ara::log)

install(TARGETS ${APP_EXECUTABLE} RUNTIME DESTINATION ${UCM_SWCL_DIR}/sample/swclSample/applicationSample/exec1/bin)

install(FILES ${PROJECT_SOURCE_DIR}/${APP_NAME}/files/MANIFEST.json PERMISSIONS OWNER_READ GROUP_READ WORLD_READ DESTINATION
	${UCM_SWCL_DIR}/sample/swclSample/applicationSample/exec1/etc)

install(FILES ${PROJECT_SOURCE_DIR}/${APP_NAME}/files/SWCL_MANIFEST.json PERMISSIONS OWNER_READ GROUP_READ WORLD_READ DESTINATION
	${UCM_SWCL_DIR}/sample/swclSample)
