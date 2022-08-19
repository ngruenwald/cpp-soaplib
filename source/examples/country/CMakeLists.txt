
#
# generate code
#

configure_file(config.xml.in ${CMAKE_CURRENT_BINARY_DIR}/config.xml @ONLY)
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS CountryInfoService.wsdl)

get_property(soapgen_dir TARGET soapgen PROPERTY RUNTIME_OUTPUT_DIRECTORY)
find_program(soapgen_binary soapgen HINTS ${soapgen_dir})

if(NOT soapgen_binary)
  message(FATAL_ERROR "soapgen binary not found")
endif()

execute_process(
  COMMAND ${soapgen_binary} ${CMAKE_CURRENT_BINARY_DIR}/config.xml
  OUTPUT_FILE ${CMAKE_CURRENT_BINARY_DIR}/soapgen.log
  ERROR_FILE ${CMAKE_CURRENT_BINARY_DIR}/soapgen.error.log
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

add_subdirectory(
  ${CMAKE_CURRENT_BINARY_DIR}/country-info
  ${CMAKE_CURRENT_BINARY_DIR}/country-info-bin
)

#
# main application
#

set(target country)

add_executable(
  ${target}
  main.cpp
)

set_project_warnings(${target})

target_link_libraries(
  ${target}
  PUBLIC
  country::CountryInfoService
)

target_link_libraries(
  ${target}
  PUBLIC
  soaplib::soaplib
)

target_link_libraries(${target} PUBLIC ${OPENSSL_LIBRARIES})
target_include_directories(${target} PUBLIC ${OPENSSL_INCLUDE_DIR})

if(WIN32)
  target_link_libraries(
    ${target}
    PUBLIC
    ws2_32
    iphlpapi
    rpcrt4
  )
else()
  find_library(LIBUUID uuid PATHS /usr/local/lib64 /usr/local/lib)

  target_link_libraries(
    ${target}
    PUBLIC
    ${LIBUUID}
  )
endif()

set_target_properties(
  ${target}
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
