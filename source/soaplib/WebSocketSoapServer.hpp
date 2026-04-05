#pragma once

#include <string>
#include <memory>
#include <httplib.h>

namespace soaplib {

class SoapServer;

/// Configuration for WebSocket server
struct WsServerConfig {
    size_t threadCount = 8;
    int keepAliveTimeoutSeconds = 5;
    int readTimeoutSeconds = 5;
    int writeTimeoutSeconds = 5;

    struct SslConfig {
        std::string certPath;
        std::string keyPath;
    } ssl;
};

/// A ready-to-use WebSocket server for SOAP services.
class WebSocketSoapServer
{
public:
    /// Initializes the WebSocket SOAP server.
    /// @param[in] service The SOAP server implementation
    /// @param[in] path The URL path for the WebSocket service
    /// @param[in] config Optional server configuration
    WebSocketSoapServer(
        SoapServer& service,
        const std::string& path = "/",
        const WsServerConfig& config = WsServerConfig());

    ~WebSocketSoapServer();

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
    WsServerConfig config_;
    std::unique_ptr<httplib::Server> svr_;
};

} // namespace soaplib
