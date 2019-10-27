# @file   test_function.cmake
# @date   15/08/17, 08-Oct-2015
# @author ngruenwald, cgruber

# Defines the format of the build info export [ JSON, HTML, TEMPL ]
if (USE_CMAKEPP)
  set(EXPORT_FORMAT TEMPL)
else()
  set(EXPORT_FORMAT HTML)
endif()

# find_digcom_lib
# @param[in] name    Name of the library
# @param[in] version String containing the required library version "maj.min.pat"
# @param[in] ARGV2   Optional: Type of library 'full' / 'hdronly'
# @param[in] ARGV3   Optional: Path (if library name doesn't match the path name)
# @param[in] ARGV4   Optional: Header file used in search
# @param[in] ARGV5   Optional: Library name(s)
# @modifies DIGCOM_LIB_BINARIES The 'global' list of libraries.
# @modifies DIGCOM_LIB_INCLUDES The 'global' list of include paths.
# @modifies DIGCOM_LIB_NOTFOUND The 'global' list of missing libraries.
# @modifies DIGCOM_LIB_INFO The 'global' list of digcom libaries.
function(find_digcom_lib name version_string)
  if(DEFINED ARGV2)
    if(NOT "${ARGV2}" STREQUAL "full" AND NOT "${ARGV2}" STREQUAL "hdronly")
      message(FATAL_ERROR "Provided library type (\"${ARGV2}\") is invalid.\nMust be one of: \"full\", \"hdronly\"")
    endif()
    set(type ${ARGV2})
  else()
    set(type "full")
  endif()

  if(DEFINED ARGV3)
    if(NOT "${ARGV3}" STREQUAL "")
      set(path ${ARGV3})
    else()
      set(path ${name})
    endif()
  else()
    set(path ${name})
  endif()

  if(DEFINED ARGV4)
    if(NOT "${ARGV4}" STREQUAL "")
      set(hint_file ${ARGV4})
    endif()
  endif()

  if(DEFINED ARGV5)
    if(NOT "${ARGV5}" STREQUAL "")
      set(lib_names ${ARGV5})
    endif()
  endif()

  set(verbosity 1)

  split_version_string(${version_string} major minor patch custom)
  __find_digcom_lib(${path} ${name} "${major}" "${minor}" "${patch}" "${custom}" "${type}" lib_bin lib_inc "${hint_file}" ${verbosity})

  list(APPEND DIGCOM_LIB_BINARIES ${lib_bin})
  list(APPEND DIGCOM_LIB_INCLUDES ${lib_inc})
  add_digcom_lib_info(${name} ${version_string})

  if("${type}" STREQUAL "hdronly")
    if("${lib_inc}" STREQUAL "")
      list(APPEND DIGCOM_LIB_NOTFOUND ${name})
    else()
      set(${name}_VERSION ${version_string} PARENT_SCOPE)
    endif()
  else()
    if("${lib_bin}" STREQUAL "")
      list(APPEND DIGCOM_LIB_NOTFOUND ${name})
    else()
      set(${name}_VERSION ${version_string} PARENT_SCOPE)
    endif()
  endif()

  set(DIGCOM_LIB_BINARIES "${DIGCOM_LIB_BINARIES}" PARENT_SCOPE)
  set(DIGCOM_LIB_INCLUDES "${DIGCOM_LIB_INCLUDES}" PARENT_SCOPE)
  set(DIGCOM_LIB_NOTFOUND "${DIGCOM_LIB_NOTFOUND}" PARENT_SCOPE)
  set(DIGCOM_LIB_INFO     "${DIGCOM_LIB_INFO}"     PARENT_SCOPE)
endfunction(find_digcom_lib)

