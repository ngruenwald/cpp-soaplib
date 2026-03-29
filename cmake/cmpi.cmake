# https://github.com/ngruenwald/cmake-project
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.28)

#
# Include at the top of your main CMakeLists.txt, e.g
#---------------------------------------
# cmake_minimum_required(VERSION 3.28)
# include(cmpi.cmake)
# project(...)
#---------------------------------------
#
# Variables:
#
# * CMP_VERSION     ... Version to use, defaults to 'main'
# * CMP_SOURCE_URL  ... Source ULR, defaults to https://github.com/ngruenwald/cmake-project
# * CMP_INSTALL_DIR ... Local install directory, defaults to CMAKE_BINARY_DIR/.cmake-project
#

function(_install_cmake_project path workdir url version)
  if("${version}" STREQUAL "")
    set(version "main")
  endif()

  if("${url}" STREQUAL "")
    set(url "https://github.com/ngruenwald/cmake-project/archive/${version}.tar.gz")
  endif()

  if("${workdir}" STREQUAL "")
    if(DEFINED CMAKE_CURRENT_BINARY_DIR)
      set(workdir "${CMAKE_CURRENT_BINARY_DIR}")
    elseif(DEFINED CMAKE_BINARY_DIR)
      set(workdir "${CMAKE_BINARY_DIR}")
    else()
      set(workdir "${CMAKE_CURRENT_FUNCTION_LIST_DIR}")
    endif()
    set(workdir "${workdir}/.cmake-project")
  endif()

  include(FetchContent)
  FetchContent_Populate(
    cmake-project
    EXCLUDE_FROM_ALL
    QUIET
    URL "${url}"
    UPDATE_DISCONNECTED
    SUBBUILD_DIR "${workdir}/subbuild"
    SOURCE_DIR "${workdir}/source"
    BINARY_DIR "${workdir}/binary"
  #  CONFIGURE_COMMAND
  #  BUILD_COMMAND
  #  INSTALL_COMMAND
  #  TEST_COMMAND
  )
  set(${path} "${workdir}/source/cmake" PARENT_SCOPE)
endfunction()

function(_cmpi_get_script_dir script_dir)
  set(${script_dir} "${CMAKE_CURRENT_FUNCTION_LIST_DIR}" PARENT_SCOPE)
endfunction()

_install_cmake_project(_cmp_install_path "${CMP_INSTALL_DIR}" "${CMP_SOURCE_URL}" "${CMP_VERSION}")

if(NOT DEFINED CMAKE_PROJECT_EXTRA_MODULES_DIR)
  _cmpi_get_script_dir(_cmpi_script_dir)
  set(CMAKE_PROJECT_EXTRA_MODULES_DIR ${_cmpi_script_dir})
endif()

include("${_cmp_install_path}/cmake_project.cmake")
