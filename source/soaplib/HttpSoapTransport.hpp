#pragma once

#include <memory>
#include <string>

#include <soaplib/SoapTransport.hpp>

namespace soaplib {

/// HTTP implementation of the SOAP transport.
class HttpSoapTransport : public SoapTransport
{
public:
    /// Initializes an HttpSoapTransport instance.
    /// @param[in] serviceAddress URL of the SOAP service
    /// @param[in] timeoutSeconds Request timeout in seconds
    HttpSoapTransport(
        const std::string& serviceAddress,
        int timeoutSeconds = 5);

    /// Destructs the instance.
    ~HttpSoapTransport() override;

    /// Sets a handler for unsolicited responses (no-op for HTTP).
    void SetResponseHandler(ResponseHandler /*handler*/) override {}

    /// Enables logging of HTTP requests and responses.
    void EnableLogging(
        bool enable) override;

    /// Sets the default read timeout for the HTTP requests.
    void SetReadTimeout(
        int timeoutSeconds) override;

    /// Sends the request data and waits for the response.
    std::unique_ptr<xml::Document> Send(
        const xml::Document& request,
        int timeoutSeconds,
        const std::string& soapAction = "",
        HttpMethod method = HttpMethod::Post) override;

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
