
set(target soaplib-xml)

add_library(
  ${target}
  STATIC
  document.cpp
  document.hpp
  exception.hpp
  node.cpp
  node.hpp
  xml.cpp
  xml.hpp
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
    document.hpp
    exception.hpp
    node.hpp
    xml.hpp
  DESTINATION
    ${HEADER_INSTALL_PATH}/xml
)
