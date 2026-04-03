cmake_minimum_required(VERSION 3.10)
project(CountryInfoService VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT TARGET soaplib::soaplib)
  find_package(cpp-soaplib REQUIRED)
endif()

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
file(GLOB MAIN_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.hpp)
file(GLOB TYPE_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} types/*.hpp)

add_library(CountryInfoService STATIC ${SOURCES} ${MAIN_HEADERS} ${TYPE_HEADERS})
add_library(country::CountryInfoService ALIAS CountryInfoService)

target_link_libraries(CountryInfoService PUBLIC soaplib::soaplib)
target_include_directories(CountryInfoService PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
  $<INSTALL_INTERFACE:${HEADER_INSTALL_PATH}>
)

if(WIN32)
  set_target_properties(CountryInfoService PROPERTIES
    RELEASE_POSTFIX "_static_${COMPILER_POSTFIX}"
    RELWITHDEBINFO_POSTFIX "_static_${COMPILER_POSTFIX}"
    DEBUG_POSTFIX "_static_${COMPILER_POSTFIX}d"
  )
else()
  set_target_properties(CountryInfoService PROPERTIES
    RELEASE_POSTFIX "-${PROJECT_VERSION}"
    RELWITHDEBINFO_POSTFIX "-${PROJECT_VERSION}"
    DEBUG_POSTFIX "-${PROJECT_VERSION}d"
  )
endif()

install(FILES ${MAIN_HEADERS} DESTINATION ${HEADER_INSTALL_PATH}/CountryInfoService)
install(FILES ${TYPE_HEADERS} DESTINATION ${HEADER_INSTALL_PATH}/CountryInfoService/types)

