if(NOT DEFINED SOAPGEN)
  message(FATAL_ERROR "SOAPGEN is not set")
endif()

if(NOT DEFINED WSDL)
  message(FATAL_ERROR "WSDL is not set")
endif()

if(NOT DEFINED GENERATED_DIR)
  message(FATAL_ERROR "GENERATED_DIR is not set")
endif()

if(NOT DEFINED COMPILE_DIR)
  message(FATAL_ERROR "COMPILE_DIR is not set")
endif()

if(NOT DEFINED PROJECT_SOURCE_DIR)
  message(FATAL_ERROR "PROJECT_SOURCE_DIR is not set")
endif()

file(REMOVE_RECURSE "${GENERATED_DIR}" "${COMPILE_DIR}")
file(MAKE_DIRECTORY "${GENERATED_DIR}")

execute_process(
  COMMAND
    "${SOAPGEN}"
    --input "${WSDL}"
    --output "${GENERATED_DIR}"
    --namespace auth
    --cmake-namespace auth
    --types-folder types
  RESULT_VARIABLE generate_result
)

if(NOT generate_result EQUAL 0)
  message(FATAL_ERROR "AuthenticationService code generation failed: ${generate_result}")
endif()

file(WRITE "${COMPILE_DIR}/CMakeLists.txt" [=[
cmake_minimum_required(VERSION 3.16)
project(AuthenticationServiceGeneratedCompile LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(LibXml2 REQUIRED)

file(GLOB_RECURSE GENERATED_SOURCES "@GENERATED_DIR@/auth/*.cpp")

add_library(authentication_service_generated OBJECT ${GENERATED_SOURCES})
target_include_directories(
  authentication_service_generated
  PRIVATE
    "@GENERATED_DIR@"
    "@GENERATED_DIR@/auth"
    "@PROJECT_SOURCE_DIR@/source"
    "@PROJECT_SOURCE_DIR@/source/soaplib"
    "@PROJECT_SOURCE_DIR@/source/soaplib/xml"
    ${LIBXML2_INCLUDE_DIRS}
)
]=])

file(READ "${COMPILE_DIR}/CMakeLists.txt" compile_cmake)
string(REPLACE "@GENERATED_DIR@" "${GENERATED_DIR}" compile_cmake "${compile_cmake}")
string(REPLACE "@PROJECT_SOURCE_DIR@" "${PROJECT_SOURCE_DIR}" compile_cmake "${compile_cmake}")
file(WRITE "${COMPILE_DIR}/CMakeLists.txt" "${compile_cmake}")

set(configure_args
  -S "${COMPILE_DIR}"
  -B "${COMPILE_DIR}/build"
)

if(DEFINED CMAKE_CXX_COMPILER AND NOT CMAKE_CXX_COMPILER STREQUAL "")
  list(APPEND configure_args -D "CMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}")
endif()

execute_process(
  COMMAND "${CMAKE_COMMAND}" ${configure_args}
  RESULT_VARIABLE configure_result
)

if(NOT configure_result EQUAL 0)
  message(FATAL_ERROR "AuthenticationService compile project configuration failed: ${configure_result}")
endif()

set(build_args --build "${COMPILE_DIR}/build")

if(DEFINED CMAKE_BUILD_TYPE AND NOT CMAKE_BUILD_TYPE STREQUAL "" AND NOT CMAKE_BUILD_TYPE STREQUAL "$<CONFIG>")
  list(APPEND build_args --config "${CMAKE_BUILD_TYPE}")
endif()

execute_process(
  COMMAND "${CMAKE_COMMAND}" ${build_args}
  RESULT_VARIABLE build_result
)

if(NOT build_result EQUAL 0)
  message(FATAL_ERROR "AuthenticationService generated code compile failed: ${build_result}")
endif()
