# Install script for directory: /home/user/Desktop/GP_VALEO_AP/ara-project-R19-11/sample-applications/ucm_examples/swcl_sample/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}/sample/swclSample/applicationSample/exec1/bin/swcl_sample" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/sample/swclSample/applicationSample/exec1/bin/swcl_sample")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/sample/swclSample/applicationSample/exec1/bin/swcl_sample"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/sample/swclSample/applicationSample/exec1/bin/swcl_sample")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/sample/swclSample/applicationSample/exec1/bin" TYPE EXECUTABLE FILES "/home/user/Desktop/GP_VALEO_AP/ara-project-R19-11/sample-applications/ucm_examples/swcl_sample/src/swcl_sample")
  if(EXISTS "$ENV{DESTDIR}/sample/swclSample/applicationSample/exec1/bin/swcl_sample" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/sample/swclSample/applicationSample/exec1/bin/swcl_sample")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/home/user/Desktop/GP_VALEO_AP/ara-project-R19-11/AUTOSAR/sdks/qemux86/sysroots/x86_64-pokysdk-linux/usr/bin/i586-poky-linux/i586-poky-linux-strip" "$ENV{DESTDIR}/sample/swclSample/applicationSample/exec1/bin/swcl_sample")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/sample/swclSample/applicationSample/exec1/etc/MANIFEST.json")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/sample/swclSample/applicationSample/exec1/etc" TYPE FILE PERMISSIONS OWNER_READ GROUP_READ WORLD_READ FILES "/home/user/Desktop/GP_VALEO_AP/ara-project-R19-11/sample-applications/ucm_examples/swcl_sample/files/MANIFEST.json")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/sample/swclSample/SWCL_MANIFEST.json")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/sample/swclSample" TYPE FILE PERMISSIONS OWNER_READ GROUP_READ WORLD_READ FILES "/home/user/Desktop/GP_VALEO_AP/ara-project-R19-11/sample-applications/ucm_examples/swcl_sample/files/SWCL_MANIFEST.json")
endif()

