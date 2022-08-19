#
# add_library_suffix(target [VERSION version] [QIET] [VERSIONED])
#
# @param[in] VERSION   The project version.
# @param[in] QUIET     If TRUE will not print postfix info.
# @param[in] VERSIONED If TRUE will install in versioned folders.
#
# @modifies Target property RELEASE_POSTFIX
# @modifies Target property RELWITHDEBINFO_POSTFIX
# @modifies Target property DEBUG_POSTFIX
#
function(add_library_suffix target)
  set(options QUIET VERSIONED)
  set(oneValueArgs VERSION)
  cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "" ${ARGN})

  get_target_property(target_type ${target} TYPE)

  if("${target_type}" STREQUAL "EXECUTABLE")
    message(WARNING "add_library_suffix cannot be applied to executables")
    return()
  endif()

  set(postfix "")

  if(WIN32)
    if("${target_type}" STREQUAL "STATIC_LIBRARY")
      set(postfix "_static")
    endif()

    set(postfix "${postfix}_${CMAKE_VS_TOOTFORM_TOOLSET}")

    if(${CMAKE_SIZEOF_VOID_P} EQUAL 8)
      set(postfix "${postfix}_x64")
    endif()
  else()
    set(project_version "")

    if(${ARG_VERSIONED})
      if(DEFINED ARG_VERSION AND NOT "${ARG_VERSION}" STREQUAL "")
        set(project_version ${ARG_VERSION})
      else()
        set(project_version ${PROJECT_VERSION})
      endif()
    endif()

    if(NOT "${project_version}" STREQUAL "")
      set(postfix "-${project_version}")
    endif()
  endif()

  if(NOT ${ARG_QUIET})
    message(STATUS "Library suffix: ${postfix}[d]")
  endif()

  set_target_properties(
    ${target}
    PROPERTIES
    RELEASE_POSTFIX "${postfix}"
    RELWITHDEBINFO_POSTFIX "${postfix}"
    MINSIZEREL_POSTFIX "${postfix}"
    DEBUG_POSTFIX "${postfix}d"
  )
endfunction()