# __find_include_path
# @param[out] result
# @param[in] name
# @param[in] version
# @param[in] version_exp
# @param[in] major
# @param[in] minor
# @param[in] patch
# @param[in] custom
# @param[in] hint_file
# @param[in,opt] verbosity <ARGV9>
function(__find_include_path result name version version_exp major minor patch custom hint_file)

  if(DEFINED ARGV9)
    set(verbosity ${ARGV9})
  else()
    set(verbosity 0)
  endif()

  vmessage(STATUS "find_include_path ${name} ${version}" ${verbosity})

  set(name_lib ${name}lib)
  set(lib_name lib${name})

  set(
    name_list
    ${name}
    ${lib_name}
    ${name_lib}
  )

  set(
    file_list
    ${name}.h
    ${name}.hh
    ${name}.hpp
    ${lib_name}.h
    ${lib_name}.hh
    ${lib_name}.hpp
    ${name_lib}.h
    ${name_lib}.hh
    ${name_lib}.hpp
    ${hint_file}
  )

  if(DEFINED ARGV7)
    set(file_list ${ARGV7} ${file_list})
  endif()

  set(var "${name}-includes")
  unset(${var} CACHE)

  if(DEFINED SEARCH_ROOTS)
    set(search_paths ${SEARCH_ROOTS})
  else()
    set(
      search_paths
      "/opt/eurofunk/libraries"
      "/usr/local"
    )
  endif()

  #
  # Search for include files in versioned folders
  #

  foreach(path ${search_paths})
    vmessage(STATUS "searching includes in '${path}'" ${verbosity})

    find_path(
      ${var}
      NAMES
        ${file_list}
      PATHS
        "${path}/include/${name}/${version}"
        "${path}/include/${name}/${version_exp}"
        "${path}/include/${name}/${major}.${minor}.${patch}"
        "${path}/include/${name}/${major}.${minor}.${patch}.${custom}"
        "${path}/include/${lib_name}/${version}"
        "${path}/include/${lib_name}/${version_exp}"
        "${path}/include/${lib_name}/${major}.${minor}.${patch}"
        "${path}/include/${lib_name}/${major}.${minor}.${patch}.${custom}"
        "${path}/include/${name_lib}/${version}"
        "${path}/include/${name_lib}/${version_exp}"
        "${path}/include/${name_lib}/${major}.${minor}.${patch}"
        "${path}/include/${name_lib}/${major}.${minor}.${patch}.${custom}"
        # certain third-party libs (sofia *cough*, *cough*)
        "${path}/include/${name}-${version}"
        "${path}/include/${name}-${major}.${minor}.${patch}"
        "${path}/include/${name}-${major}.${minor}"
      NO_DEFAULT_PATH
    )

    if(${var})
      vmessage(STATUS "found includes for ${name} ${version} in ${${var}} [1]" ${verbosity})
      set(${result} ${${var}} PARENT_SCOPE)
      return()
    endif()

    #
    # Search for include files in versioned folder with sub-paths
    #

    find_path(
      ${var}
      NAMES
        ${file_list}
      PATHS
        "${path}/include/${name}/${version}"
        "${path}/include/${name}/${version_exp}"
        "${path}/include/${name}/${major}.${minor}.${patch}"
        "${path}/include/${name}/${major}.${minor}.${patch}.${custom}"
        "${path}/include/${lib_name}/${version}"
        "${path}/include/${lib_name}/${version_exp}"
        "${path}/include/${lib_name}/${major}.${minor}.${patch}"
        "${path}/include/${lib_name}/${major}.${minor}.${patch}.${custom}"
        "${path}/include/${name_lib}/${version}"
        "${path}/include/${name_lib}/${version_exp}"
        "${path}/include/${name_lib}/${major}.${minor}.${patch}"
        "${path}/include/${name_lib}/${major}.${minor}.${patch}.${custom}"
        # certain third-party libs (sofia *cough*, *cough*)
        "${path}/include/${name}-${version}"
        "${path}/include/${name}-${major}.${minor}.${patch}"
        "${path}/include/${name}-${major}.${minor}"
      PATH_SUFFIXES
        ${name_list}
      NO_DEFAULT_PATH
    )

    if(${var})
      get_filename_component(path ${${var}} DIRECTORY)
      vmessage(STATUS "found includes for ${name} ${version} in ${path} (${${var}}) [2]" ${verbosity})
      set(${result} ${path} PARENT_SCOPE)
      return()
    endif()

    #
    # Search for a versioned folder
    #

    find_path(
      ${var}
      NAMES
        "${version}"
        "${version_exp}"
        "${major}.${minor}.${patch}"
        "${major}.${minor}.${patch}.${custom}"
      PATHS
        "${path}/include/${name}"
        "${path}/include/${lib_name}"
        "${path}/include/${name_lib}"
      NO_DEFAULT_PATH
    )

    if(${var})
      set(base_path ${${var}})
      if(EXISTS "${base_path}/${version}")
        set(full_path "${base_path}/${version}")
      elseif(EXISTS "${base_path}/${version_exp}")
        set(full_path "${base_path}/${version_exp}")
      elseif(EXISTS "${base_path}/${major}.${minor}.${patch}")
        set(full_path "${base_path}/${major}.${minor}.${patch}")
      elseif(EXISTS "${base_path}/${major}.${minor}.${patch}.${custom}")
        set(full_path "${base_path}/${major}.${minor}.${patch}.${custom}")
      else()
        set(full_path ${base_path})
      endif()
      vmessage(STATUS "found includes for ${name} ${version} in ${full_path} (${${var}})" ${verbosity})
      set(${result} ${full_path} PARENT_SCOPE)
      return()
    endif()

  endforeach()

  #
  # Find specific header in standard paths
  #

  find_path(
    ${var}
    NAMES
      ${file_list}
    PATHS
      "/usr/local/include"
      "/usr/include"
    PATH_SUFFIXES
      ${name_list}
  )

  if(${var})
    vmessage(STATUS "found includes for ${name} ${version} in ${path} (${${var}}) [3]" ${verbosity})
    set(${result} ${${var}} PARENT_SCOPE)
    return()
  endif()

  #
  # No more tries
  #

  vmessage(STATUS "did not find includes for ${name} ${version}" ${verbosity})

  set(${result} ${${var}} PARENT_SCOPE)

