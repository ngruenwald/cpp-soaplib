#include "Xml.hpp"

namespace soaplib::xml {

void SetErrorLogger(
    xmlGenericErrorFunc logHandler,
    void* logContext)
{
    xmlSetGenericErrorFunc(logContext, logHandler);
}

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

void Cleanup()
{
    xmlCleanupParser();
}

} // namespace soaplib::xml
