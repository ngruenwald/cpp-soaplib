
#
# generate code
#

configure_file(config.xml.in ${CMAKE_CURRENT_BINARY_DIR}/config.xml @ONLY)
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS calculator.wsdl)

get_property(soapgen_dir TARGET soapgen PROPERTY RUNTIME_OUTPUT_DIRECTORY)
find_program(soapgen_binary soapgen HINTS ${soapgen_dir})

if(NOT soapgen_binary)
  mesaage(FATAL_ERROR "sopagen binary not found")
endif()

execute_process(
  COMMAND ${soapgen_binary} ${CMAKE_CURRENT_BINARY_DIR}/config.xml > ${CMAKE_CURRENT_BINARY_DIR}/soapgen.log
  OUTPUT_FILE ${CMAKE_CURRENT_BINARY_DIR}/soapgen.log
  ERROR_FILE ${CMAKE_CURRENT_BINARY_DIR}/soapgen.error.log
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

add_subdirectory(
  ${CMAKE_CURRENT_BINARY_DIR}/calc
  ${CMAKE_CURRENT_BINARY_DIR}/calc-bin
)

#
# main application
#

set(target calculator)

add_executable(
  ${target}
  main.cpp
)

set_project_warnings(${target})

target_link_libraries(
  ${target}
  PUBLIC
  calc::calculator-service
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
