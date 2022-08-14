cmake_minimum_required(VERSION 3.2)

project(soaplib VERSION 0.1.0 DESCRIPTION "C++ SOAP library" LANGUAGES C CXX)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/contrib/libs)

option(USE_OPENSSL "Enable SSL support" ON)
option(UNIT_TESTS "Build the unit tests." ON)
option(EXAMPLES "Build the examples. (requires generated code!)" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(sanitizers)
include(warnings)

add_subdirectory(contrib)
add_subdirectory(source)
add_subdirectory(docs)
add_subdirectory(package)
