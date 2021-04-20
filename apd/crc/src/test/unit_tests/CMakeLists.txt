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

function(add_gtest name source)
	find_package(GTest REQUIRED)

	add_executable(${name} ${CMAKE_CURRENT_SOURCE_DIR}/${source})
	target_link_libraries(${name} apd_crc ${GTEST_BOTH_LIBRARIES})
	add_test(${name} ${name})
	install(TARGETS ${name} RUNTIME DESTINATION test/${PROJECT_NAME})
endfunction()


# Buffer class tests
add_gtest(BufferUnitTests src/buffer/buffer_test.cpp)

# CRC calculators tests
add_gtest(CRC_calculateProfile04 src/crc/crc_calculate_profile04_test.cpp)
add_gtest(CRC_calculateProfile05 src/crc/crc_calculate_profile05_test.cpp)
add_gtest(CRC_calculateProfile06 src/crc/crc_calculate_profile06_test.cpp)
add_gtest(CRC_calculateProfile07 src/crc/crc_calculate_profile07_test.cpp)
add_gtest(CRC_calculateProfile11 src/crc/crc_calculate_profile11_test.cpp)
add_gtest(CRC_calculateProfile22 src/crc/crc_calculate_profile22_test.cpp)
add_gtest(CRC_calculateCRC32     src/crc/crc_calculate_crc32_test.cpp)
