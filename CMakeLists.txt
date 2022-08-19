cmake_minimum_required(VERSION 3.20)

project(
  soaplib
  VERSION 0.1.0
  DESCRIPTION "C++ SOAP library"
  HOMEPAGE_URL "https://github.com/ngruenwald/cpp-soaplib"
  LANGUAGES C CXX
)

set(PROJECT_LICENSE MIT)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/contrib/libs)

option(USE_OPENSSL "Enable SSL support" ON)
option(UNIT_TESTS "Build the unit tests." ON)
option(EXAMPLES "Build the examples. (requires generated code!)" OFF)
option(INSTALL_VERSIONED "Create versioned installation." ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(sanitizers)
include(compiler_warnings)
include(install_paths)
include(library_suffix)
include(os_info)
include(scm_info)
include(packaging)

if(INSTALL_VERSIONED)
  setup_install_paths(VERSIONED)
else()
  setup_install_paths(QUIET)
endif()

print_os_info(COMPACT)
print_scm_info(COMPACT)

add_subdirectory(contrib)
add_subdirectory(source)
add_subdirectory(docs)
add_subdirectory(package)
