# Library Suffix

This module can be used to append a suffix to library binaries.

On Windows the suffix will container the toolkit version and for 64-bit
builds the architecture.

On Linux the suffix will contain the version, if the _VERSIONED_ flag is set.

Debug builds will be marked with a trailing _d_.


## Usage

Most basic example.

See _library_suffix.cmake_ for more options.

```
project(mylib VERSION 1.0.0)

include(library_suffix.cmake)

add_library(mylib STATIC mylib.cpp)

add_library_suffix(mylib)
```
