
find_package(Doxygen QUIET)

if(DOXYGEN_FOUND)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
    ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    @ONLY
  )

  add_custom_target(
    doc
    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIRY}
    COMMENT "Generating API documentation with Doxygen"
    VERBATIM
  )
else()
  message(STATUS "Doxygen not found")

  add_custom_target(
    doc
    echo ""
    COMMENT "Skipping API documentation"
  )
endif()
