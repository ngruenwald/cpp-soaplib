include(CMakeFindDependencyMacro)

#find_dependency(xmlwrp)
find_dependency(OpenSSL)

include(${CMAKE_CURRENT_LIST_DIR}/soaplibTargets.cmake)
