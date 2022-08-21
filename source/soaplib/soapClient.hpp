#pragma once

#include <memory>
#include <string>

#include <soaplib/xml/document.hpp>

namespace soaplib {

/// SOAP Client
class SoapClient
{
public:
    /// Initializes a SoapClient instance.
    /// @param[in] serviceAddress URL of the SOAP service
    /// @param[in] timeoutSeconds Request timeout in seconds
    SoapClient(
        const std::string& serviceAddress,
        int timeoutSeconds = 5);

    /// Destructs the instance.
    ~SoapClient();

    /// Enables logging of HTTP requests and responses.
    /// @param[in] enable If true, logging is enabled, otherwise disabled
    void EnableLogging(
        bool enable);

    /// Sets the default read timeout for the HTTP requests.
    /// @param[in] timeoutSeconds Read timeout in seconds
    void SetReadTimeout(
        int timeoutSeconds);

    /// Sends the request data and waits for the response.
    /// @param[in] request The request data
    /// @returns The response data
    /// @throws soaplib::SoapException
    std::unique_ptr<xml::Document> Send(
        const xml::Document& request);

    /// Sends the request data and waits for the response, using a custom timeout.
    /// @param[in] request The request data
    /// @param[in] timeoutSeconds Request timeout in seconds
    /// @returns The response data
    /// @throws soaplib::SoapException
    std::unique_ptr<xml::Document> Send(
        const xml::Document& request,
        int timeoutSeconds);

private:
    void extractAddressParts(
        const std::string& serviceAddress);

private:
    std::string host_;
    int port_;
    std::string path_;
    int timeout_;   // seconds
    bool logging_ = false;

};

} // namespace soaplib
