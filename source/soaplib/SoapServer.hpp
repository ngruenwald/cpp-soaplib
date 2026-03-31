#pragma once

#include <soaplib/SoapBase.hpp>

namespace soaplib {

/// Base class for SOAP server stubs.
class SoapServer : public SoapBase {
public:
    virtual ~SoapServer() = default;

    /// Handles a SOAP request.
    /// @param[in] request The request XML document
    /// @returns The response XML document
    virtual std::unique_ptr<xml::Document> HandleRequest(
        const xml::Document& request) = 0;
};

} // namespace soaplib
