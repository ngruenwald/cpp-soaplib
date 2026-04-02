cmake_minimum_required(VERSION 3.10)
project({{ target }} VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT TARGET soaplib::soaplib)
  find_package(cpp-soaplib REQUIRED)
endif()

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
file(GLOB MAIN_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.hpp)
file(GLOB TYPE_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} types/*.hpp)

add_library({{ target }} STATIC ${SOURCES} ${MAIN_HEADERS} ${TYPE_HEADERS})
{% if length(options.cmakeNamespace) > 0 %}
add_library({{ options.cmakeNamespace }}::{{ target }} ALIAS {{ target }})
{% endif %}

target_link_libraries({{ target }} PUBLIC soaplib::soaplib)
target_include_directories({{ target }} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
  $<INSTALL_INTERFACE:${HEADER_INSTALL_PATH}>
)

if(WIN32)
  set_target_properties({{ target }} PROPERTIES
    RELEASE_POSTFIX "_static_${COMPILER_POSTFIX}"
    RELWITHDEBINFO_POSTFIX "_static_${COMPILER_POSTFIX}"
    DEBUG_POSTFIX "_static_${COMPILER_POSTFIX}d"
  )
else()
  set_target_properties({{ target }} PROPERTIES
    RELEASE_POSTFIX "-${PROJECT_VERSION}"
    RELWITHDEBINFO_POSTFIX "-${PROJECT_VERSION}"
    DEBUG_POSTFIX "-${PROJECT_VERSION}d"
  )
endif()

install(FILES ${MAIN_HEADERS} DESTINATION ${HEADER_INSTALL_PATH}/{{ target }})
install(FILES ${TYPE_HEADERS} DESTINATION ${HEADER_INSTALL_PATH}/{{ target }}/types)

{% if length(options.cmakeExport) > 0 %}
install(
  TARGETS {{ target }}
  EXPORT  {{ options.cmakeExport }}
  RUNTIME DESTINATION ${LIBRARY_INSTALL_PATH}
  LIBRARY DESTINATION ${LIBRARY_INSTALL_PATH}
  ARCHIVE DESTINATION ${LIBRARY_INSTALL_PATH}
)
{% endif %}
