
#
# Reads an os-release file (https://www.linux.org/docs/man5/os-release.html)
# This will create a variable for each valid input line.
# (<PREFIX>_<KEY> = <VALUE>)
#
# @param[in] PATH       Path of the os-release file. Default: /etc/os-release
# @param[in] PREFIX     Prefix for the generated variables Default: OS
# @param[in] REQUIRED   If set, the file must exist
#
function(read_os_release)
  set(options REQUIRED)
  set(oneValueArgs PATH PREFIX)
  set(multiValueArgs )
  cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  if("${ARG_PATH}" STREQUAL "")
    set(ARG_PATH "/etc/os-release")
  endif()

  if("${ARG_PREFIX}" STREQUAL "")
    set(ARG_PREFIX "OS")
  endif()

  file(REAL_PATH "${ARG_PATH}" ARG_PATH)

  if(NOT EXISTS "${ARG_PATH}")
    set(${PREFIX}_FOUND FALSE)
    if(ARG_REQUIRED)
      set(loglvl FATAL_ERROR)
    else()
      set(loglvl STATUS)
    endif()
    message(${loglvl} "os-release file \"${ARG_PATH}\" not found")
    return()
  endif()

  set(${PREFIX}_FOUND TRUE)

  file(STRINGS ${ARG_PATH} content LENGTH_MINIMUM 3)

  foreach(line IN ITEMS ${content})
    string(REGEX REPLACE "(.*)=(.*)" "\\1" key ${line})
    string(REGEX REPLACE "(.*)=(.*)" "\\2" val ${line})
    string(REGEX REPLACE "\"(.*)\"" "\\1" qval ${val})
    if(NOT ${qval} STREQUAL "")
      set(val ${qval})
    endif()

    set(${ARG_PREFIX}_${key} ${val})
    set(${ARG_PREFIX}_${key} ${val} PARENT_SCOPE)
    message(DEBUG "${ARG_PREFIX}_${key} = ${val}")
  endforeach()

  if("${${ARG_PREFIX}_NAME}" STREQUAL "")
    set(${ARG_PREFIX}_NAME "Linux" PARENT_SCOPE)
  endif()

  if("${${ARG_PREFIX}_ID}" STREQUAL "")
    set(${ARG_PREFIX}_ID "linux" PARENT_SCOPE)
  endif()

  if(DEFINED ${ARG_PREFIX}_ID_LIKE)
    string(REPLACE " " ";" lst "${${ARG_PREFIX}_ID_LIKE}")
    set(${ARG_PREFIX}_ID_LIKE ${lst} PARENT_SCOPE)
  endif()
endfunction()

#
# Gets an OS label (mostly used for packaging)
#
# @param[out] label     The OS label
# @param[in]  PREFIX    Prefix of the variables. Default: OS
# @param[in]  COMPACT   Creates a more compact label string
#
function(get_os_label label)
  set(options COMPACT)
  set(oneValueArgs PATH PREFIX)
  set(multiValueArgs )
  cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  if(NOT DEFINED ARG_PREFIX)
    set(ARG_PREFIX "OS")
  endif()

  if(WIN32)
    set(os_label "win")

    if(DEFINED CMAKE_VS_PLATFORM_TOOLSET)
      set(${os_label} "${os_label}-${CMAKE_VS_PLATFORM_TOOLSET}")
    endif()

    set(${label} ${os_label} PARENT_SCOPE)
    return()
  endif()

  # check for exact match
  if("${${ARG_PREFIX}_ID}" STREQUAL "rhel")
    set(os_label "el")
  elseif("${${ARG_PREFIX}_ID}" STREQUAL "debian")
    set(os_label "deb")
  elseif("${${ARG_PREFIX}_ID}" STREQUAL "fedora")
    set(os_label "f")
  elseif("${${ARG_PREFIX}_ID}" STREQUAL "ubuntu")
    set(os_label "ubu")
  endif()

  # check for related names - note: the order is important
  if(NOT DEFINED os_label AND DEFINED ${ARG_PREFIX}_ID_LIKE)
    if("rhel" IN_LIST ${ARG_PREFIX}_ID_LIKE)
        set(os_label "el")
    elseif("debian" IN_LIST ${ARG_PREFIX}_ID_LIKE)
        set(os_label "deb")
    elseif("archlinux" IN_LIST ${ARG_PREFIX}_ID_LIKE OR "arch" IN_LIST ${ARG_PREFIX}_ID_LIKE)
        set(os_label "arch")
    endif()
  endif()

  # defaults
  if("${os_label}" STREQUAL "")
    set(os_label ${${ARG_PREFIX}_ID})
  endif()
  if("${os_label}" STREQUAL "")
    set(os_label "linux")
  endif()

  # version
  if(NOT "${${ARG_PREFIX}_VERSION_ID}" STREQUAL "" AND NOT "${${ARG_PREFIX}_VERSION_ID}" STREQUAL "rolling")
    string(REPLACE "." "_" ver ${${ARG_PREFIX}_VERSION_ID})
    if(ARG_COMPACT)
      string(REGEX REPLACE "(.*)_.*" "\\1" ver_maj ${ver})
      if(NOT "${ver_maj}" STREQUAL "")
        set(ver ${ver_maj})
      endif()
    endif()
    set(os_label "${os_label}${ver}")
  endif()

  set(${label} ${os_label} PARENT_SCOPE)
endfunction()


