cmake_minimum_required(VERSION 3.20)

#
# generate_resources([TARGET target] [RESOURCES ...] [PATH path])
#
# @param[in] TARGET     The target to create (optional)
# @param[in] RESOURCES  List of input files
# @param[in] CEXT       Generate .c files
#
function(generate_resources)
  set(options CEXT)
  set(oneValueArgs TARGET)
  set(multiValueArgs RESOURCES)
  cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  if(DEFINED ARG_RESOURCES AND NOT "${ARG_RESOURCES}" STREQUAL "")
    set(outfile ${CMAKE_CURRENT_BINARY_DIR}/resources.h)
    generate_resources_header(RESOURCES ${ARG_RESOURCES} PATH ${outfile})
    list(APPEND sources ${outfile})

    foreach(res IN LISTS ARG_RESOURCES)
      _embedded_resources_get_output_file(outfile ${res} ${ARG_CEXT})
      generate_resource_file(${res} OUTFILE ${outfile})
      list(APPEND sources ${outfile})
    endforeach()

    if(DEFINED ARG_TARGET AND NOT "{ARG_TARGET}" STREQUAL "")
      add_library(${ARG_TARGET} STATIC ${sources})
    endif()
  endif()
endfunction()

#
# generate_resource_file(infile [OUTFILE <output-path>] [LABEL label])
#
# Converts the 'INFILE' to an unsigned char array named 'LABEL' in 'OUTFILE'.
#
# @param[in] INFILE   The input file
# @param[in] OUTFILE  The output file. Default: ${CMAKE_CURRENT_BINARY_DIR}/${INFILE}.c
# @param[in] LABEL    The variable name. Default: ${INFILE}
# @param[in] CEXT     If TRUE, the extension of the auto-generated outputfile will be ".c"
#
function(generate_resource_file INFILE)
  set(options CEXT)
  set(oneValueArgs OUTFILE LABEL)
  cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "" ${ARGN})

  if(DEFINED ARG_OUTFILE AND NOT "${ARG_OUTFILE}" STREQUAL "")
    set(OUTFILE ${ARG_OUTFILE})
  else()
    _embedded_resources_get_output_file(OUTFILE "${INFILE}" ${ARG_CEXT})
  endif()

  if(DEFINED ARG_LABEL AND NOT "${ARG_LABEL}" STREQUAL "")
    set(LABEL ${ARG_LABEL})
  else()
    _embedded_resources_extract_from_filename("${INFILE}" lname lext)
    set(LABEL "_binary_${lext}_${lname}_${lext}")
  endif()

  file(READ ${INFILE} hexdata HEX)
  string(REGEX REPLACE "(..)" "0x\\1, " formatted ${hexdata})
  file(WRITE ${OUTFILE} "unsigned char ${LABEL}_data[] = { ${formatted} };\nunsigned long ${LABEL}_size = sizeof(${LABEL}_data);")
endfunction()


# generate_resources_header([RESOURCES ...] [PATH output-path])
#
# @param[in] RESOURCES  List of input files
# @param[in] PATH       Output path. Default: ${CMAKE_CURRENT_BINARY_DIR}/resources.h
#
function(generate_resources_header)
  set(options "")
  set(oneValueArgs PATH)
  set(multiValueArgs RESOURCES)
  cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  if(DEFINED ARG_PATH AND NOT "${ARG_PATH}" STREQUAL "")
    set(path ${ARG_PATH})
  else()
    set(path ${CMAKE_CURRENT_BINARY_DIR}/resources.h)
  endif()

  file(
    CONFIGURE
    OUTPUT ${path}
    CONTENT
"#pragma once

#define __BINARY_RESOURCE_CONCAT(prefix, project, postfix) prefix ## project ## postfix
#define _BINARY_RESOURCE_CONCAT(prefix, project, postfix) __BINARY_RESOURCE_CONCAT(prefix, project, postfix)

#define _BINARY_RESOURCE_DATA(project, type) _BINARY_RESOURCE_CONCAT(_binary_ ## type ## _, project, _ ## type ## _data)
#define _BINARY_RESOURCE_SIZE(project, type) _BINARY_RESOURCE_CONCAT(_binary_ ## type ## _, project, _ ## type ## _size)

#define BINARY_RESOURCE_DATA(name, project, type) \\
  extern unsigned char _BINARY_RESOURCE_DATA(project, type)[]; \\
  static const unsigned char* name = &_BINARY_RESOURCE_DATA(project, type)[0];

#define BINARY_RESOURCE_SIZE(name, project, type) \\
  extern unsigned long _BINARY_RESOURCE_SIZE(project, type); \\
  static const unsigned long name = _BINARY_RESOURCE_SIZE(project, type);

#define BINARY_RESOURCE(name, project, type) \\
  BINARY_RESOURCE_DATA(name ## _data, project, type) \\
  BINARY_RESOURCE_SIZE(name ## _size, project, type)

"
    @ONLY
    NEWLINE_STYLE LF
  )

  if(DEFINED ARG_RESOURCES AND NOT "${ARG_RESOURCES}" STREQUAL "")
    foreach(res IN LISTS ARG_RESOURCES)
      _embedded_resources_extract_from_filename("${res}" fname fext)
      file(APPEND ${path} "BINARY_RESOURCE(${fname}_${fext}, ${fname}, ${fext});\n")
    endforeach()
  endif()
endfunction()

function(_embedded_resources_sanitize output input)
  set(tmp "${input}")
  string(REPLACE "." "_" tmp "${tmp}")
  string(REPLACE "-" "_" tmp "${tmp}")
  set(${output} "${tmp}" PARENT_SCOPE)
endfunction()

function(_embedded_resources_extract_from_filename input filename extension)
  cmake_path(GET input STEM LAST_ONLY fname)
  cmake_Path(GET input EXTENSION fext)
  _embedded_resources_sanitize(fname "${fname}")
  _embedded_resources_sanitize(fext "${fext}")
  string(REGEX REPLACE "^\\_(.*)" "\\1" fext "${fext}")
  set(${filename} "${fname}" PARENT_SCOPE)
  set(${extension} "${fext}" PARENT_SCOPE)
endfunction()

function(_embedded_resources_get_output_file outfile infile cext)
  cmake_path(GET infile FILENAME fname)
  if("${cext}" STREQUAL "TRUE")
    set(fname ${fname}.c)
  else()
    set(fname ${fname}.cpp)
  endif()
  set(${outfile} ${CMAKE_CURRENT_BINARY_DIR}/${fname} PARENT_SCOPE)
endfunction()
