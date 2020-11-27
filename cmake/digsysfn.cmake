cmake_minimum_required(VERSION 3.8)

# @file   test_function.cmake
# @date   15/08/17, 08-Oct-2015
# @author ngruenwald, cgruber

if(NOT DEFINED DIGSYSFN_WINSANE)
  # WINSANE... interpret it as you want.
  #  * Disables leading zeros
  set(DIGSYSFN_WINSANE FALSE)
endif()

# Defines the format of the build info export [ JSON, HTML ]
set(EXPORT_FORMAT HTML)

set(MSG_PREFIX "EF_CMAKE_MODULES: ")

#
# commart_find_package(<name> [version] [COMMART] [HDRONLY] [NOEXPORT]
#                      [PATH path]
#                      [TARGET target]
#                      [HEADER_FILES file1 [file2 ...]
#                      [LIBRARY_NAMES name1 [name2 ...])
#
function(commart_find_package name)
  set(options COMMART HDRONLY NOEXPORT)
  set(oneValueArgs PATH TARGET)
  set(multiValueArgs HEADER_FILES LIBRARY_NAMES)
  set(argPrefix ARG)
  cmake_parse_arguments(${argPrefix} "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
  
  set(version ${ARGV1})
 
  if(COMMART IN_LIST ARGV)
    # don't use find_package
    message(TRACE "COMMART option set -- skipping find_package")
  else()
    # filter out commart args
    # WARNING: this might break stuff (e.g. same value in different args)
    
    set(args ${ARGV})
    
    # remove options  
    foreach(item ${options})
      list(REMOVE_ITEM args ${item})
    endforeach()
    
    # remove oneValueArgs
    foreach(item ${oneValueArgs})
      list(REMOVE_ITEM args ${item})
      if(DEFINED ${argPrefix}_${item} AND NOT "${${argPrefix}_${item}}" STREQUAL "")
        list(REMOVE_ITEM args ${${argPrefix}_${item}})
      endif()
    endforeach()
    
    # remove multiValueArgs
    foreach(item ${multiValueArgs})
      list(REMOVE_ITEM args ${item})
      if(DEFINED ${argPrefix}_${item} AND NOT "${${argPrefix}_${item}}" STREQUAL "")
        foreach(itemValue ${${argPrefix}_${item}})
          list(REMOVE_ITEM args "${itemValue}")
        endforeach()
      endif()
    endforeach()
    
    # forward to cmake's find_package
    set(CMAKE_FIND_PACKAGE_SORT_ORDER_NATURAL)
    set(CMAKE_FIND_PACKAGE_SORT_DIRECTION_DESC)
    
    if(DEFINED SEARCH_ROOTS AND NOT "${SEARCH_ROOTS}" STREQUAL "")
      list(APPEND search_paths ${SEARCH_ROOTS})
    endif()
    if(DEFINED CMAKE_PREFIX_PATH AND NOT "${CMAKE_PREFIX_PATH}" STREQUAL "")
      list(APPEND search_paths ${CMAKE_PREFIX_PATH})
    endif()
    if(NOT DEFINED search_paths OR "${search_paths}" STREQUAL "")
      list(APPEND search_paths /opt/eurofunk/libraries)
      list(APPEND search_paths /usr/local)
      list(APPEND search_paths /usr)
    endif()
    
    find_package(${args} QUIET PATHS ${search_paths} NO_DEFAULT_PATH)
    find_package(${args} QUIET)
    message(TRACE "find_package ${name}_FOUND: ${${name}_FOUND}")
          
    if(${${name}_FOUND})
      if(NOT ${ARG_NOEXPORT})
        _EXPORT_DEPENDENCY(${args})
      endif()
      
      # try to extract the package version
      if(DEFINED ${name}_VERSION)
        set(pkg_version ${${name}_VERSION})
      elseif(DEFINED ${name}_VERSION_STRING)
        set(pkg_version ${${name}_VERSION_STRING})
      else()
        string(TOUPPER ${name} uname)
        if(DEFINED ${uname}_VERSION)
          set(pkg_version ${${uname}_VERSION})
        elseif(DEFINED ${uname}_VERSION_STRING)
          set(pkg_version ${${uname}_VERSION_STRING})
        endif()
      endif()

      # build version info string
      if("${pkg_version}" STREQUAL "${version}" OR "${pkg_version}" STREQUAL "")
        set(version_info ${version})
      elseif(NOT "${pkg_version}" STREQUAL "")
        if("${version}" STREQUAL "")
          set(version_info ${pkg_version})
        else()
          set(version_info "${pkg_version} (${version})")
        endif()
      endif()
            
      # print success message
      message(STATUS "${MSG_PREFIX}using: ${name} ${version_info} (pkg)")
      return()
    endif()
  endif()

  # use our own lookup routines
  
  if(NOT DEFINED version)
    message(FATAL_ERROR "${name} -- version is required")
    return()
  endif()
  
  commart_find_library(${ARGV})
  
  if(NOT ${ARG_NOEXPORT})
    _EXPORT_COMMART_DEPENDENCY(${ARGV})
  endif()
  
  set(DIGCOM_LIB_BINARIES "${DIGCOM_LIB_BINARIES}" PARENT_SCOPE)
  set(DIGCOM_LIB_INCLUDES "${DIGCOM_LIB_INCLUDES}" PARENT_SCOPE)
  set(DIGCOM_LIB_NOTFOUND "${DIGCOM_LIB_NOTFOUND}" PARENT_SCOPE)
  set(DIGCOM_LIB_INFO     "${DIGCOM_LIB_INFO}"     PARENT_SCOPE)
  set(DIGCOM_LIB_TARGETS  "${DIGCOM_LIB_TARGETS}"  PARENT_SCOPE)
endfunction()

macro(_EXPORT_DEPENDENCY args)
  set(FIND_PACKAGE_DEPENDENCIES "${FIND_PACKAGE_DEPENDENCIES}\nfind_dependency(${args})" PARENT_SCOPE)
endmacro()

macro(_EXPORT_COMMART_DEPENDENCY args)
  #set(FIND_COMMART_PACKAGE_DEPENDENCIES "${FIND_COMMART_PACKAGE_DEPENDENCIES}\nfind_commart_lib(${args})" PARENT_SCOPE)
  list(GET args 0 name)
  list(GET args 1 version)
  set(
    FIND_COMMART_PACKAGE_DEPENDENCIES 
    "${FIND_COMMART_PACKAGE_DEPENDENCIES}\nmessage(DEBUG \"Dependency ${PROJECT_NAME} ${PROJECT_VERSION} requires ${name} ${version}\")" 
    PARENT_SCOPE
  )
endmacro()

#
# commart_find_library(<name> <version> [HDRONLY]
#                      [PATH path]
#                      [HEADER_FILES file1 [file2 ...]
#                      [LIBRARY_NAMES name1 [name2 ...]
#                      [TARGET target])
#
function(commart_find_library name version)
  set(options HDRONLY)
  set(oneValueArgs PATH TARGET)
  set(multiValueArgs HEADER_FILES LIBRARY_NAMES)
  cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  if(${ARG_HDRONLY})
    set(type "hdronly")
  else()
    set(type "full")
  endif()
  
  find_digcom_lib("${name}" "${version}" "${type}" "${ARG_PATH}" "${ARG_HEADER_FILES}" "${ARG_LIBRARY_NAMES}" "${ARG_TARGET}")

  set(DIGCOM_LIB_BINARIES "${DIGCOM_LIB_BINARIES}" PARENT_SCOPE)
  set(DIGCOM_LIB_INCLUDES "${DIGCOM_LIB_INCLUDES}" PARENT_SCOPE)
  set(DIGCOM_LIB_NOTFOUND "${DIGCOM_LIB_NOTFOUND}" PARENT_SCOPE)
  set(DIGCOM_LIB_INFO     "${DIGCOM_LIB_INFO}"     PARENT_SCOPE)
  set(DIGCOM_LIB_TARGETS  "${DIGCOM_LIB_TARGETS}"  PARENT_SCOPE)
endfunction()

# find_digcom_lib
# @param[in] name    Name of the library
# @param[in] version String containing the required library version "maj.min.pat"
# @param[in] ARGV2   Optional: Type of library 'full' / 'hdronly'
# @param[in] ARGV3   Optional: Path (if library name doesn't match the path name)
# @param[in] ARGV4   Optional: Header file used in search
# @param[in] ARGV5   Optional: Library name(s)
# @param[in] ARGV6   Optional: Target name
# @modifies DIGCOM_LIB_BINARIES The 'global' list of libraries.
# @modifies DIGCOM_LIB_INCLUDES The 'global' list of include paths.
# @modifies DIGCOM_LIB_NOTFOUND The 'global' list of missing libraries.
# @modifies DIGCOM_LIB_INFO The 'global' list of digcom libaries.
# @modifies DIGCOM_LIB_TARGETS The 'global' list of "commart" targets.
function(find_digcom_lib name version_string)
  if(DEFINED ARGV2)
    if(NOT "${ARGV2}" STREQUAL "full" AND NOT "${ARGV2}" STREQUAL "hdronly")
      message(FATAL_ERROR "${MSG_PREFIX}Provided library type (\"${ARGV2}\") is invalid.\nMust be one of: \"full\", \"hdronly\"")
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

  if(DEFINED ARGV6 AND NOT "${ARGV6}" STREQUAL "")
    set(target_name ${ARGV6})
  else()
    set(target_name ${name}::${name})
  endif()

  set(verbosity 0)

  split_version_string(${version_string} major minor patch custom)
  __find_digcom_lib(${path} ${name} "${major}" "${minor}" "${patch}" "${custom}" "${type}" lib_bin lib_inc "${hint_file}" "${lib_names}" ${verbosity})

  list(APPEND DIGCOM_LIB_BINARIES ${lib_bin})
  list(APPEND DIGCOM_LIB_INCLUDES ${lib_inc})
  list(APPEND DIGCOM_LIB_TARGETS ${target_name})
  add_digcom_lib_info(${name} ${version_string})

  if("${type}" STREQUAL "hdronly")
    if("${lib_inc}" STREQUAL "")
      list(APPEND DIGCOM_LIB_NOTFOUND ${name})
    else()
      set(${name}_VERSION ${version_string} PARENT_SCOPE)

      add_library(${target_name} INTERFACE IMPORTED)

      set_target_properties(
        ${target_name}
        PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES ${lib_inc}
      )
    endif()
  else()
    if("${lib_bin}" STREQUAL "")
      list(APPEND DIGCOM_LIB_NOTFOUND ${name})
    else()
      set(${name}_VERSION ${version_string} PARENT_SCOPE)

      add_library(${target_name} STATIC IMPORTED)

      set_target_properties(
        ${target_name}
        PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES ${lib_inc}
        IMPORTED_LOCATION ${lib_bin}
        IMPORTED_IMPLIB ${lib_bin}
        INTERFACE_LINK_LIBRARIES ${lib_bin}
      )
    endif()
  endif()

  set(DIGCOM_LIB_BINARIES "${DIGCOM_LIB_BINARIES}" PARENT_SCOPE)
  set(DIGCOM_LIB_INCLUDES "${DIGCOM_LIB_INCLUDES}" PARENT_SCOPE)
  set(DIGCOM_LIB_NOTFOUND "${DIGCOM_LIB_NOTFOUND}" PARENT_SCOPE)
  set(DIGCOM_LIB_INFO     "${DIGCOM_LIB_INFO}"     PARENT_SCOPE)
  set(DIGCOM_LIB_TARGETS  "${DIGCOM_LIB_TARGETS}"  PARENT_SCOPE)
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

  vmessage(STATUS "${MSG_PREFIX}find_include_path ${name} ${version}" ${verbosity})

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
    vmessage(STATUS "${MSG_PREFIX}searching includes in '${path}'" ${verbosity})

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
      vmessage(STATUS "${MSG_PREFIX}found includes for ${name} ${version} in ${${var}} [1]" ${verbosity})
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
      vmessage(STATUS "${MSG_PREFIX}found includes for ${name} ${version} in ${path} (${${var}}) [2]" ${verbosity})
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
      vmessage(STATUS "${MSG_PREFIX}found includes for ${name} ${version} in ${full_path} (${${var}})" ${verbosity})
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
    vmessage(STATUS "${MSG_PREFIX}found includes for ${name} ${version} in ${path} (${${var}}) [3]" ${verbosity})
    set(${result} ${${var}} PARENT_SCOPE)
    return()
  endif()

  #
  # No more tries
  #

  vmessage(STATUS "${MSG_PREFIX}did not find includes for ${name} ${version}" ${verbosity})

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
    message(SEND_ERROR "${MSG_PREFIX}library name not specified.")
  endif()

  if(NOT DEFINED major)
    message(SEND_ERROR "${MSG_PREFIX}major version not specified")
  endif()

  if(NOT DEFINED minor)
    message(SEND_ERROR "${MSG_PREFIX}minor version not specified")
  endif()

  if(NOT DEFINED patch)
    message(SEND_ERROR "${MSG_PREFIX}patch version not specified")
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
  vmessage(STATUS "${MSG_PREFIX}find_digcom_lib: ${name} ${major}.${minor}.${patch} [.${custom}] (${version})" ${verbosity})

  if(DEFINED lib_names AND NOT "${lib_names}" STREQUAL "")
    set(_lib_names ${lib_names})
    list(FILTER _lib_names EXCLUDE REGEX "^.*\.[a|so|lib]$" )
    if(NOT "${_lib_names}" STREQUAL "")
      list(TRANSFORM _lib_names APPEND "-${version}.a" OUTPUT_VARIABLE _lib_names_ver)
      list(TRANSFORM _lib_names APPEND "-${version_exp}.a" OUTPUT_VARIABLE _lib_names_vex)
      set(lib_names ${lib_names} ${_lib_names_ver} ${_lib_names_vex})
    endif()
  endif()

  # libraries path
  if(WIN32)
    if(NOT DEFINED DIGCOM_LIB)
      string(REGEX REPLACE "\\\\" "/" DIGCOM_LIB $ENV{DIGCOM_LIB}) # only forward slashes
      if(NOT DEFINED DIGCOM_LIB)
        message(FATAL_ERROR "${MSG_PREFIX}DIGCOM_LIB path not defined.")
      else()
        vmessage(STATUS "${MSG_PREFIX} DIGCOM Libraries: ${DIGCOM_LIB}" ${verbosity})
      endif()
    endif()
  endif()

  # postfixes
  if(NOT DEFINED DEBUG_POSTFIX)
    set(DEBUG_POSTFIX d)
    vmessage(STATUS "${MSG_PREFIX}    DEBUG_POSTFIX: ${DEBUG_POSTFIX}" ${verbosity})
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
      vmessage(STATUS "${MSG_PREFIX} COMPILER_POSTFIX: ${COMPILER_POSTFIX}" ${verbosity})
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
    if(DEFINED SEARCH_ROOTS)
      list(GET SEARCH_ROOTS 0 base_path)
    else()
      set(base_path "${DIGCOM_LIB}")
    endif()
    if(DIGSYSFN_WINSANE)
      set(dir1 "${base_path}/${name}-${version}")
      set(dir2 "${base_path}/lib${name}-${version}")
    else()
      set(dir1 "${base_path}/${path}/V_${version}")
      set(dir2 "${base_path}/lib${path}/V_${version}")
    endif()
    set(dir3 "${base_path}/${path}/${version}")
    set(dir4 "${base_path}/lib${path}/${version}")
    if(EXISTS "${dir1}" AND IS_DIRECTORY "${dir1}")
      set(LIB_ROOT ${dir1})
    elseif(EXISTS "${dir2}" AND IS_DIRECTORY "${dir2}")
      set(LIB_ROOT ${dir2})
    elseif(EXISTS "${dir3}" AND IS_DIRECTORY "${dir3}")
      set(LIB_ROOT ${dir3})
    elseif(EXISTS "${dir4}" AND IS_DIRECTORY "${dir4}")
      set(LIB_ROOT ${dir4})
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

  vmessage(STATUS "${MSG_PREFIX}search_paths: ${search_paths}" ${verbosity})

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
        vmessage(STATUS "${MSG_PREFIX}searching binaries in '${path}'" ${verbosity})
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
            # versioned sub folders
            ${path}/lib64/${name}-${version}
            ${path}/lib/${name}-${version}
            # just in case with 'lib' prefix
            ${path}/lib64/lib${name}-${version}
            ${path}/lib/lib${name}-${version}
            # e.g. glog
            ${path}/lib64/${name}/${version}
            ${path}/lib/${name}/${version}
          NO_DEFAULT_PATH # system path(s) should be included in the search_paths
        )
        if(NOT ${LIB} STREQUAL "LIB-NOTFOUND")
          vmessage(STATUS "${MSG_PREFIX}found binaries for ${name} ${version} at ${LIB} (${path})" ${verbosity})
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
            # versioned subfolders for find_package
            ${path}/lib64/${name}-${version}
            ${path}/lib/${name}-${version}
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
    message(STATUS "${MSG_PREFIX}not found: ${name} ${version} (binaries not found)")
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
      message(STATUS "${MSG_PREFIX}not found: ${name} ${version} (includes not found)")
      set(${lib_inc} "" PARENT_SCOPE)
      set(${lib_bin} "" PARENT_SCOPE)
    else()
      set(${lib_inc} "${INC}" PARENT_SCOPE)

      vmessage(STATUS "${MSG_PREFIX} ${name} binaries: ${LIB} ${LIB_DBG}" ${verbosity})
      vmessage(STATUS "${MSG_PREFIX} ${name} includes: ${INC}" ${verbosity})

      if(${verbosity} EQUAL 0)
        message(STATUS "${MSG_PREFIX}using: ${name} ${version}")
      endif()
    endif()
  endif()
