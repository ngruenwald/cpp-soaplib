if(NOT TARGET soaplib::soaplib)
  find_package(cpp-soaplib REQUIRED)
endif()

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
file(GLOB MAIN_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.hpp)
file(GLOB TYPE_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} types/*.hpp)

add_library(calculator-service STATIC ${SOURCES} ${MAIN_HEADERS} ${TYPE_HEADERS})
add_library(calc::calculator-service ALIAS calculator-service)

target_link_libraries(calculator-service PUBLIC soaplib::soaplib)
target_include_directories(calculator-service PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
  $<INSTALL_INTERFACE:${HEADER_INSTALL_PATH}>
)

if(WIN32)
  set_target_properties(calculator-service PROPERTIES
    RELEASE_POSTFIFX "_static_${COMPILER_POSTFIX}"
    RELWITHDEBINFO_POSTFIX "_static_${COMPILER_POSTFIX}"
    DEBUG_POSTFIX "_static_${COMPILER_POSTFIX}d"
  )
else()
  set_target_properties(calculator-service PROPERTIES
    RELEASE_POSTFIX "-${PROJECT_VERSION}"
    RELWITHDEBINFO_POSTFIX "-${PROJECT_VERSION}"
    DEBUG_POSTFIX "-${PROJECT_VERSION}d"
  )
endif()

install(FILES ${MAIN_HEADERS} DESTINATION ${HEADER_INSTALL_PATH}/calculator-service)
install(FILES ${TYPE_HEADERS} DESTINATION ${HEADER_INSTALL_PATH}/calculator-service/types)

