

add_library(
  contrib-argparse
  INTERFACE
)

target_include_directories(
  contrib-argparse
  INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(USE_OPENSSL)
  set_target_properties(
    contrib-argparse
    PROPERTIES
    INTERFACE_COMPILE_DEFINITIONS CPPHTTPLIB_OPENSSL_SUPPORT
  )
endif()