endfunction(__find_digcom_lib)

# check_missing_libs
function(check_missing_libs)
  if(NOT "${DIGCOM_LIB_NOTFOUND}" STREQUAL "")
    message(FATAL_ERROR "${MSG_PREFIX}Missing libraries: ${DIGCOM_LIB_NOTFOUND}\n")
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
    message(SEND_ERROR "${MSG_PREFIX}digsyslibs.cmake: function find_third_lib not yet tested on windows.")
  endif()

  if(DEFINED ARGV2)
    if (NOT "${ARGV2}" STREQUAL "full" AND NOT "${ARGV2}" STREQUAL "hdronly")
      message(FATAL_ERROR "${MSG_PREFIX}Provided library type (\"${ARGV2}\") is invalid.\nMust be one of: \"full\", \"hdronly\"")
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

  message(STATUS "${MSG_PREFIX}using: ${${name}_bin} including ${${name}_inc}")

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
  set(html ${html} "<li>SCM revision: ${PROJECT_VERSION_SCM}</li>\n")
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
    if(WIN32 AND NOT DIGSYSFN_WINSANE)
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
    message(SEND_ERROR "${MSG_PREFIX}${name}: invalid version format (${version_string}); must be  {major}.{minor}.{patch} [.{custom}]")
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
# @param[in] version_string         The version string (major.minor.patch).
# @param[in] ARGV1                  If set to "PREFER_TAG", the SCM_TAG_VERSION variable is used first
# @modifies  PROJECT_VERSION        The version string as provided.
# @modifies  PROJECT_VERSION_STRING The formatted version string.
# @modifies  PROJECT_VERSION_MAJOR  The major number.
# @modifies  PROJECT_VERSION_MINOR  The minor number.
# @modifies  PROJECT_VERSION_PATCH  The patch number.
# @modifies  PROJECT_VERSION_CUSTOM The custom number. (When we mess with 3rd party libs.)
# @modifies  PROJECT_VERSION_INT    The project version as integer number.
function(set_project_version version_string)
  set(PROJECT_VERSION_TYPE "cmake" PARENT_SCOPE)

  if(DEFINED ARGV1 AND "${ARGV1}" STREQUAL "PREFER_TAG")
    if(DEFINED SCM_TAG_VERSION)
      #message(STATUS "${MSG_PREFIX}using tag version as project version")
      set(version_string ${SCM_TAG_VERSION})
      set(PROJECT_VERSION_TYPE "tag" PARENT_SCOPE)
    else()
      #message(STATUS "${MSG_PREFIX}no scm tag version available")
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

