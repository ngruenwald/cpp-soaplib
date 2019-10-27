
add_executable(
  unit_tests
  unit_tests.cpp
)

add_test(
  NAME unit_tests
  COMMAND unit_tests
)

set_target_properties(
  unit_tests
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
