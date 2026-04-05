
set(target soaplib-xml)

add_library(
  ${target}
  OBJECT
  Document.cpp
  Document.hpp
  Exception.hpp
  Node.cpp
  Node.hpp
  Xml.cpp
  Xml.hpp
)

add_library(
  soaplib::xml
  ALIAS
  ${target}
)

target_link_libraries(
  ${target}
  PUBLIC
  LibXml2::LibXml2
)

target_include_directories(
  ${target}
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../..>
  $<INSTALL_INTERFACE:${HEADER_INSTALL_ROOT}>
)

install(
  FILES
    Document.hpp
    Exception.hpp
    Node.hpp
    Xml.hpp
  DESTINATION
    ${HEADER_INSTALL_PATH}/xml
)
