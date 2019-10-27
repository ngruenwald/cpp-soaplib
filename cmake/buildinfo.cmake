# @file buildinfo.cmake
# @date 2015/08/21
# @author ngruenwald

# sets SVN_REVISION, BUILD_TYPE, BUILD_NODE, RELEASE_TAG

# Extracts build information from environment variables
# param[out] type  Contains
function(get_build_info info)
  if(DEFINED ENV{JENKINS_URL})
    set(origin "Jenkins")
  else()
    set(origin "Local")
  endif()

  if((DEFINED ENV{RELEASE_VERSION}) OR (DEFINED ENV{RELEASE_TAG}))
    set(build "release build $ENV{RELEASE_TAG} $ENV{RELEASE_VERSION}")
  elseif(DEFINED ENV{BUILD_NUMBER})
    set(build "build #$ENV{BUILD_NUMBER}")
  else()
    set(build "build")
  endif()

  set(${info} "${origin} ${build}" PARENT_SCOPE)
endfunction(get_build_info)

# Retrieves the 'last changed' revision number, or zero if SVN is not available.
# param[in]       dir         The working directory to check.
# param[in]       is_svn      Indicates if svn repo.
# param[out]      url         The repository address
# param[out]      revision    The revision number
# param[out]      tag_version If built from tag, the tag version info)
# param[in,opt]   ARGV5       If set to one (1) then print messages
function(get_svn_revision dir is_svn url revision tag_version)
  if(DEFINED ARGV5)
    set(verbosity ${ARGV5})
  else()
    set(verbosity 0)
  endif()

  # default values in case something goes wrong
  set(${is_svn} 0 PARENT_SCOPE)
  set(${revision} 0 PARENT_SCOPE)
  set(${url} "" PARENT_SCOPE)
  set(${tag_version} "" PARENT_SCOPE)

  find_package(Subversion QUIET)
  if(NOT Subversion_FOUND)
    message(STATUS "Subversion not found.")
    return()
  endif()

  execute_process(
    COMMAND
      ${Subversion_SVN_EXECUTABLE} info ${dir}
    RESULT_VARIABLE
      svn_info_result
    OUTPUT_QUIET
    ERROR_QUIET
  )

  if(${svn_info_result} EQUAL 1)
    #message(STATUS "Not a SVN repository.")
    return()
  endif()

  Subversion_WC_INFO(${dir} svn)
    # svn_WC_URL
    # svn_WC_ROOT
    # svn_WC_REVISION
    # svn_WC_LAST_CHANGED_AUTHOR
    # svn_WC_LAST_CHANGED_DATE
    # svn_WC_LAST_CHANGED_REV
    # snv_WC_INFO

  get_tag_version(${svn_WC_URL} tag_version_)
  if(tag_version_)
    set(${tag_version} ${tag_version_} PARENT_SCOPE)
  endif()

  if(NOT ${verbosity} EQUAL 0)
    message("")
    message(STATUS "Using SVN revision ${svn_WC_REVISION} from working copy")
    message(STATUS "URL: ${svn_WC_URL}")
    message(STATUS "Last changed author: ${svn_WC_LAST_CHANGED_AUTHOR}")
    message(STATUS "Last changed rev: ${svn_WC_LAST_CHANGED_REV}")
    message(STATUS "Last changed date: ${svn_WC_LAST_CHANGED_DATE}")
    if(tag_version)
      message(STATUS "Tag version: ${tag_version_}")
    endif()
    message("")
  endif()

  set(${is_svn} 1 PARENT_SCOPE)
  set(${revision} ${svn_WC_LAST_CHANGED_REV} PARENT_SCOPE)
  set(${url} ${svn_WC_URL} PARENT_SCOPE)
endfunction(get_svn_revision)

# Extracts the tag version from the given subversion url
# param[in]  url      The svn URL
# param[out] version  The extracted version (not set if unsucessful)
function(get_tag_version url version)
  set(${version} "" PARENT_SCOPE)
  string(
    REGEX MATCHALL
    "^.*/tags/(.*)/(ver|rel)[-_]([0-9]+)[-_.]([0-9]+)[-_.]([0-9]+)"
    dummy
    ${url}
  )
  if(CMAKE_MATCH_COUNT EQUAL 5)
    set(major ${CMAKE_MATCH_3})
    set(minor ${CMAKE_MATCH_4})
    set(patch ${CMAKE_MATCH_5})
    strip_leading_zeros(${major} major)
    strip_leading_zeros(${minor} minor)
    strip_leading_zeros(${patch} patch)
    set(${version} ${major}.${minor}.${patch} PARENT_SCOPE)
  endif()
