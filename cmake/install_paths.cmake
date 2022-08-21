#
# setup_install_paths([NAME name] [VERSION version] [QUIET] [VERSIONED])
#
# @param[in] NAME                   The project name.
# @param[in] VERSION                The project version.
# @param[in] QUIET                  If TRUE will not print the paths.
# @param[in] VERSIONED              If TRUE will install in versioned folders.
#
# @modifies  CONFIG_INSTALL_PATH    Location for cmake config installation.
# @modifies  BINARY_INSTALL_PATH    Location for binary installation.
# @modifies  LIBRARY_INSTALL_PATH   Location for library installation.
# @modifies  HEADER_INSTALL_PATH    Location for header installation.
# @modifies  HEADER_INSTALL_ROOT    The root location for header installation.
#
function(setup_install_paths)
  set(options QUIET VERSIONED)
  set(oneValueArgs NAME HEADER_NAME VERSION)
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

  if(DEFINED ARG_HEADER_NAME AND NOT "${ARG_HEADER_NAME}" STREQUAL "")
    set(header_name ${ARG_HEADER_NAME})
  else()
    set(header_name ${project_name})
  endif()

  if(ARG_VERSIONED)
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
      set(BINARY_INSTALL_PATH bin)
      set(LIBRARY_INSTALL_PATH lib)
      set(HEADER_INSTALL_ROOT include)
      set(HEADER_INSTALL_PATH ${HEADER_INSTALL_ROOT}/${PROJECT_NAME_PREFIX}${project_name})
      set(CONFIG_INSTALL_PATH cmake)
    else()
      set(BINARY_INSTALL_PATH bin/${project_name}-${project_version})
      set(LIBRARY_INSTALL_PATH lib${LIBSUFFIX}/${project_name}-${project_version})
      set(HEADER_INSTALL_ROOT include/${project_name}/${project_version})
      set(HEADER_INSTALL_PATH ${HEADER_INSTALL_ROOT}/${PROJECT_NAME_PREFIX}${header_name})
      set(CONFIG_INSTALL_PATH ${LIBRARY_INSTALL_PATH}/cmake)
    endif()
  else()
    if(WIN32)
      if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
        set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/binaries" CACHE STRING "Install path" FORCE)
        message(STATUS "Set install prefix to ${CMAKE_INSTALL_PREFIX}")
      endif()
    endif()
    set(BINARY_INSTALL_PATH bin)
    set(LIBRARY_INSTALL_PATH lib${LIBSUFFIX})
    set(HEADER_INSTALL_ROOT include)
    set(HEADER_INSTALL_PATH ${HEADER_INSTALL_ROOT}/${PROJECT_NAME_PREFIX}${header_name})
    set(CONFIG_INSTALL_PATH ${LIBRARY_INSTALL_PATH}/cmake)
  endif()

  set(BINARY_INSTALL_PATH ${BINARY_INSTALL_PATH} PARENT_SCOPE)
  set(LIBRARY_INSTALL_PATH ${LIBRARY_INSTALL_PATH} PARENT_SCOPE)
  set(HEADER_INSTALL_ROOT ${HEADER_INSTALL_ROOT} PARENT_SCOPE)
  set(HEADER_INSTALL_PATH ${HEADER_INSTALL_PATH} PARENT_SCOPE)
  set(CONFIG_INSTALL_PATH ${CONFIG_INSTALL_PATH} PARENT_SCOPE)

  if(NOT ARG_QUIET)
    message(STATUS "CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")
    message(STATUS "BINARY_INSTALL_PATH:  ${BINARY_INSTALL_PATH}")
    message(STATUS "LIBRARY_INSTALL_PATH: ${LIBRARY_INSTALL_PATH}")
    message(STATUS "CONFIG_INSTALL_PATH:  ${CONFIG_INSTALL_PATH}")
    message(STATUS "HEADER_INSTALL_ROOT:  ${HEADER_INSTALL_ROOT}")
    message(STATUS "HEADER_INSTALL_PATH:  ${HEADER_INSTALL_PATH}")
  endif()
endfunction()
