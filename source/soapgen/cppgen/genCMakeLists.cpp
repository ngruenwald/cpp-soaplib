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
    stream << "file(GLOB MAIN_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.hpp)" << '\n';
    stream << "file(GLOB TYPE_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} types/*.hpp)" << '\n';
    stream << '\n';

    stream << "add_library(" << target << " STATIC ${SOURCES} ${MAIN_HEADERS} ${TYPE_HEADERS})" << '\n';
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

    stream << '\n';

    stream << "if(WIN32)" << '\n';
    stream << "  set_target_properties(" << target << " PROPERTIES"
           << " RELEASE_POSTFIFX \"_static_${COMPILER_POSTFIX}\""
           << " RELWITHDEBINFO_POSTFIX \"_static_${COMPILER_POSTFIX}\""
           << " DEBUG_POSTFIX \"_static_${COMPILER_POSTFIX}d\")" << '\n';
    stream << "else()" << '\n';
    stream << "  set_target_properties(" << target << " PROPERTIES"
           << " RELEASE_POSTFIX \"-${PROJECT_VERSION}\""
           << " RELWITHDEBINFO_POSTFIX \"-${PROJECT_VERSION}\""
           << " DEBUG_POSTFIX \"-${PROJECT_VERSION}d\")" << '\n';
    stream << "endif()" << '\n';

    stream << '\n';

    stream << "install(FILES ${MAIN_HEADERS} DESTINATION ${HEADER_INSTALL_PATH}/"
           << target << ")" << '\n';
    stream << "install(FILES ${TYPE_HEADERS} DESTINATION ${HEADER_INSTALL_PATH}/"
           << target << "/types)" << '\n';

    stream << '\n';

    if (!options.cmakeExport.empty())
    {
        stream << "install(\n"
            << "  TARGETS " << target << "\n"
            << "  EXPORT  " << options.cmakeExport << "\n"
            << "  RUNTIME DESTINATION ${LIBRARY_INSTALL_PATH}\n"
            << "  LIBRARY DESTINATION ${LIBRARY_INSTALL_PATH}\n"
            << "  ARCHIVE DESTINATION ${LIBRARY_INSTALL_PATH}\n"
            << ")\n";

        stream << '\n';
    }
}

} // namespace cmake
} // namespace cppgen
