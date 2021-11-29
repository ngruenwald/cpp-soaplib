
find_program(CLANG_TIDY_EXE NAMES "clang-tidy")

if(CLANG_TIDY_EXE)
  set(
    CMAKE_CXX_CLANG_TIDY
    "${CLANG_TIDY_EXE};--format-style=file;--header-filter='${CMAKE_SOURCE_DIR}/*'"
    CACHE STRING ""
    FORCE
  )
else()
  message(STATUS "clang-tidy NOT found")
  set(CMAKE_CXX_CLANG_TIDY "" CACHE STRING "" FORCE)
endif()
