#pragma once

#include <memory>
#include <soaplib/xml/xml.hpp>

namespace soaplib {

/// Interface for SOAP transport layers.
class SoapTransport {
public:
    virtual ~SoapTransport() = default;

    /// Sends a SOAP request and returns the response.
    /// @param[in] request The request XML document
    /// @param[in] timeoutSeconds Request timeout in seconds
    /// @param[in] soapAction Optional SOAP action string (required for 1.1)
    /// @returns The response XML document
    /// @throws soaplib::SoapException
    virtual std::unique_ptr<xml::Document> Send(
        const xml::Document& request,
        int timeoutSeconds,
        const std::string& soapAction = "") = 0;

    /// Sets the default read timeout.
    virtual void SetReadTimeout(int timeoutSeconds) = 0;

    /// Enables or disables logging.
    virtual void EnableLogging(bool enable) = 0;
};

} // namespace soaplib
