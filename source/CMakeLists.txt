add_subdirectory(soaplib)
add_subdirectory(soapgen)

if(UNIT_TESTS)
  add_subdirectory(unit_tests)
endif()

if(EXAMPLES)
  add_subdirectory(examples)
endif()
