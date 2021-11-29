include(CMakeFindDependencyMacro)

find_dependency(LibXml2)
find_dependency(OpenSSL)

include(${CMAKE_CURRENT_LIST_DIR}/soaplibTargets.cmake)