endfunction(__find_include_path)


# __find_digcom_lib
# @param[in]     path               Path of the library
# @param[in]     name               Name of the library
# @param[in]     major              Major part of the version number
# @param[in]     minor              Minor part of the version number
# @param[in]     patch              Patch part of the version number
# @param[in]     custom             Custom part of the version number
# @param[in]     type               Library type: "full" / "hdronly"
# @param[out]    lib_bin            Returns the library paths
# @param[out]    lib_inc            Returns the include paths
# @param[in,opt] hint_file <ARGV9>  Name of a header file use during the search
# @param[in,opt] lib_names <ARGV10> Library (binary) names
# @param[in,opt] verbosity <ARGV11> Controls the verbosity of the function
function(__find_digcom_lib path name major minor patch custom type lib_bin lib_inc)
  # parameters
  if(NOT DEFINED name)
    message(SEND_ERROR "library name not specified.")
  endif()

  if(NOT DEFINED major)
    message(SEND_ERROR "major version not specified")
  endif()

  if(NOT DEFINED minor)
    message(SEND_ERROR "minor version not specified")
  endif()

  if(NOT DEFINED patch)
    message(SEND_ERROR "patch version not specified")
  endif()

  if(DEFINED ARGV9)
    set(hint_file ${ARGV9})
  endif()

  if(DEFINED ARGV10)
    set(lib_names ${ARGV10})
  endif()

  if(DEFINED ARGV11)
    set(verbosity ${ARGV11})
  else()
    set(verbosity 0)
  endif()

  get_version_string("${major}" "${minor}" "${patch}" "${custom}" version)
  get_expanded_version_string("${major}" "${minor}" "${patch}" "${custom}" version_exp)
  vmessage(STATUS "find_digcom_lib: ${name} ${major}.${minor}.${patch} [.${custom}] (${version})" ${verbosity})

  # libraries path
  if(WIN32)
    if(NOT DEFINED DIGCOM_LIB)
      string(REGEX REPLACE "\\\\" "/" DIGCOM_LIB $ENV{DIGCOM_LIB}) # only forward slashes
      if(NOT DEFINED DIGCOM_LIB)
        message(FATAL_ERROR "DIGCOM_LIB path not defined.")
      else()
        vmessage(STATUS " DIGCOM Libraries: ${DIGCOM_LIB}" ${verbosity})
      endif()
    endif()
  endif()

  # postfixes
  if(NOT DEFINED DEBUG_POSTFIX)
    set(DEBUG_POSTFIX d)
    vmessage(STATUS "    DEBUG_POSTFIX: ${DEBUG_POSTFIX}" ${verbosity})
  endif()

  if(WIN32)
    if(NOT DEFINED COMPILER_POSTFIX)
      set(COMPILER_POSTFIX "msvc${MSVC_VERSION}")
      if(MSVC80)
        set(COMPILER_POSTFIX "vc80")
      endif()
      if(MSVC90)
        set(COMPILER_POSTFIX "vc90")
      endif()
      if(MSVC10)
        set(COMPILER_POSTFIX "vc100")
      endif()
      if(MSVC11)
        set(COMPILER_POSTFIX "vc110")
      endif()
      if(MSVC12)
        set(COMPILER_POSTFIX "vc120")
      endif()
      vmessage(STATUS " COMPILER_POSTFIX: ${COMPILER_POSTFIX}" ${verbosity})
    endif()

    set(LIB_NAME ${name}_static_${COMPILER_POSTFIX}.lib)
    set(LIB_NAME_DBG ${name}_static_${COMPILER_POSTFIX}${DEBUG_POSTFIX}.lib)
  else()
    set(LIB_NAME ${name}-${version}.a)
    set(LIB_NAME_EXP ${name}-${version_exp}.a)
    set(LIB_NAME_DBG ${name}-${version}${DEBUG_POSTFIX}.a)
    set(LIB_NAME_DBG_EXP ${name}-${version}${DEBUG_POSTIFX}.a)
    if("${custom}" STREQUAL "")
      set(LIB_NAME_2 ${name}-${major}.${minor}.${patch}.a)
      set(LIB_NAME_DBG_2 ${name}-${major}.${minor}.${patch}${DEBUG_POSTFIX}.a)
    else()
      set(LIB_NAME_2 ${name}-${major}.${minor}.${patch}.${custom}.a)
      set(LIB_NAME_DBG_2 ${name}-${major}.${minor}.${patch}.${custom}${DEBUG_POSTIFX}.a)
    endif()
  endif()

  # find it
  if(WIN32)
    set(dir1 "${DIGCOM_LIB}/${path}/V_${version}")
    set(dir2 "${DIGCOM_LIB}/lib${path}/V_${version}")
    if(EXISTS "${dir1}" AND IS_DIRECTORY "${dir1}")
      set(LIB_ROOT ${dir1})
    elseif(EXISTS "${dir2}" AND IS_DIRECTORY "${dir2}")
      set(LIB_ROOT ${dir2})
    endif()
  else()
    if(DEFINED SEARCH_ROOTS)
      set(search_paths ${SEARCH_ROOTS})
    else()
      set(
        search_paths
        "/opt/eurofunk/libraries"
        "/usr/local"
      )
    endif()
  endif()

  vmessage(STATUS "search_paths: ${search_paths}" ${verbosity})

  set(LIB NOTFOUND)
  set(LIB_DBG NOTFOUND)

  if("${type}" STREQUAL "hdronly")
    set(LIB " ")
    set(LIB_DBG " ")
  else() # "full"
    if(WIN32)
      find_library(
        LIB
        NAMES
        ${LIB_NAME}
        lib${LIB_NAME}
        ${lib_names}
        PATHS
        ${LIB_ROOT}/lib
      )
      find_library(
        LIB_DBG
        NAMES
        ${LIB_NAME_DBG}
        lib${LIB_NAME_DBG}
        PATHS
        ${LIB_ROOT}/lib
      )
    else()
      # Make sure that cgruber wont see this!
      if(${name} STREQUAL "zlib")
        set(DIST_LIB_NAME "libz.a")
      else()
        set(DIST_LIB_NAME "lib${name}.a")
      endif()
      foreach(path ${search_paths})
        vmessage(STATUS "searching binaries in '${path}'" ${verbosity})
        find_library(
          LIB
          NAMES
            ${lib_names}
            ${LIB_NAME}
            lib${LIB_NAME}
            ${LIB_NAME_EXP}
            lib${LIB_NAME_EXP}
            ${LIB_NAME_2}
            lib${LIB_NAME_2}
            # fallback to dist package
            ${DIST_LIB_NAME}
          PATHS
            ${path}/lib64
            ${path}/lib
            # e.g. glog
            ${path}/lib64/${name}/${version}
            ${path}/lib/${name}/${version}
          NO_DEFAULT_PATH # system path(s) should be included in the search_paths
        )
        if(NOT ${LIB} STREQUAL "LIB-NOTFOUND")
          vmessage(STATUS "found binaries for ${name} ${version} at ${LIB} (${path})" ${verbosity})
          break()
        endif()
      endforeach()

      foreach(path ${search_paths})
        find_library(
          LIB_DBG
          NAMES
            ${LIB_NAME_DBG}
            lib${LIB_NAME_DBG}
            ${LIB_NAME_DBG_EXP}
            lib${LIB_NAME_DBG_EXP}
            ${LIB_NAME_DBG_2}
            lib${LIB_NAME_DBG_2}
            # fallback to dist package
            ${DIST_LIB_NAME}
          PATHS
            ${path}/lib64
            ${path}/lib
            # e.g. glog
            ${path}/lib64/${name}/${version}
            ${path}/lib/${name}/${version}
        )
        if(NOT ${LIB_DBG} STREQUAL "LIB_DBG-NOTFOUND")
          break()
        endif()
      endforeach()
    endif()

    if(${LIB} STREQUAL "LIB-NOTFOUND")
      if (NOT ${LIB_DBG} STREQUAL "LIB_DBG-NOTFOUND")
        set(LIB ${LIB_DBG})
      endif()
    else()
      if (${LIB_DBG} STREQUAL "LIB_DBG-NOTFOUND")
        set(LIB_DBG ${LIB})
      endif()
    endif()
  endif()

  if(${LIB} STREQUAL "LIB-NOTFOUND")
    message(STATUS "not found: ${name} ${version} (binaries not found)")
  else()
    if("${type}" STREQUAL "hdronly")
    else()
      if(WIN32)
        set(${lib_bin} "optimized;${LIB};debug;${LIB_DBG}" PARENT_SCOPE)
      else()
        if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
          set(${lib_bin} "${LIB_DBG}" PARENT_SCOPE)
        else()
          set(${lib_bin} "${LIB}" PARENT_SCOPE)
        endif()
      endif()
    endif()

    if(WIN32)
      set(INC "${LIB_ROOT}/include")
    else()
      __find_include_path(INC "${name}" "${version}" "${version_exp}" "${major}" "${minor}" "${patch}" "${custom}" "${hint_file}" ${verbosity})
    endif()

    if(NOT INC)
      message(STATUS "not found: ${name} ${version} (includes not found)")
      set(${lib_inc} "" PARENT_SCOPE)
      set(${lib_bin} "" PARENT_SCOPE)
    else()
      set(${lib_inc} "${INC}" PARENT_SCOPE)

      vmessage(STATUS " ${name} binaries: ${LIB} ${LIB_DBG}" ${verbosity})
      vmessage(STATUS " ${name} includes: ${INC}" ${verbosity})

      if(${verbosity} EQUAL 0)
        message(STATUS "using: ${name} ${version}")
      endif()
    endif()
  endif()