#
# setup_install_paths([NAME name] [VERSION version] [QUIET])
#
# @param[in] NAME                   The project name.
# @param[in] VERSION                The project version.
# @param[in] QUIET                  If TRUE will not print the paths.
# @modifies  CONFIG_INSTALL_PATH    Location for cmake config installation.
# @modifies  LIBRARY_INSTALL_PATH   Location for library installation.
# @modifies  HEADER_INSTALL_PATH    Location for header installation.
# @modifies  HEADER_INSTALL_ROOT    The root location for header installation.
function(setup_install_paths)
  set(options QUIET)
  set(oneValueArgs NAME VERSION)
  cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "" ${ARGN})

  if(${CMAKE_SIZEOF_VOID_P} EQUAL 8)
    set(LIBSUFFIX 64)
  else()
    set(LIBSUFFIX "")
  endif()

  if(DEFINED ARG_NAME AND NOT "${ARG_NAME}" STREQUAL "")
    set(project_name ${ARG_NAME})
  else()
    set(project_name ${PROJECT_NAME})
  endif()

  if(DEFINED ARG_VERSION AND NOT "${ARG_VERSION}" STREQUAL "")
    set(project_version ${ARG_VERSION})
  elseif(DEFINED PROJECT_VERSION_STRING AND NOT "${PROJECT_VERSION_STRING}" STREQUAL "")
    set(project_version ${PROJECT_VERSION_STRING})
  else()
    set(project_version ${PROJECT_VERSION})
  endif()

  if(WIN32)
    if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
      set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/binaries/${project_version}" CACHE STRING "Install path" FORCE)
      message(STATUS "Set install prefix to ${CMAKE_INSTALL_PREFIX}")
    endif()

    set(LIBRARY_INSTALL_PATH lib)
    set(HEADER_INSTALL_ROOT include)
    set(HEADER_INSTALL_PATH ${HEADER_INSTALL_ROOT}/${PROJECT_NAME_PREFIX}${project_name})
    set(CONFIG_INSTALL_PATH cmake)
  else()
    set(LIBRARY_INSTALL_PATH lib${LIBSUFFIX}/${project_name}-${project_version})
    set(HEADER_INSTALL_ROOT include/${project_name}/${project_version})
    set(HEADER_INSTALL_PATH ${HEADER_INSTALL_ROOT}/${PROJECT_NAME_PREFIX}${project_name})
    set(CONFIG_INSTALL_PATH ${LIBRARY_INSTALL_PATH}/cmake)
  endif()

  set(LIBRARY_INSTALL_PATH ${LIBRARY_INSTALL_PATH} PARENT_SCOPE)
  set(HEADER_INSTALL_ROOT ${HEADER_INSTALL_ROOT} PARENT_SCOPE)
  set(HEADER_INSTALL_PATH ${HEADER_INSTALL_PATH} PARENT_SCOPE)
  set(CONFIG_INSTALL_PATH ${CONFIG_INSTALL_PATH} PARENT_SCOPE)

  if(NOT ARG_QUIET)
    message(STATUS "${MSG_PREFIX}CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")
    message(STATUS "${MSG_PREFIX}LIBRARY_INSTALL_PATH: ${LIBRARY_INSTALL_PATH}")
    message(STATUS "${MSG_PREFIX}CONFIG_INSTALL_PATH:  ${CONFIG_INSTALL_PATH}")
    message(STATUS "${MSG_PREFIX}HEADER_INSTALL_ROOT:  ${HEADER_INSTALL_ROOT}")
    message(STATUS "${MSG_PREFIX}HEADER_INSTALL_PATH:  ${HEADER_INSTALL_PATH}")
  endif()
