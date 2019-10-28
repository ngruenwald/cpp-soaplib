#include "genCMakeLists.hpp"

#include <ostream>
#include <wsdl.hpp>

#include "options.hpp"

namespace cppgen {
namespace cmake {

void GenerateCMakeLists(
    std::ostream& stream,
    const Options& options,
    const Definition& definition)
{
    std::string target = definition.name.name;

    stream << "find_package(soaplib REQUIRED)" << '\n';
    stream << '\n';

    stream << "file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)" << '\n';
    stream << "file(GLOB_RECURSE HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.hpp)" << '\n';
    stream << '\n';

    stream << "add_library(" << target << " STATIC ${SOURCES} ${HEADERS})" << '\n';
    if (!options.cmakeNamespace.empty())
    {
        stream << "add_library(" << options.cmakeNamespace << "::" << target << " ALIAS " << target << ")" << '\n';
    }
    stream << '\n';

    stream << "target_link_libraries(" << target << " PUBLIC soaplib::soaplib)" << '\n';
    stream << "target_include_directories(" << target << " PUBLIC "
           << "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..> "
           << "$<INSTALL_INTERFACE:${HEADER_INSTALL_PATH}>"
           << ")" << '\n';
}

} // namespace cmake
} // namespace cppgen
