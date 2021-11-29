
set(target client)

add_executable(
  ${target}
  client.cpp
  IAuthenticationService.cpp
  IAuthenticationService.h
  IAuthenticationService_GetPublicKey.h
  AuthenticationService.cpp
  AuthenticationService.hpp
  encrypt.cpp
  encrypt.hpp
)

set_project_warnings(${target})

target_link_libraries(
  ${target}
  PUBLIC
  admin::AuthenticationService
)

target_link_libraries(
  ${target}
  PUBLIC
  soaplib::soaplib
  #OpenSSL::Crypto
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

# fix me
target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)

set_target_properties(
  ${target}
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
