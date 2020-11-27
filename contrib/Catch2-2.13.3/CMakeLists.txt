

add_library(
  contrib-catch2
  INTERFACE
)

target_include_directories(
  contrib-catch2
  INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
