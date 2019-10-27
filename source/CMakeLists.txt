
find_package(Threads REQUIRED)
find_package(xmlwrp 3.0 REQUIRED)
find_package(httplib REQUIRED)

if(USE_OPENSSL)
  if(WIN32)
    set(OPENSSL_PATH_VERSION "01.00.02k")
    set(OPENSSL_ROOT_DIR ${CMAKE_SOURCE_DIR}/contrib/openssl/V_${OPENSSL_PATH_VERSION})
  endif()
  find_package(OpenSSL REQUIRED)
endif()

add_subdirectory(soaplib)
add_subdirectory(soapgen)

if(UNIT_TESTS)
  add_subdirectory(unit_tests)
endif()

#add_subdirectory(admin)
#add_subdirectory(client)

