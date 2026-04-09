
set(target cppgen)

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
  genServerHeader.cpp
  genServerHeader.hpp
  genServerImpl.cpp
  genServerImpl.hpp
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
  Renderer.cpp
  Renderer.hpp
  toJson.cpp
  toJson.hpp
  ../wsdl.cpp
  ../ResourceResolver.cpp
  ../ResourceResolver.hpp
)

include(embedded_resources)
file(GLOB TEMPLATES "${CMAKE_CURRENT_SOURCE_DIR}/../templates/*.tpl")
generate_resources(TARGET templates_res RESOURCES ${TEMPLATES})

add_library(${target} STATIC ${SOURCES})
target_link_libraries(${target} PUBLIC soaplib::xml inja nlohmann_json httplib::httplib templates_res)
target_compiler_warnings(${target})

if(WIN32)
  set_target_properties(${target} PROPERTIES COMPILE_FLAGS "/DLIBXML_STATIC")
endif()

target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../)
target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/../)
# external
target_include_directories(${target} PUBLIC ${DIGCOM_LIB_INCLUDES})

set(CONFIG_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "cppgen include directory")
