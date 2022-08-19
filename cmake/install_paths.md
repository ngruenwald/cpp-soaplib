# Install Paths

This module can be used to prepare variables for file and target installation.

Variables:

* _CONFIG_INSTALL_PATH_  Location for cmake config installation.
* _BINARY_INSTALL_PATH_  Location for binary installation.
* _LIBRARY_INSTALL_PATH_ Location for library installation.
* _HEADER_INSTALL_PATH_  Location for header installation.
* _HEADER_INSTALL_ROOT_  The root location for header installation.


## Usage

Most basic example - no parameters.

See _install_paths.cmake_ for more options.

```
project(mylib VERSION 1.0.0)

include(install_paths.cmake)

setup_install_paths()

install(
  TARGETS mylib
  EXPORT  mylibTargets
  RUNTIME DESTINATION ${LIBRARY_INSTALL_PATH}
  LIBRARY DESTINATION ${LIBRARY_INSTALL_PATH}
  ARCHIVE DESTINATION ${LIBRARY_INSTALL_PATH}
)

install(FILE ${PUBLIC_HEADERS} DESTINATION ${HEADER_INSTALL_PATH})
```
