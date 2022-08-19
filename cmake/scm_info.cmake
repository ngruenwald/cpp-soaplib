# variables
#
# * PROJECT_VERSION_SCM (revision)
#
# * SCM_TYPE
# * SCM_BRANCH
# * SCM_REVISION
# * SCM_TAG_VERSION
# * SCM_TAG_VERSION_MAJOR
# * SCM_TAG_VERSION_MINOR
# * SCM_TAG_VERSION_PATCH
# * SCM_TAG_VERSION_TWEAK

function(_scm_info_get_git_info found branch revision)
  find_program(GIT git)
  if(GIT)
    execute_process(COMMAND ${GIT} branch --show-current OUTPUT_VARIABLE git_bra ERROR_QUIET)
    string(STRIP "${git_bra}" git_bra)
    if ("${git_bra}" STREQUAL "")
      execute_process(COMMAND ${GIT} describe --exact-match --tags OUTPUT_VARIABLE git_bra ERROR_QUIET)
      string(STRIP "${git_bra}" git_bra)
    endif()
    execute_process(COMMAND ${GIT} log --pretty=format:%h -n 1 OUTPUT_VARIABLE git_rev ERROR_QUIET)
    string(STRIP "${git_rev}" git_rev)
    set(${found}    TRUE       PARENT_SCOPE)
    set(${branch}   ${git_bra} PARENT_SCOPE)
    set(${revision} ${git_rev} PARENT_SCOPE)
  else()
    set(${found} FALSE PARENT_SCOPE)
  endif()
endfunction()

function(_scm_info_extract_tag_version tag full major minor patch tweak)
  if ("${tag}" MATCHES "tag/v.*")
    string(SUBSTRING "${tag}" 5 -1 vstr)
    set(${full} ${vstr} PARENT_SCOPE)

    string(REPLACE "." ";" vlst ${vstr})
    list(LENGTH vlst vlen)

    if(${vlen} GREATER 0)
      list(GET vlst 0 tmp)
      set(${major} ${tmp} PARENT_SCOPE)
    endif()
    if(${vlen} GREATER 1)
      list(GET vlst 1 tmp)
      set(${minor} ${tmp} PARENT_SCOPE)
    endif()
    if(${vlen} GREATER 2)
      list(GET vlst 2 tmp)
      set(${patch} ${tmp} PARENT_SCOPE)
    endif()
    if(${vlen} GREATER 3)
      list(GET vlst 3 tmp)
      set(${tweak} ${tmp} PARENT_SCOPE)
    endif()
  endif()
endfunction()

macro(get_scm_info)
  _scm_info_get_git_info(found branch revision)
  if(found)
    set(SCM_TYPE "git")
    set(SCM_BRANCH "${branch}")
    set(SCM_REVISION "${revision}")
    set(PROJECT_VERSION_SCM ${SCM_REVISION})
    _scm_info_extract_tag_version(
      "${SCM_BRANCH}"
      SCM_TAG_VERSION
      SCM_TAG_VERSION_MAJOR
      SCM_TAG_VERSION_MINOR
      SCM_TAG_VERSION_PATCH
      SCM_TAG_VERSION_TWEAK
    )
  else()
    message(DEBUG "git not found")
  endif()

  if("${SCM_TYPE}" STREQUAL "")
    set(SCM_TYPE "none")
  endif()
endmacro()

function(print_scm_info)
  set(options COMPACT)
  set(oneValueArgs )
  set(multiValueArgs )
  cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  if(ARG_COMPACT)
    set(msg "SCM: ${SCM_TYPE} \"${SCM_BRANCH}\" #${SCM_REVISION}")
    if(SCM_TAG_VERSION)
      set(msg "${msg} (tag version: ${SCM_TAG_VERSION})")
    endif()
    message(STATUS "${msg}")
  else()
    message("SCM:")
    message("  TYPE:        ${SCM_TYPE}")
    message("  BRANCH:      ${SCM_BRANCH}")
    message("  REVISION:    ${SCM_REVISION}")
    message("  TAG_VERSION: ${SCM_TAG_VERSION}")
    if(SCM_TAG_VERSION)
      message("    MAJOR:     ${SCM_TAG_VERSION_MAJOR}")
      message("    MINOR:     ${SCM_TAG_VERSION_MINOR}")
      message("    PATCH:     ${SCM_TAG_VERSION_PATCH}")
      message("    TWEAK:     ${SCM_TAG_VERSION_TWEAK}")
    endif()
  endif()
endfunction()

get_scm_info()
