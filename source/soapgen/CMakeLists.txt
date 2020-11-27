cmake_minimum_required(VERSION 3.0)

add_subdirectory(cppgen)

add_executable(
  soapgen
  main.cpp
  wsdl.cpp
  wsdl.hpp
)

target_link_libraries(
  soapgen
  PUBLIC
  cppgen
)

target_link_libraries(
  soapgen
  PUBLIC
  xmlwrp::xmlwrp
  LibXml2::LibXml2
)

target_link_libraries(
  soapgen
  PUBLIC
  stdc++fs
)

set_target_properties(
  soapgen
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

#
# Install files
#

install(
  TARGETS ${target}
  EXPORT  soaplibTargets
  RUNTIME DESTINATION bin
)
