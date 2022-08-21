
#
# Sub folders
#

add_subdirectory(xml)

#
# Target
#

set(target soaplib)

add_library(
  ${target}
  STATIC
  soapClient.cpp
  soapClient.hpp
  soapException.cpp
  soapException.hpp
  soapService.cpp
  soapService.hpp
  types/dateTypes.cpp
  types/dateTypes.hpp
  types/uuid.cpp
  types/uuid.hpp
  types/base64.cpp
  types/base64.h
  types/simpleTypes.cpp
  types/simpleTypes.hpp
  types/soapBaseType.hpp
  basicTypes.cpp
  basicTypes.hpp
  parseHelper.cpp
  parseHelper.hpp
)

add_library(
  soaplib::${target}
  ALIAS
  ${target}
)

set_target_properties(
  ${target}
  PROPERTIES
  OUTPUT_NAME cpp-soaplib
)

#set_project_warnings(${target})

target_link_libraries(
  ${target}
  PUBLIC
  soaplib::xml
  LibXml2::LibXml2
  Threads::Threads
)

target_include_directories(
  ${target}
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
  $<INSTALL_INTERFACE:${HEADER_INSTALL_ROOT}>
)

# https://gitlab.kitware.com/cmake/cmake/-/issues/15415
target_include_directories(
  ${target}
  PRIVATE
  $<TARGET_PROPERTY:contrib-cpp-httplib,INTERFACE_INCLUDE_DIRECTORIES>
)

target_compile_definitions(
  ${target}
  PRIVATE
  $<TARGET_PROPERTY:contrib-cpp-httplib,INTERFACE_COMPILE_DEFINITIONS>
)

if(INSTALL_VERSIONED)
  add_library_suffix(${target} VERSIONED)
else()
  add_library_suffix(${target})
endif()

#
# Install files
#

install(
  TARGETS ${target} soaplib-xml
  EXPORT  soaplibTargets
  RUNTIME DESTINATION ${LIBRARY_INSTALL_PATH}
  LIBRARY DESTINATION ${LIBRARY_INSTALL_PATH}
  ARCHIVE DESTINATION ${LIBRARY_INSTALL_PATH}
)

install(
  FILES
    soapClient.hpp
    soapException.hpp
    soapService.hpp
    basicTypes.hpp
    parseHelper.hpp
  DESTINATION
    ${HEADER_INSTALL_PATH}
)

install(
  FILES
    types/dateTypes.cpp
    types/dateTypes.hpp
    types/simpleTypes.hpp
    types/soapBaseType.hpp
    types/uuid.hpp
    types/base64.h
  DESTINATION
    ${HEADER_INSTALL_PATH}/types
)

#
# Export project information
#

install(
  EXPORT
    soaplibTargets
  FILE
    cpp-soaplibTargets.cmake
  NAMESPACE
    soaplib::
  DESTINATION
    ${CONFIG_INSTALL_PATH}
)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
  "cpp-soaplibConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
)

install(
  FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/cpp-soaplibConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/cpp-soaplibConfigVersion.cmake"
  DESTINATION
    ${CONFIG_INSTALL_PATH}
)
