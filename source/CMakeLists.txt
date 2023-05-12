
find_package(Threads REQUIRED)
find_package(LibXml2 REQUIRED)

if(USE_OPENSSL)
  if(WIN32)
    set(OPENSSL_PATH_VERSION "3.1.0")
    set(OPENSSL_ROOT_DIR ${CMAKE_SOURCE_DIR}/contrib/openssl/V_${OPENSSL_PATH_VERSION})
  endif()

  find_package(OpenSSL REQUIRED)
endif()

add_subdirectory(soaplib)
add_subdirectory(soapgen)

if(UNIT_TESTS)
  add_subdirectory(unit_tests)
endif()

if(EXAMPLES)
  add_subdirectory(examples)
endif()