endfunction(__find_digcom_lib)

# check_missing_libs
function(check_missing_libs)
  if(NOT "${DIGCOM_LIB_NOTFOUND}" STREQUAL "")
    message(FATAL_ERROR "Missing libraries: ${DIGCOM_LIB_NOTFOUND}\n")
  endif()
endfunction()

# find_third_lib
# @param[in] name    Name of the library
# @param[in] version String containing the required library version "maj.min.pat"
# @param[in] ARGV2   Optional: Type of library 'full' / 'hdronly'
# @param[in] ARGV3   Optional: lib Path (if library name doesn't match the path name)
# @param[in] ARGV4   Optional: library file name
# @param[in] ARGV5   Optional: inc Path (if library name doesn't match the path name)
# @param[in] ARGV6   Optional: Header File Name
# @modifies THIRD_LIB_BINARIES The 'global' list of libraries.
# @modifies THIRD_LIB_INCLUDES The 'global' list of include paths.
# @modifies THIRD_LIB_NOTFOUND The 'global' list of missing libraries.
# @modifies THIRD_LIB_INFO The 'global' list of third party libaries.
function(find_third_lib name version)
  if(WIN32)
    message(SEND_ERROR "digsyslibs.cmake: function find_third_lib not yet tested on windows.")
  endif()

  if(DEFINED ARGV2)
    if (NOT "${ARGV2}" STREQUAL "full" AND NOT "${ARGV2}" STREQUAL "hdronly")
      message(FATAL_ERROR "Provided library type (\"${ARGV2}\") is invalid.\nMust be one of: \"full\", \"hdronly\"")
    endif()
    set(type ${ARGV2})
  else()
    set(type "full")
  endif()

  if(DEFINED ARGV3)
    set(LIB_PATH ${ARGV3})
  endif()

  if(DEFINED ARGV4)
	set(libname ${ARGV4})
  endif()

  if(DEFINED ARGV5)
    set(INC_PATH ${ARGV5})
  endif()

  if(DEFINED ARGV6)
    set(header_file_name ${ARGV6})
  endif()

  if(DEFINED ARGV3 AND DEFINED ARGV4 AND DEFINED ARGV5 AND DEFINED ARGV6)
