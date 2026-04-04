#pragma once

#include <string>
#include <memory>
#include <httplib.h>

namespace soaplib {

class SoapServer;

/// Configuration for HTTP server
struct HttpServerConfig {
    size_t threadCount = 8;
    size_t payloadMaxLength = 1024 * 1024 * 10; // 10MB
    int keepAliveTimeoutSeconds = 5;
    int readTimeoutSeconds = 5;
    int writeTimeoutSeconds = 5;

    struct SslConfig {
        std::string certPath;
        std::string keyPath;
    } ssl;
};

/// A ready-to-use HTTP server for SOAP services.
/// Decouples transport from SOAP logic.
class HttpSoapServer
{
public:
    /// Initializes the HTTP SOAP server.
    /// @param[in] service The SOAP server implementation
    /// @param[in] path The URL path for the SOAP service
    /// @param[in] config Optional server configuration
    HttpSoapServer(
        SoapServer& service,
        const std::string& path = "/",
        const HttpServerConfig& config = HttpServerConfig());

    /// Destructs the instance.
    ~HttpSoapServer();

    /// Starts listening for requests.
    /// @param[in] host Hostname or IP address
    /// @param[in] port Port number
    void Listen(
        const std::string& host,
        int port);

    /// Stops the server.
    void Stop();

    /// Checks if the server is running.
    bool IsRunning() const;

private:
    SoapServer& service_;
    std::string path_;
    HttpServerConfig config_;
    std::unique_ptr<httplib::Server> svr_;
};

} // namespace soaplib