endfunction(get_tag_version)

# Retrieves the 'last changed' commit hash, or zero if Git is not available.
# param[in]       dir         The working directory to check.
# param[out]      is_git      Indicates if git repo
# param[out]      url         The repository address
# param[out]      branch      The branch name
# param[out]      commit_hash The commit hash
# param[out]      tag_version The tag version if built from tag, otherwise empty.
# param[in,opt]   ARGV6       If set to one (1) then print messages
function(get_git_info dir is_git url branch commit_hash tag_version)
  if(DEFINED ARGV6)
    set(verbosity ${ARGV6})
  else()
    set(verbosity 0)
  endif()

  # default values in case something goes wrong
  set(${is_git} 0 PARENT_SCOPE)
  set(${url} "" PARENT_SCOPE)
  set(${branch} "" PARENT_SCOPE)
  set(${commit_hash} 0 PARENT_SCOPE)
  set(${tag_version} "" PARENT_SCOPE)

  find_package(Git QUIET)
  if(NOT ${Git_FOUND})
    message(STATUS "Git not found.")
    return()
  endif()

  # test if repo
  execute_process(
    COMMAND
      ${GIT_EXECUTABLE} rev-parse --is-inside-work-tree
    RESULT_VARIABLE
      git_result
    OUTPUT_QUIET
    ERROR_QUIET
  )

  if(NOT ${git_result} EQUAL 0)
    #message(STATUS "Not a Git repository.")
    return()
  endif()

  # get the branch name
  execute_process(
    COMMAND
      ${GIT_EXECUTABLE} symbolic-ref --short HEAD
    RESULT_VARIABLE
      git_result
    OUTPUT_VARIABLE
      git_branch
    ERROR_VARIABLE
      git_error
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_STRIP_TRAILING_WHITESPACE
  )

  # for tags the above command might fail
  if(${git_result} EQUAL 128)
    execute_process(
      COMMAND
        ${GIT_EXECUTABLE} describe --exact-match HEAD
      RESULT_VARIABLE
        git_result
      OUTPUT_VARIABLE
        git_branch
      ERROR_VARIABLE
        git_error
      OUTPUT_STRIP_TRAILING_WHITESPACE
      ERROR_STRIP_TRAILING_WHITESPACE
    )
    # only accept tags
    string(REGEX MATCH "^(tag)/.*$" dummy ${git_branch})
    if(NOT "${CMAKE_MATCH_1}" STREQUAL "tag")
      return()
    endif()
  endif()
  
  if(NOT ${git_result} EQUAL 0)
    message(STATUS "Could not retrieve Git branch name.")
    message("${git_error}")
    return()
  endif()
  
  # get the commit hash
  execute_process(
    COMMAND
      ${GIT_EXECUTABLE} log -1 --format=%H
    RESULT_VARIABLE
      git_result
    OUTPUT_VARIABLE
      git_commit_hash
    ERROR_VARIABLE
      git_error
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_STRIP_TRAILING_WHITESPACE
  )

  if(NOT ${git_result} EQUAL 0)
    message(STATUS "Could not retrieve Git commit hash.")
    message(${git_error})
    return()
  endif()

  # get the project url
  execute_process(
    COMMAND
      ${GIT_EXECUTABLE} config --get remote.origin.url
    RESULT_VARIABLE
      git_result
    OUTPUT_VARIABLE
      git_url
    ERROR_VARIABLE
      git_error
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_STRIP_TRAILING_WHITESPACE
  )

  if(NOT ${git_result} EQUAL 0)
    message(STATUS "Could not retrive Git url.")
    message(${git_error})
    set(git_url "")
  endif()

  if(NOT ${verbosity} EQUAL 0)
    execute_process(
      COMMAND
        ${GIT_EXECUTABLE} log -1 --format="-- Author: %aN%n-- Committer: %cN%n-- Date: %ad%n-- Subject: %s"
      RESULT_VARIABLE
        git_result
      OUTPUT_VARIABLE
        git_info
      ERROR_QUIET
    )

    message("")
    message(STATUS "URL: ${git_url}")
    message(STATUS "Branch: ${git_branch}")
    message(STATUS "Commit: ${git_commit_hash}")
    if(${git_result} EQUAL 0)
      # strip leading and trailing quotes
      string(LENGTH ${git_info} git_info_length)
      math(EXPR git_info_length "${git_info_length} - 3")
      string(SUBSTRING ${git_info} 1 ${git_info_length} git_info)
      message(${git_info})
    endif()
    message("")
  endif()

  set(${is_git} 1 PARENT_SCOPE)
  set(${url} ${git_url} PARENT_SCOPE)
  set(${branch} ${git_branch} PARENT_SCOPE)
  set(${commit_hash} ${git_commit_hash} PARENT_SCOPE)
  
