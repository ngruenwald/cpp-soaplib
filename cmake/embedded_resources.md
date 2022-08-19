# Embedded Ressources

This module creates source code files containing an _unsigned char array_ with
the data from the given input files. (function _generate_resource_file_)

Additionally it can create a header file with some helper macros for easier
resource access. (function _generate_resources_header_)

## Usage

This is a basic usage example.

See _embedded_resources.cmake_ for more options.

### Add resources via CMake

```
include(embedded_resources.cmake)

generate_resource_file(CMakeLists.txt)
generate_resource_file(modules/embedded_resources.md)

generate_resources_header(RESOURCES CMakeLists.txt embedded_resources.md)

add_executable(
  my_target
  main.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/CMakeLists.txt.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/embedded_resources.md.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/resources.h
)

target_include_directories(my_target PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
```
or use the all-in-one function to generate a static _resources_ library

```
include(embedded_resources.cmake)
generate_resources(TARGET resources RESOURCES CMakeLists.txt modules/embedded_resources.md)
add_executable(my_target main.cpp)
target_link_libraries(my_target resources)
```

### Use resources in code

```
#include <iostream>
#include "resources.h"

int main(int argc, char** argv)
{
    std::cout << embedded_resources_md_data << '\n' << '\n';
    std::cout << "size: " << embedded_resources_md_size << " bytes" << '\n';
    return 0;
}
```
