cmake_minimum_required(VERSION 3.0)

set(TARGET cppgen)

set(
  SOURCES
  cppgen.cpp
  cppgen.hpp
  options.hpp
  typeMap.cpp
  typeMap.hpp
  utils.cpp
  utils.hpp
  genServiceHeader.cpp
  genServiceHeader.hpp
  genServiceImpl.cpp
  genServiceImpl.hpp
  genServiceHelper.cpp
  genServiceHelper.hpp
  genTypeHeader.cpp
  genTypeHeader.hpp
  genTypeImpl.cpp
  genTypeImpl.hpp
  genAnyTypeHeader.cpp
  genAnyTypeHeader.hpp
  genAnyTypeImpl.cpp
  genAnyTypeImpl.hpp
  genCMakeLists.cpp
  genCMakeLists.hpp
)

add_library(${TARGET} STATIC ${SOURCES})

if(WIN32)
  set_target_properties(${TARGET} PROPERTIES COMPILE_FLAGS "/DLIBXML_STATIC")
endif()

target_include_directories(${TARGET} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${TARGET} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../)
target_include_directories(${TARGET} PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(${TARGET} PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/../)
# external
target_include_directories(${TARGET} PUBLIC ${DIGCOM_LIB_INCLUDES})

set(CONFIG_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "cppgen include directory")
