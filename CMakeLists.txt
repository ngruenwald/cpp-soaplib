cmake_minimum_required(VERSION 3.30)

option(USE_OPENSSL "Enable SSL support" ON)
option(UNIT_TESTS "Build the unit tests." ON)
option(EXAMPLES "Build the examples. (requires generated code!)" OFF)
option(INSTALL_VERSIONED "Create versioned installation." ON)
option(USE_SPHINX "Create documentation with Sphinx." OFF)

set(CMP_VERSION dependency_order)
include(cmake/cmpi.cmake)

project(${CM_PROJECT_PARAMS})

set(PROJECT_LICENSE MIT)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

get_target_property(cmp_cmake_modules_dir cmake_modules MODULES_PATH)
list(APPEND CMAKE_MODULE_PATH "${cmp_cmake_modules_dir}")

include(sanitizers)
include(compiler_warnings)
include(install_paths)
include(library_suffix)
include(os_info)
include(scm_info)
include(packaging)

if(INSTALL_VERSIONED)
  setup_install_paths(HEADER_NAME soaplib VERSIONED)
else()
  setup_install_paths(HEADER_NAME soaplib QUIET)
endif()

print_os_info(COMPACT)
print_scm_info(COMPACT)

add_subdirectory(source)
add_subdirectory(docs)
add_subdirectory(package)
