set(PARSER_TESTS OFF)

if(PARSER_TESTS)
  add_subdirectory(generated)
endif()


find_library(LIBUUID uuid PATHS /usr/local/lib64 /usr/local/lib)

set(target unit_test)

add_executable(
  ${target}
  unit_tests.cpp
  pointer_tests.cpp
)

if(PARSER_TESTS)
  target_sources(
    ${target}
    PRIVATE
    parser_tests.cpp
  )
endif()

target_link_libraries(
  ${target}
  PUBLIC
  contrib-catch2
)

target_link_libraries(
  ${target}
  PUBLIC
  LibXml2::LibXml2
  xmlwrp::xmlwrp
)

if(PARSER_TESTS)
  target_link_libraries(
    ${target}
    PUBLIC
    admin::DispatcherService
  )
endif()


target_link_libraries(
  ${target}
  PUBLIC
  ${LIBUUID}
)

target_compile_definitions(
  ${target}
  PUBLIC
  FIXTURES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/fixtures"
)

set_target_properties(
  ${target}
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
