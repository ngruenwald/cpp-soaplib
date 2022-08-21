find_package(Doxygen COMPONENTS dot)

if(NOT DOXYGEN_FOUND)
  message(STATUS "Doxygen not found")
  add_custom_target(doc echo "" COMMENT "Skipping documentation")
  return()
endif()

set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doxygen)
if(NOT USE_SPHINX)
  set(DOXYGEN_USE_MDFILE_AS_MAINPAGE ${CMAKE_SOURCE_DIR}/README.md)
endif()
set(DOXYGEN_GENERATE_XML YES)
set(DOXYGEN_EXTRACT_PRIVATE NO)
set(DOXYGEN_EXTRACT_STATIC YES)
set(DOXYGEN_STRIP_FROM_PATH ${CMAKE_SOURCE_DIR})
set(DOXYGEN_IMAGE_FORMAT svg)
set(DOXYGEN_DOT_TRANSPARENT YES)

set(
  DOXYGEN_HTML_EXTRA_STYLESHEET
  "${CMAKE_CURRENT_SOURCE_DIR}/doxygen/doxygen-awesome.css \\
  ${CMAKE_CURRENT_SOURCE_DIR}/doxygen/doxygen-awesome-sidebar-only.css \\
  ${CMAKE_CURRENT_SOURCE_DIR}/doxygen/custom.css"
)
set(DOXYGEN_VERBATIM_VARS DOXYGEN_HTML_EXTRA_STYLESHEET)
set(DOXYGEN_GENERATE_TREEVIEW YES)


doxygen_add_docs(
  doxygen
  ${CMAKE_SOURCE_DIR}/README.md
  ${CMAKE_SOURCE_DIR}/CHANGELOG.md
  ${CMAKE_SOURCE_DIR}/LICENSE.txt
  ${CMAKE_SOURCE_DIR}/source/soaplib
  COMMENT "Creating documentation with Doxygen"
)

if(NOT USE_SPHINX)
  add_custom_target(doc DEPENDS doxygen)
  return()
endif()

find_package(Sphinx REQUIRED)

if(NOT SPHINX_FOUND)
  message(STATUS "Sphinx not found")
  add_custom_target(doc DEPENDS doxygen)
  return()
endif()

#set(SPHINX_SOURCE ${CMAKE_CURRENT_SOURCE_DIR})  # FIXME!
set(SPHINX_SOURCE ${CMAKE_CURRENT_BINARY_DIR})
set(SPHINX_BUILD ${CMAKE_CURRENT_BINARY_DIR}/sphinx)
set(SPHINX_INDEX_FILE ${SPHINX_BUILD}/index.html)

configure_file(conf.py.in ${CMAKE_CURRENT_BINARY_DIR}/conf.py)

add_custom_target(
  sphinx
  COMMAND
    ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
  COMMAND
    ${SPHINX_EXECUTABLE}
    -b html
    -c ${CMAKE_CURRENT_BINARY_DIR}
    -D breathe_projects.${PROJECT_NAME}=${DOXYGEN_OUTPUT_DIRECTORY}/xml
    ${SPHINX_SOURCE} ${SPHINX_BUILD}
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  COMMENT "Generating documentation with Sphinx"
  VERBATIM
  DEPENDS doxygen
)

add_custom_target(doc DEPENDS sphinx)