#  string(REGEX MATCH "^(tag)/v(.*)$" dummy ${git_branch})
#  if("${CMAKE_MATCH_1}" STREQUAL "tag")
#    set(${tag_version} "${CMAKE_MATCH_2}" PARENT_SCOPE)
#  endif()
  get_git_tag_version(${git_branch} tag_version_)
  if(tag_version_)
    set(${tag_version} ${tag_version_} PARENT_SCOPE)
  endif()
endfunction(get_git_info)

# Extracts the tag version from the given git tag name
# param[in]  tag      The git tag name
# param[out] version  The extracted version (not set if unsucessful)
function(get_git_tag_version tag version)
  set(${version} "" PARENT_SCOPE)
  string(
    REGEX MATCHALL
    "^tag/v([0-9]+)[-_.]([0-9]+)[-_.]([0-9]+).*$"
    dummy
    ${tag}
  )
  if(CMAKE_MATCH_COUNT EQUAL 3)
    set(major ${CMAKE_MATCH_1})
    set(minor ${CMAKE_MATCH_2})
    set(patch ${CMAKE_MATCH_3})
    strip_leading_zeros(${major} major)
    strip_leading_zeros(${minor} minor)
    strip_leading_zeros(${patch} patch)
    set(${version} ${major}.${minor}.${patch} PARENT_SCOPE)
  endif()
endfunction(get_git_tag_version)

# Removes leading zeros from the given input string
# param[in]  input   The input string
# param[out] output  The trimmed output string
function(strip_leading_zeros input output)
  string(REGEX MATCH "^(0)0*$" dummy ${input})
  if(CMAKE_MATCH_COUNT EQUAL 1)
    set(${output} ${CMAKE_MATCH_1} PARENT_SCOPE)
  else()
    string(REGEX MATCH "^0*([1-9][0-9]*)$" dummy ${input})
    if(CMAKE_MATCH_COUNT EQUAL 1)
      set(${output} ${CMAKE_MATCH_1} PARENT_SCOPE)
    else()
      set(${output} ${input} PARENT_SCOPE)
    endif()
  endif()
endfunction(strip_leading_zeros)

# Reads the 'platform' name from the specified file.
# param[in]   release_file    The path of the source file
# param[out]  platform        The platform name (eg 'CentOS Linux')
function(read_os_release release_file platform)
  file(READ "${release_file}" contents)
  string(REGEX REPLACE ";" "\\\\;" contents "${contents}")
  string(REGEX REPLACE "\n" ";" contents "${contents}")

  foreach(line IN LISTS contents)
    if(NOT "${line}" STREQUAL "")
      string(REGEX REPLACE "(.*)=\"(.*)\"" "\\1;\\2" key_value ${line})
      list(GET key_value 0 key)
      list(GET key_value 1 value)

      if("${key}" STREQUAL "NAME")
        set(${platform} "${value}" PARENT_SCOPE)
      endif()
    endif()
  endforeach()
endfunction(read_os_release)

