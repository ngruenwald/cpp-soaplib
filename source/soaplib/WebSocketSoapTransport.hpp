#pragma once

#include <memory>
#include <string>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <atomic>
#include <httplib.h>
#include <soaplib/SoapTransport.hpp>

namespace soaplib {

/// Configuration for WebSocket transport
struct WsConfig {
    int connectionTimeout = 5;
    int readTimeout = 5;
    
    struct SslConfig {
        bool verifyServerCertificate = true;
        std::string caCertPath;
    } ssl;

    std::map<std::string, std::string> customHeaders;
};

/// WebSocket implementation of the SOAP transport.
class WebSocketSoapTransport : public SoapTransport
{
public:
    /// Initializes a WebSocketSoapTransport instance.
    /// @param[in] wsAddress URL of the SOAP service (ws:// or wss://)
    /// @param[in] config WebSocket configuration
    WebSocketSoapTransport(
        const std::string& wsAddress,
        const WsConfig& config = WsConfig());

    /// Destructs the instance.
    ~WebSocketSoapTransport() override;

    /// Sets a handler for unsolicited responses.
    void SetResponseHandler(ResponseHandler handler) override;

    /// Enables logging of messages.
    void EnableLogging(
        bool enable) override;

    /// Sets the default read timeout.
    void SetReadTimeout(
        int timeoutSeconds) override;

    /// Sends the request data and waits for the response.
    std::unique_ptr<xml::Document> Send(
        const xml::Document& request,
        int timeoutSeconds,
        const std::string& soapAction = "",
        HttpMethod method = HttpMethod::Post) override;

    /// Closes the connection.
    void Close();

private:
    bool connect();

private:
    std::string address_;
    WsConfig config_;
    bool logging_ = false;
    std::unique_ptr<httplib::ws::WebSocketClient> ws_client_;

    ResponseHandler responseHandler_;
    std::thread readThread_;
    std::atomic<bool> running_{false};
    
    // For synchronous Send synchronization
    std::mutex mtx_;
    std::condition_variable cv_;
    std::unique_ptr<xml::Document> currentResponse_;
};

} // namespace soaplib