endfunction()

# debug
# @param[in] msg The debug message to print.
macro(debug msg)
  message(STATUS "${MSG_PREFIX}DEBUG ${msg}")
endmacro()

# debugValue
# @param[in] variableName The name of the variable whose value to print.
macro(debugValue variableName)
  debug("${variableName}=\${${variableName}}")
endmacro()

# Initialize global variables
set(DIGCOM_LIB_INCLUDES "")
set(DIGCOM_LIB_BINARIES "")
set(DIGCOM_LIB_TARGETS "")
set(THIRD_LIB_INCLUDES "")
set(THIRD_LIB_BINARIES "")
set(DIGCOM_LIB_INFO "")
set(THIRD_LIB_INFO "")

set(FIND_PACKAGE_DEPENDENCIES "")
set(FIND_COMMART_PACKAGE_DEPENDENCIES "")

if(WIN32)
  if(NOT DEFINED DIGCOM_LIB)
    string(
      REGEX
      REPLACE "\\\\" "/"
      DIGCOM_LIB
      $ENV{DIGCOM_LIB}
    )
  endif()
  message(STATUS "${MSG_PREFIX}DIGCOM_LIB: ${DIGCOM_LIB}")
endif()


#
# Prefix path
#

option(EFP "Enable eurofunk prefix path" OFF) # name kept short because i'm a lazy typer ;)
set(EF_PREFIX_PATH "/opt/eurofunk/libraries")

if(EFP)
  list(APPEND CMAKE_PREFIX_PATH "${EF_PREFIX_PATH}")
endif()
