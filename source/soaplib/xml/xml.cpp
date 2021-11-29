#include "xml.hpp"

namespace soaplib {
namespace xml {

/// Sets the logger callback
/// @param[in] logHandler The function to call when an error needs to be logged.
/// @param[in] logContext User parameter passed to the log function.
void SetErrorLogger(
    xmlGenericErrorFunc logHandler,
    void* logContext)
{
    xmlSetGenericErrorFunc(logContext, logHandler);
}

/// Initializes the XML parser
/// @param[in] logHandler The function to call when an error needs to be logged.
/// @param[in] logContext User parameter passed to the log function.
void Init(
    xmlGenericErrorFunc logHandler,
    void* logContext)
{
    xmlInitParser();

    if (logHandler || logContext)
    {
        SetErrorLogger(logHandler, logContext);
    }
}

/// Cleans up the allocated memory
void Cleanup()
{
    xmlCleanupParser();
}

} // namespace xml
} // namespace soaplib
