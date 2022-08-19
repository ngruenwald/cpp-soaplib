
add_subdirectory(cppgen)

set(target soapgen)

add_executable(
  ${target}
  main.cpp
  wsdl.cpp
  wsdl.hpp
)

target_link_libraries(
  ${target}
  PUBLIC
  cppgen
)

target_link_libraries(
  ${target}
  PUBLIC
  soaplib::xml
  LibXml2::LibXml2
)

target_link_libraries(
  ${target}
  PUBLIC
  stdc++fs
)

set_target_properties(
  ${target}
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

#
# Install files
#

install(
  TARGETS ${target}
  RUNTIME DESTINATION ${BINARY_INSTALL_PATH}
)