#
# Get the OS package format
#
# @param[out] pkgfmt  Package format (CPack Generator)
#
function(get_os_package_format pkgfmt)
  if(DEFINED OS_ID)
    _get_os_package_format(tmp "${OS_ID}")
    if(NOT "${tmp}" STREQUAL "")
      set(${pkgfmt} ${tmp} PARENT_SCOPE)
      return()
    endif()
  endif()

  if(DEFINED OS_ID_LIKE)
    foreach(os_id IN ITEMS ${OS_ID_LIKE})
      _get_os_package_format(tmp "${os_id}")
      if(NOT "${tmp}" STREQUAL "")
        set(${pkgfmt} ${tmp} PARENT_SCOPE)
        return()
      endif()
    endforeach()
  endif()

  set(${pkgfmt} "TGZ" PARENT_SCOPE)
endfunction()


#
# Helper method for "get_os_package"
#
# @param[out] pkgfmt  Package format
# @param[in]  os_id   OS identifier
#
function(_get_os_package_format pkgfmt os_id)
  set(os_package_types DEB;RPM;TZST;ZIP)
  set(os_id_DEB debian;ubuntu)
  set(os_id_RPM fedora;rhel;sles;suse)
  set(os_id_TZST arch;archlinux)
  set(os_id_ZIP win;windows;win32;win64)

  foreach(pt IN ITEMS ${os_package_types})
    if(NOT DEFINED os_id_${pt})
      continue()
    endif()
    if(${os_id} IN_LIST os_id_${pt})
      set(${pkgfmt} ${pt} PARENT_SCOPE)
      return()
    endif()
  endforeach()
endfunction()


#
# Prints some OS infos
#
# @param[in]  PREFIX    Prefix of the variables. Default: OS
#
function(print_os_info)
  set(options COMPACT FULL)
  set(oneValueArgs PREFIX)
  set(multiValueArgs )
  cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  if(NOT DEFINED ARG_PREFIX)
    set(ARG_PREFIX "OS")
  endif()

  if(ARG_COMPACT)
    set(os_name "")
    _append(os_name ${ARG_PREFIX}_NAME SEPARATOR " ")
    _append(os_name ${ARG_PREFIX}_VERSION_ID SEPARATOR " ")
    get_os_label(os_label COMPACT)
    message(STATUS "OS: ${os_name} (${os_label})")
  elseif(ARG_FULL)
    if(DEFINED ${ARG_PREFIX}_PRETTY_NAME)
      message(STATUS "OS Pretty Name: ${${ARG_PREFIX}_PRETTY_NAME}")
    endif()
    if(DEFINED ${ARG_PREFIX}_NAME)
      message(STATUS "OS Name: ${${ARG_PREFIX}_NAME}")
    endif()
    if(DEFINED ${ARG_PREFIX}_ID)
      message(STATUS "OS ID: ${${ARG_PREFIX}_ID}")
    endif()
    if(DEFINED ${ARG_PREFIX}_ID_LIKE)
      message(STATUS "OS Like: ${${ARG_PREFIX}_ID_LIKE}")
    endif()
    if(DEFINED ${ARG_PREFIX}_VERSION)
      message(STATUS "OS Version: ${${ARG_PREFIX}_VERSION}")
    endif()
    if(DEFINED ${ARG_PREFIX}_VERSION_ID)
      message(STATUS "OS Version ID: ${${ARG_PREFIX}_VERSION_ID}")
    endif()
    if(DEFINED ${ARG_PREFIX}_PLATFORM_ID)
      message(STATUS "OS Platform ID: ${${ARG_PREFIX}_PLATFORM_ID}")
    endif()
  else()
    set(os_name "")
    _append(os_name ${ARG_PREFIX}_PRETTY_NAME)
    if(NOT "${${ARG_PREFIX}_NAME}" STREQUAL "${${ARG_PREFIX}_PRETTY_NAME}")
      _append(os_name ${ARG_PREFIX}_NAME)
    endif()

    set(os_id "")
    _append(os_id ${ARG_PREFIX}_ID)
    _append(os_id ${ARG_PREFIX}_ID_LIKE)

    set(os_version "")
    _append(os_version ${ARG_PREFIX}_VERSION)
    if(NOT "${${ARG_PREFIX}_VERSION}" STREQUAL "${${ARG_PREFIX}_VERSION_ID}")
      _append(os_version ${ARG_PREFIX}_VERSION_ID)
    endif()

    get_os_label(os_label COMPACT)

    message(STATUS "OS Name: ${os_name}")
    message(STATUS "OS ID: ${os_id}")
    message(STATUS "OS Version: ${os_version}")
    message(STATUS "OS Label: ${os_label}")
  endif()
endfunction()


#
# Helper method to append strings
#
# @param[in,out]  str       The target string
# @param[in]      varname   The name of the variable to append
#
function(_append str varname)
  set(options BRACKETS)
  set(oneValueArgs SEPARATOR)
  set(multiValueArgs )
  cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  if(NOT DEFINED ${varname})
    return()
  endif()

  if("${${str}}" STREQUAL "")
    set(${str} "${${varname}}" PARENT_SCOPE)
    return()
  endif()

  if(ARG_BRACKETS)
    set(${str} "${${str}} (${${varname}})" PARENT_SCOPE)
    return()
  endif()

  if(NOT DEFINED ARG_SEPARATOR)
    set(ARG_SEPARATOR " / ")
  endif()

  set(${str} "${${str}}${ARG_SEPARATOR}${${varname}}" PARENT_SCOPE)
endfunction()


#
# Gathers OS infos
#
macro(gather_os_info)
  if(WIN32)
    set(OS_NAME "Windows")
    set(OS_ID "windows")
  else()
    read_os_release(PATH "${OS_RELEASE_PATH}")
  endif()

  get_os_package_format(OS_PACKAGE_FORMAT)
endmacro()


#
#  auto magic
#

if(NOT DEFINED OS_AUTO_GATHER OR OS_AUTO_GATHER)
  gather_os_info()
endif()

