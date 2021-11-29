
#
# Setup install paths
#

if(WIN32)
  set(
    ORIGINAL_INSTALL_PREFIX
    binaries
  )

  mark_as_advanced(
    ORIGINAL_INSTALL_PREFIX
  )

  set(
    CMAKE_INSTALL_PREFIX
    "${ORIGINAL_INSTALL_PREFIX}"
    CACHE STRING "Install path"
    FORCE
  )

  set(
    LIBRARY_INSTALL_PATH
    ${PROJECT_NAME}/V_${PROJECT_VERSION}/lib
  )

  set(
    HEADER_INSTALL_ROOT
    ${PROJECT_NAME}/V_${PROJECT_VERSION}/include
  )

  set(
    HEADER_INSTALL_PATH
    ${HEADER_INSTALL_ROOT}/${PROJECT_NAME_PREFIX}${PROJECT_NAME}
  )

  set(
    CONFIG_INSTALL_PATH
    cmake/${PROJECT_NAME}-${PROJECT_VERSION}
  )

  message(STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")
else()
  if(${CMAKE_SIZEOF_VOID_P} EQUAL 8)
    set(LIBSUFFIX 64)
  else()
    set(LIBSUFFIX "")
  endif()

  set(
    LIBRARY_INSTALL_PATH
    lib${LIBSUFFIX}/${PROJECT_NAME}-${PROJECT_VERSION}
  )

  set(
    HEADER_INSTALL_ROOT
    include/${PROJECT_NAME}-${PROJECT_VERSION}
  )

  set(
    HEADER_INSTALL_PATH
    ${HEADER_INSTALL_ROOT}/${PROJECT_NAME_PREFIX}${PROJECT_NAME}
  )

  set(
    CONFIG_INSTALL_PATH
    ${LIBRARY_INSTALL_PATH}/cmake
  )
endif()

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
#  types/timestamp.cpp
#  types/timestamp.hpp
#  types/duration.cpp
#  types/duration.hpp
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

#set_project_warnings(${target})

# target_link_libraries(
#   ${target}
#   PUBLIC
#   soaplib::xml
# )

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


if(WIN32)
  set_target_properties(
    ${target}
    PROPERTIES
      RELEASE_POSTFIX "_static_${COMPILER_POSTFIX}"
      RELWITHDEBINFO_POSTFIX "_static_${COMPILER_POSTFIX}"
      DEBUG_POSTFIX "_static_${COMPILER_POSTFIX}d"
  )
else()
  set_target_properties(
    ${target}
    PROPERTIES
      RELEASE_POSTFIX "-${PROJECT_VERSION}"
      RELWITHDEBINFO_POSTFIX "-${PROJECT_VERSION}"
      DEBUG_POSTFIX "-${PROJECT_VERSION}d"
  )
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
    types/timestamp.hpp
    types/duration.hpp
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
    soaplibTargets.cmake
NAMESPACE
    soaplib::
  DESTINATION
    ${CONFIG_INSTALL_PATH}
)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
  "soaplibConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
)

install(
  FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/soaplibConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/soaplibConfigVersion.cmake"
  DESTINATION
    ${CONFIG_INSTALL_PATH}
)