# Reads the SuSE Linux version from the specified file.
# param[in]   release_file      The path of the source file
# param[out]  release_version   The version information (eg 'sles11_3')
function(read_suse_version release_file release_version)
  file(READ "${release_file}" contents)
  string(REGEX REPLACE ";" "\\\\;" contents "${contents}")
  string(REGEX REPLACE "\n" ";" contents "${contents}")

  set(major "x")
  set(minor "x")

  foreach(line IN LISTS contents)
    if(NOT "${line}" STREQUAL "")
      string(REGEX REPLACE "(.*) = (.*)" "\\1;\\2" key_value ${line})
      list(LENGTH key_value count)
      if(${count} EQUAL 2)
        list(GET key_value 0 key)
        list(GET key_value 1 value)
        if("${key}" STREQUAL "VERSION")
          set(major "${value}")
        endif()
        if("${key}" STREQUAL "PATCHLEVEL")
          set(minor "${value}")
        endif()
      endif()
    endif()
  endforeach()

  if(NOT ${major} STREQUAL "x" AND NOT ${minor} STREQUAL "x")
    set(${release_version} "sles${major}_${minor}" PARENT_SCOPE)
  endif()
endfunction(read_suse_version)

# Reads the Enterprise Linux version from the specified file.
# param[in]   release_file      The path of the source file
# param[out]  release_version   The version information (eg 'el7_2')
function(read_el_version release_file release_version)
  file(READ "${release_file}" contents)
  string(REGEX REPLACE ".* release (.*)\\.(.*)\\..*" "\\1;\\2" parts "${contents}")
  list(LENGTH parts length)
  if(${length} EQUAL 2)
    list(GET parts 0 major)
    list(GET parts 1 minor)
    set(${release_version} "el${major}_${minor}" PARENT_SCOPE)
  endif()
endfunction(read_el_version)

# Detects the build platform (and version)
# param[out] platform   The name of the build platform
# param[out] osrelease  The version of the build platform
function(detect_platform platform osrelease)
  set(PLATFORM "unknown")
  set(OSRELEASE "unknown")

  if(WIN32)
    set(PLATFORM "Windows")
  else()
    if(EXISTS "/etc/centos-release")
      set(PLATFORM "CentOS")
      read_el_version("/etc/centos-release" OSRELEASE)
    elseif(EXISTS "/etc/SuSE-release")
      set(PLATFORM "SuSE")
      read_suse_version("/etc/SuSE-release" OSRELEASE)
    elseif(EXISTS "/etc/redhat-release" OR EXISTS "/etc/redhat_version")
      set(PLATFORM "Red Hat")
      read_el_version("/etc/redhat-release" OSRELEASE)
    elseif(EXISTS "/etc/debian_release" OR EXISTS "/etc/debian_version")
      set(PLATFORM "Debian")
    elseif(EXISTS "/etc/arch-release")
      set(PLATFORM "ArchLinux")
      set(OSRELEASE "")
    elseif(EXISTS "/etc/fedora-release")
      set(PLATFORM "Fedora")
    elseif(EXISTS "/etc/slackware-release" OR EXISTS "/etc/slackware-version")
      set(PLATFORM "Slackware")
    elseif(EXISTS "/etc/mandrake-release")
      set(PLATFORM "Mandrake")
    elseif(EXISTS "/etc/yellowdog-release")
      set(PLATFORM "Yellow dog")
    elseif(EXISTS "/etc/alpine-release")
      set(PLATFORM "Alpine Linux")
    elseif(EXISTS "/etc/os-release")
      read_os_release("/etc/os-release" PLATFORM)
    endif()
  endif()

  set(${platform} "${PLATFORM}" PARENT_SCOPE)
  set(${osrelease} "${OSRELEASE}" PARENT_SCOPE)
endfunction(detect_platform)

