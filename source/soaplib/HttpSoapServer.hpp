#pragma once

#include <string>
#include <memory>
#include <httplib.h>

namespace soaplib {

class SoapServer;

/// A ready-to-use HTTP server for SOAP services.
/// Decouples transport from SOAP logic.
class HttpSoapServer
{
public:
    /// Initializes the HTTP SOAP server.
    /// @param[in] service The SOAP server implementation
    /// @param[in] path The URL path for the SOAP service
    HttpSoapServer(
        SoapServer& service,
        const std::string& path = "/");

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
    httplib::Server svr_;
};

} // namespace soaplib
