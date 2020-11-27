

# add_library(
#   contrib-cpp-httplib
#   INTERFACE IMPORTED
# )

# set_target_properties(
#   contrib-cpp-httplib
#   PROPERTIES
#   INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE}/include
# )


add_library(
  contrib-cpp-httplib
  INTERFACE
)

target_include_directories(
  contrib-cpp-httplib
  INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(USE_OPENSSL)
  set_target_properties(
    contrib-cpp-httplib
    PROPERTIES
    INTERFACE_COMPILE_DEFINITIONS CPPHTTPLIB_OPENSSL_SUPPORT
  )
endif()