# Might set the following variables:
# * JENKINS_URL
# * RELEASE_TAG
# * RELEASE_VERSION
# * BUILD_NODE
# * BUILD_NUMBER
# * BUILD_TYPE
# * SVN_REVISION
# * SVN_URL
# * OSRELEASE
macro(check_build_info)

  if(NOT DEFINED JENKINS_URL)
    if(DEFINED ENV{JENKINS_URL})
      set(JENKINS_URL $ENV{JENKINS_URL})
    endif()
  endif()

  if(NOT DEFINED RELEASE_TAG)
    if(DEFINED ENV{RELEASE_TAG})
      set(RELEASE_TAG $ENV{RELEASE_TAG})
    endif()
  endif()

  if(NOT DEFINED RELEASE_VERSION)
    if(DEFINED ENV{RELEASE_VERSION})
      set(RELEASE_VERSION $ENV{RELEASE_VERSION})
    endif()
  endif()

  if(NOT DEFINED BUILD_NODE)
    if(DEFINED ENV{BUILD_NODE})
      set(BUILD_NODE $ENV{BUILD_NODE})
    else()
      site_name(BUILD_NODE)
    endif()
  endif()

  if(NOT DEFINED BUILD_NUMBER)
    if(DEFINED ENV{BUILD_NUMBER})
      set(BUILD_NUMBER $ENV{BUILD_NUMBER})
    endif()
  endif()

  if((NOT DEFINED SVN_REVISION) OR (NOT DEFINED SVN_URL))
    get_git_info(${CMAKE_SOURCE_DIR} is_git git_url git_branch git_commit tag_version 0)
    if("${is_git}" EQUAL "1")
      # TODO: introduce new variables
      set(PROJECT_VERSION_SCM "${git_commit}")
      string(SUBSTRING ${PROJECT_VERSION_SCM} 0 8 PROJECT_VERSION_SCM_SHORT)
      set(PROJECT_VERSION_TAG "${tag_version}")
      set(SVN_REVISION 0)
      set(SVN_URL ${git_url})
      set(SVN_TAG_VERSION ${git_branch})
    else()
      get_svn_revision(${CMAKE_SOURCE_DIR} is_svn svn_url svn_revision tag_version 0)
      if("${is_svn}" EQUAL "1")
        set(PROJECT_VERSION_SCM "${svn_revision}")
        set(PROJECT_VERSION_SCM_SHORT ${PROJECT_VERSION_SCM})
        set(SVN_REVISION ${PROJECT_VERSION_SCM_SHORT})
        set(SVN_URL ${svn_url})
        set(SVN_TAG_VERSION ${tag_version})
      else()
        message(WARNING "Project is not under version control.")
        set(PROJECT_VERSION_SCM "")
        set(PROJECT_VERSION_SCM_SHORT "")
        set(SVN_REVISION 0)
        set(SVN_URL "")
      endif()
    endif()
  endif()

  # for compatibility
  set(VERSION_SVN ${PROJECT_VERSION_SCM_SHORT})
  set(PROJECT_VERSION_SVN ${PROJECT_VERSION_SCM_SHORT})

  if((NOT DEFINED PLATFORM) OR (NOT DEFINED OSRELEASE))
    detect_platform(platform osrelease)
    if(NOT DEFINED PLATFORM)
      set(PLATFORM ${platform})
    endif()
    if(NOT DEFINED OSRELEASE)
      if(DEFINED ENV{OSRELEASE})
        set(OSRELEASE $ENV{OSRELEASE})
      else()
        set(OSRELEASE ${osrelease})
      endif()
    endif()
  endif()

  get_build_info(BUILD_TYPE)

endmacro(check_build_info)

# Prints the build information
macro(print_build_info)

  message(" ")
  message(STATUS "Version:     ${PROJECT_VERSION}")
  if("${PROJECT_VERSION_SCM}" STREQUAL "${PROJECT_VERSION_SCM_SHORT}")
    message(STATUS "Revision:    ${PROJECT_VERSION_SCM}")
  else()
    message(STATUS "Revision:    ${PROJECT_VERSION_SCM} (${PROJECT_VERSION_SCM_SHORT})")
  endif()
  if(NOT "${PROJECT_VERSION_TAG}" STREQUAL "")
    message(STATUS "Tag Version: ${PROJECT_VERSION_TAG}")
  endif()
  if("${RELEASE_TAG}" STREQUAL "")
    message(STATUS "Release Tag: ---")
  else()
    message(STATUS "Release Tag: ${RELEASE_TAG}")
  endif()
  if(NOT "${PLATFORM}" STREQUAL "")
    if(NOT "${OSRELEASE}" STREQUAL "")
      message(STATUS "Platform:    ${PLATFORM} (${OSRELEASE})")
    else()
      message(STATUS "Platform:    ${PLATFORM}")
    endif()
  endif()
  message(STATUS "Building on: ${CMAKE_SYSTEM_NAME} (${BUILD_NODE})")
  message(" ")

endmacro(print_build_info)

check_build_info()