# message("BLABLA ${LIB_PATH}")
	find_library(
	  ${name}_bin
	  NAMES
	  ${libname}
	  PATH
	  ${LIB_PATH}
	  )
	find_path(
	  ${name}_inc
	  NAMES
	  ${header_file_name}
	  PATHS
	  ${INC_PATH}
	  )
  else()
    find_library(
      ${name}_bin
      NAMES
      ${libname}
      ${name}-${version}.a
      lib${name}-${version}.a
      ${name}.a
      lib${name}.a
      ${name}lib-${version}.a
      ${name}lib.a
      PATHS
      ${LIB_PATH}
      /usr/lib64
      /usr/local/lib64
      /usr/lib
      /usr/lib/x86_64-linux-gnu/
      /usr/local/lib
      NO_DEFAULT_PATH
      )

	find_path(
      ${name}_inc
      NAMES
      ${header_file_name}
      ${name}/${name}-${version}.h
      ${name}-${version}.h
      ${name}/${name}.h
      ${name}.h
      ${name}
      PATHS
      ${INC_PATH}
      /usr/include/${name}
      /usr/local/include/${name}/${version}
      /usr/include /usr/local/include
      NO_DEFAULT_PATH
      )
  endif()

  list(APPEND THIRD_LIB_INCLUDES ${${name}_inc})
  add_third_lib_info(${name} ${version})

  if ("${type}" STREQUAL "hdronly")
    if("${${name}_inc}" STREQUAL "${name}_inc-NOTFOUND")
      list(APPEND THIRD_LIB_NOTFOUND ${name})
    endif()
  else()
    if("${${name}_bin}" STREQUAL "${name}_bin-NOTFOUND")
      list(APPEND THIRD_LIB_NOTFOUND ${name})
    else()
      list(APPEND THIRD_LIB_BINARIES "-Wl,-Bstatic ${${name}_bin}")
    endif()
  endif()

  message(STATUS "using: ${${name}_bin} including ${${name}_inc}")

  set(THIRD_LIB_BINARIES "${THIRD_LIB_BINARIES}" PARENT_SCOPE)
  set(THIRD_LIB_INCLUDES "${THIRD_LIB_INCLUDES}" PARENT_SCOPE)
  set(THIRD_LIB_NOTFOUND "${THIRD_LIB_NOTFOUND}" PARENT_SCOPE)
  set(THIRD_LIB_INFO     "${THIRD_LIB_INFO}"     PARENT_SCOPE)
