
#
# generate code
#

configure_file(config.xml.in ${CMAKE_CURRENT_BINARY_DIR}/config.xml @ONLY)

#set(soapgen_binary "$<TARGET_FILE:soapgen>")
set(soapgen_binary ${CMAKE_BINARY_DIR}/soapgen)

execute_process(
  COMMAND ${soapgen_binary} ${CMAKE_CURRENT_BINARY_DIR}/config.xml
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
