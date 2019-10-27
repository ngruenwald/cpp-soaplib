cmake_minimum_required(VERSION 3.0)

find_package(xmlwrp 3.0 REQUIRED)

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
  commsys::xmlwrp
)

set_target_properties(
  soapgen
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
