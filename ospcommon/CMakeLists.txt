## ======================================================================== ##
## Copyright 2009-2019 Intel Corporation                                    ##
##                                                                          ##
## Licensed under the Apache License, Version 2.0 (the "License");          ##
## you may not use this file except in compliance with the License.         ##
## You may obtain a copy of the License at                                  ##
##                                                                          ##
##     http://www.apache.org/licenses/LICENSE-2.0                           ##
##                                                                          ##
## Unless required by applicable law or agreed to in writing, software      ##
## distributed under the License is distributed on an "AS IS" BASIS,        ##
## WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. ##
## See the License for the specific language governing permissions and      ##
## limitations under the License.                                           ##
## ======================================================================== ##

add_library(${PROJECT_NAME}
  common.cpp

  memory/malloc.cpp

  networking/BufferedDataStreaming.cpp
  networking/Socket.cpp
  networking/SocketFabric.cpp

  os/FileName.cpp
  os/library.cpp

  tasking/detail/enkiTS/TaskScheduler.cpp
  tasking/detail/tasking_system_init.cpp
  tasking/detail/TaskSys.cpp

  utility/demangle.cpp
  utility/ParameterizedObject.cpp
  utility/PseudoURL.cpp
  utility/TimeStamp.cpp

  xml/XML.cpp
)

target_link_libraries(${PROJECT_NAME}
  PUBLIC
    ospcommon_tasking
    ${CMAKE_DL_LIBS}
    $<${OSPCOMMON_PLATFORM_WIN}:ws2_32>
)

target_include_directories(${PROJECT_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/..>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

if (WIN32)
  set(EXTRA_COMPILE_DEFINITIONS -DNOMINMAX)
endif()

target_compile_definitions(${PROJECT_NAME}
  PRIVATE
    # NOTE(jda) - OSPRAY_SOVERSION fallbacks in library.cpp should be handled
    #             in the Library object's interface directly, where libospcommon
    #             would inject versioning information independent of ospcommon
    #             version.
    -DOSPCOMMON_SOVERSION=0
  PUBLIC
    ${EXTRA_COMPILE_DEFINITIONS}
)

set_property(TARGET ospcommon PROPERTY POSITION_INDEPENDENT_CODE ON)

## Install library + targets ##################################################

install(TARGETS ${PROJECT_NAME}
  EXPORT ospcommon_Exports
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    NAMELINK_SKIP
  # on Windows put the dlls into bin
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  # ... and the import lib into the devel package
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(EXPORT ospcommon_Exports
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ospcommon
  NAMESPACE ospcommon::
)

install(TARGETS ${PROJECT_NAME}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    NAMELINK_ONLY
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

## Install headers ############################################################

install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  FILES_MATCHING
  PATTERN *.h
  PATTERN *.inl
)