endfunction(find_third_lib)

# add_digcom_lib_info
# @param[in] name The name of the library
# @param[in] version The version of the library
# @modifies DIGCOM_LIB_INFO The 'global' list of digcom libaries.
function(add_digcom_lib_info name version)
  format_lib_info(${name} ${version} lib_info)
  list(APPEND DIGCOM_LIB_INFO ${lib_info})
  set(DIGCOM_LIB_INFO "${DIGCOM_LIB_INFO}" PARENT_SCOPE)
endfunction(add_digcom_lib_info)

# add_third_lib_info
# @param[in] name The name of the library
# @param[in] version The version of the library
# @modifies THIRD_LIB_INFO The 'global' list of third party libaries.
function(add_third_lib_info name version)
  format_lib_info(${name} ${version} lib_info)
  list(APPEND THIRD_LIB_INFO ${lib_info})
  set(THIRD_LIB_INFO "${THIRD_LIB_INFO}" PARENT_SCOPE)
endfunction(add_third_lib_info)

# format_lib_info
# @param[in] name The name of the library.
# @param[in] version The version of the library.
# @param[out] lib_info The formatted library info.
function(format_lib_info name version lib_info)
  if(${EXPORT_FORMAT} STREQUAL HTML)
    # format at HTML list entry
    set(${lib_info} "<li>${name} ${version}</li>" PARENT_SCOPE)
  endif()
  if(${EXPORT_FORMAT} STREQUAL JSON)
    # format as JSON array/list entry
    set(${lib_info} "{\"name\": \"${name}\", \"version\": \"${version_string}\"}" PARENT_SCOPE)
  endif()
  if(${EXPORT_FORMAT} STREQUAL TEMPL)
    # use a format suitable for cmakepp template generator
    set(${lib_info} "{ name:'${name}', version:'${version}' }" PARENT_SCOPE)
  endif()
endfunction(format_lib_info)

# export_project_info
# @param[in] file_name The name of the output file, without extension.
function(export_project_info file_name)
  if(${EXPORT_FORMAT} STREQUAL HTML)
    # export as HTML
    export_project_info_html("${file_name}.html")
  endif()
  if(${EXPORT_FORMAT} STREQUAL JSON)
    # export as JSON
    export_project_info_json("${file_name}.json")
  endif()
  if(${EXPORT_FORMAT} STREQUAL TEMPL)
    # use file_name as input for the template generator
    export_project_info_templ("${file_name}")
  endif()
endfunction(export_project_info)

# export_project_info_html
# @param[in] file_name The name of the output file.
function(export_project_info_html file_name)
  string(REGEX REPLACE ";" "\n" digcom "${DIGCOM_LIB_INFO}")
  string(REGEX REPLACE ";" "\n" third "${THIRD_LIB_INFO}")

  set(html "")
  # project info
  set(html ${html} "<h1>${PROJECT_NAME} ${PROJECT_VERSION}</h1>\n")
  #build info
  set(html ${html} "<div>\n")
  set(html ${html} "<h2>Build Parameters</h2>\n")
  set(html ${html} "<ul>\n")
  set(html ${html} "<li>SVN revision: ${SVN_REVISION}</li>\n")
  set(html ${html} "<li>Release tag: ${RELEASE_TAG}</li>\n")
  set(html ${html} "<li>Build node: ${BUILD_NODE}</li>\n")
  set(html ${html} "</ul>\n")
  set(html ${html} "</div>\n")
  # dependencies
  if (NOT "${digcom}" STREQUAL "" OR NOT "${third}"  STREQUAL "")
    set(html ${html} "<div>\n")
    set(html ${html} "<h2>Dependencies</h2>\n")
  endif()
  if (NOT "${digcom}" STREQUAL "")
    set(html ${html} "<div>\n")
    set(html ${html} "<h3>Digcom Libraries</h3>\n")
    set(html ${html} "<ul>\n")
    set(html ${html} "${digcom}\n")
    set(html ${html} "</ul>\n")
    set(html ${html} "</div>\n")
  endif()
  if (NOT "${third}" STREQUAL "")
    set(html ${html} "<div>\n")
    set(html ${html} "<h3>Third Party Libraries</h3>\n")
    set(html ${html} "<ul>\n")
    set(html ${html} "${third}\n")
    set(html ${html} "</ul>\n")
    set(html ${html} "</div>\n")
  endif()
  if (NOT "${digcom}" STREQUAL "" OR NOT "${third}"  STREQUAL "")
    set(html ${html} "</div>\n")
  endif()

  file(WRITE ${file_name} ${html})
endfunction(export_project_info_html)

