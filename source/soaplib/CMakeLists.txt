
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
  SoapBase.cpp
  SoapBase.hpp
  SoapServer.hpp
  SoapTransport.hpp
  HttpSoapTransport.cpp
  HttpSoapTransport.hpp
  HttpSoapServer.cpp
  HttpSoapServer.hpp
  WebSocketSoapTransport.cpp
  WebSocketSoapTransport.hpp
  WebSocketSoapServer.cpp
  WebSocketSoapServer.hpp
  SoapFault.cpp
  SoapFault.hpp
  SoapException.cpp
  SoapException.hpp
  SoapService.cpp
  SoapService.hpp
  SoapClient.cpp
  SoapClient.hpp
  types/DateTypes.cpp
  types/DateTypes.hpp
  types/Uuid.cpp
  types/Uuid.hpp
  types/Base64.cpp
  types/Base64.hpp
  types/SimpleTypes.cpp
  types/SimpleTypes.hpp
  types/SoapBaseType.hpp
  BasicTypes.cpp
  BasicTypes.hpp
  ParseHelper.cpp
  ParseHelper.hpp
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

#target_compiler_warnings(${target})

target_link_libraries(
  ${target}
  PUBLIC
  soaplib::xml
  httplib::httplib
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
    SoapException.hpp
    SoapService.hpp
    SoapBase.hpp
    SoapServer.hpp
    SoapTransport.hpp
    HttpSoapTransport.hpp
    HttpSoapServer.hpp
    WebSocketSoapTransport.hpp
    WebSocketSoapServer.hpp
    SoapFault.hpp
    BasicTypes.hpp
    ParseHelper.hpp
    SoapClient.hpp
    SoapMessage.hpp
  DESTINATION
    ${HEADER_INSTALL_PATH}
)

install(
  FILES
    types/DateTypes.hpp
    types/SimpleTypes.hpp
    types/SoapBaseType.hpp
    types/Uuid.hpp
    types/Base64.hpp
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
