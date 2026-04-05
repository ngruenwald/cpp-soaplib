#pragma once

#include <memory>
#include <string>
#include <map>

#include <soaplib/SoapTransport.hpp>

namespace soaplib {

/// Configuration for HTTP transport
struct HttpConfig {
    int connectionTimeout = 5;
    int readTimeout = 5;
    int writeTimeout = 5;
    
    bool compress = true;
    bool decompress = true;
    bool keepAlive = true;
    bool followRedirects = true;

    struct SslConfig {
        bool verifyServerCertificate = true;
        std::string caCertPath;
        std::string clientCertPath;
        std::string clientKeyPath;
    } ssl;

    struct ProxyConfig {
        std::string host;
        int port = 0;
        std::string username;
        std::string password;
    } proxy;
    
    std::map<std::string, std::string> customHeaders;
};

/// HTTP implementation of the SOAP transport.
class HttpSoapTransport : public SoapTransport
{
public:
    /// Initializes an HttpSoapTransport instance.
    /// @param[in] serviceAddress URL of the SOAP service
    /// @param[in] config HTTP configuration
    HttpSoapTransport(
        const std::string& serviceAddress,
        HttpConfig config = HttpConfig());

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
    void ExtractAddressParts(
        const std::string& serviceAddress);

private:
    std::string scheme_;
    std::string host_;
    int port_;
    std::string path_;
    HttpConfig config_;
    bool logging_ = false;
};

} // namespace soaplib