# export_project_info_json
# @param[in] file_name The name of the output file.
function(export_project_info_json file_name)
  string(REGEX REPLACE ";" ",\n" digcom_libs "${DIGCOM_LIB_INFO}")
  string(REGEX REPLACE ";" ",\n" third_libs "${THIRD_LIB_INFO}")

  # opening
  set(json "{\n")

  # project info
  set(json ${json} "\"project\": \"${PROJECT_NAME}\",\n")
  set(json ${json} "\"version\": \"${PROJECT_VERSION}\",\n")
  # build info
  set(json ${json} "\"svn-revision\": \"${SVN_REVISION}\",\n")
  set(json ${json} "\"release-tag\": \"${RELEASE_TAG}\",\n")
  set(json ${json} "\"build-node\": \"${BUILD_NODE}\",\n")
  # dependencies
  set(json ${json} "\"libs\": {\n")
  set(json ${json} "\"digcom\": [\n${digcom_libs}\n],\n")
  set(json ${json} "\"third\": [\n${third_libs}\n]\n")
  set(json ${json} "}\n") # libs

  # closing
  set(json ${json} "}")

  file(WRITE ${file_name} ${json})
endfunction(export_project_info_json)

if(${EXPORT_FORMAT} STREQUAL TEMPL)
  include(cmakepp)

  function(export_project_info_templ template_file)
    string(REGEX REPLACE ";" ", " digcom_libs "${DIGCOM_LIB_INFO}")
    string(REGEX REPLACE ";" ", " third_libs "${THIRD_LIB_INFO}")

    data("{
      project:'${PROJECT_NAME}',
      version:'${PROJECT_VERSION}',
      svn-revision:'${SVN_REVISION}',
      release-tag:'${RELEASE_TAG}',
      build-node:'${BUILD_NODE}'
    }")
    ans(info)

    data("[ ${digcom_libs} ]")
    ans(digcom)

    data("[ ${third_libs} ]")
    ans(third)

    template_run_file("${template_file}")
    ans(generated_content)

    string(REGEX REPLACE "(.*/)(.*)\\.in$" "\\2" output_file ${template_file})
    message(STATUS "Writing build info to ${output_file}")

    file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/${output_file}" "${generated_content}")
  endfunction(export_project_info_templ)
endif()

# vmessage
# @param[in] level      The message level (see cmake message doc).
# @param[in] text       The text to print.
# @param[in] verbosity  The verbosity; if greater '0', the message will be print.
function(vmessage level text verbosity)
  if(${verbosity} GREATER 0)
    message(${level} ${text})
  endif()
endfunction(vmessage)

# get_version_string
# @param[in]  major  The major part of the version number.
# @param[in]  minor  The minor part of the version number.
# @param[in]  patch  The patch part of the version number.
# @param[in]  custom The custom part of the version number.
# @param[out] result The version number as a string.
function(get_version_string major minor patch custom result)
  expand_version_number_os("${major}" major)
  expand_version_number_os("${minor}" minor)
  expand_version_number_os("${patch}" patch)
  if ("${custom}" STREQUAL "")
    set(${result} "${major}.${minor}.${patch}" PARENT_SCOPE)
  else()
    expand_version_number_os("${custom}" custom)
    set(${result} "${major}.${minor}.${patch}.${custom}" PARENT_SCOPE)
  endif()
endfunction(get_version_string)

# get_expanded_version_string
# @param[in]  major  The major part of the version number.
# @param[in]  minor  The minor part of the version number.
# @param[in]  patch  The patch part of the version number.
# @param[in]  custom The custom part of the version number.
# @param[out] result The version number as a string.
function(get_expanded_version_string major minor patch custom result)
  expand_version_number("${major}" major)
  expand_version_number("${minor}" minor)
  expand_version_number("${patch}" patch)
  if ("${custom}" STREQUAL "")
    set(${result} "${major}.${minor}.${patch}" PARENT_SCOPE)
  else()
    expand_version_number("${custom}" custom)
    set(${result} "${major}.${minor}.${patch}.${custom}" PARENT_SCOPE)
  endif()
endfunction(get_expanded_version_string)

# expand_version_number_os
# @param[in]  number The number to expand.
# @param[out] result The resulting string.
function(expand_version_number_os number result)
  if("${number}" STREQUAL "")
    set(${result} "" PARENT_SCOPE)
  else()
    if(WIN32)
      string(REGEX REPLACE "^(.)$" "0\\1" expanded_number "${number}")
      set(${result} "${expanded_number}" PARENT_SCOPE)
    else()
      set(${result} "${number}" PARENT_SCOPE)
    endif()
  endif()
endfunction(expand_version_number_os)

# expand_version_number
# @param[in]  number The number to expand.
# @param[out] result The resulting string.
function(expand_version_number number result)
  if("${number}" STREQUAL "")
    set(${result} "" PARENT_SCOPE)
  else()
    string(REGEX REPLACE "^(.)$" "0\\1" expanded_number "${number}")
    set(${result} "${expanded_number}" PARENT_SCOPE)
  endif()
endfunction(expand_version_number)

# split_version_string
# @param[in]  version_string    The version string (major.minor.patch[.custom]).
# @param[out] version_major     The major number.
# @param[out] version_minor     The minor number.
# @param[out] version_patch     The patch number.
# @param[out] version_custom    The custom number.
function(split_version_string version_string version_major version_minor version_patch version_custom)
  string(REPLACE "." ";" version_list ${version_string})
  separate_arguments(version_list)
  list(LENGTH version_list len)
  if(${len} LESS 3)
    message(SEND_ERROR "${name}: invalid version format (${version_string}); must be  {major}.{minor}.{patch} [.{custom}]")
  else()
    list(GET version_list 0 major)
    list(GET version_list 1 minor)
    list(GET version_list 2 patch)
    set(${version_major} "${major}" PARENT_SCOPE)
    set(${version_minor} "${minor}" PARENT_SCOPE)
    set(${version_patch} "${patch}" PARENT_SCOPE)
    if(${len} LESS 4)
      set(${version_custom} "" PARENT_SCOPE)
    else()
      list(GET version_list 3 custom)
      set(${version_custom} "${custom}" PARENT_SCOPE)
    endif()
  endif()
endfunction(split_version_string)

# set_project_version
# @param[in] ARGV1                  The version string (major.minor.patch).
# @param[in] ARGV2                  If set to "PREFER_TAG", the SVN_TAG_VERSION variable is used first
# @modifies  PROJECT_VERSION        The version string as provided.
# @modifies  PROJECT_VERSION_STRING The formatted version string.
# @modifies  PROJECT_VERSION_MAJOR  The major number.
# @modifies  PROJECT_VERSION_MINOR  The minor number.
# @modifies  PROJECT_VERSION_PATCH  The patch number.
# @modifies  PROJECT_VERSION_CUSTOM The custom number. (When we mess with 3rd party libs.)
# @modifies  PROJECT_VERSION_INT    The project version as integer number.
function(set_project_version)
  set(PROJECT_VERSION_TYPE "cmake" PARENT_SCOPE)

  if(DEFINED ARGV1 AND NOT "${ARGV1}" STREQUAL "")
    set(version_string ${ARGV1})
  elseif(DEFINED PROJECT_VERSION AND NOT "${PROJECT_VERSION}" STREQUAL "")
    set(version_string ${PROJECT_VERSION})
  else()
    message(ERROR "Either set PROJECT_VERSION or pass the version in ARGV1.")
  endif()

  if(DEFINED ARGV2 AND "${ARGV2}" STREQUAL "PREFER_TAG")
    if(DEFINED SVN_TAG_VERSION)
      #message(STATUS "using tag version as project version")
      set(version_string ${SVN_TAG_VERSION})
      set(PROJECT_VERSION_TYPE "tag" PARENT_SCOPE)
    else()
      #message(STATUS "no svn tag version available")
    endif()
  endif()

  set(PROJECT_VERSION ${version_string} PARENT_SCOPE)

  split_version_string(${version_string} major minor patch custom)
  set(PROJECT_VERSION_MAJOR ${major} PARENT_SCOPE)
  set(PROJECT_VERSION_MINOR ${minor} PARENT_SCOPE)
  set(PROJECT_VERSION_PATCH ${patch} PARENT_SCOPE)
  set(PROJECT_VERSION_CUSTOM ${custom} PARENT_SCOPE)

  get_version_string("${major}" "${minor}" "${patch}" "${custom}" version_formatted)
  set(PROJECT_VERSION_STRING ${version_formatted} PARENT_SCOPE)

  # WARNING: custom modifications are not incorporated into the integer version

  math(EXPR version_integer "${major} * 10000 + ${minor}*100 + ${patch}")
  set(PROJECT_VERSION_INT ${version_integer} PARENT_SCOPE)
endfunction(set_project_version)

# debug
# @param[in] msg The debug message to print.
macro(debug msg)
  message(STATUS "DEBUG ${msg}")
endmacro()

# debugValue
# @param[in] variableName The name of the variable whose value to print.
macro(debugValue variableName)
  debug("${variableName}=\${${variableName}}")
endmacro()

# Initialize global variables
set(DIGCOM_LIB_INCLUDES "")
set(DIGCOM_LIB_BINARIES "")
set(THIRD_LIB_INCLUDES "")
set(THIRD_LIB_BINARIES "")
set(DIGCOM_LIB_INFO "")
set(THIRD_LIB_INFO "")

if(WIN32)
  if(NOT DEFINED DIGCOM_LIB)
    string(
      REGEX
      REPLACE "\\\\" "/"
      DIGCOM_LIB
      $ENV{DIGCOM_LIB}
    )
  endif()
  message(STATUS "DIGCOM_LIB: ${DIGCOM_LIB}")
endif()

