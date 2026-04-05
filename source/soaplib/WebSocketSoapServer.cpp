#include <soaplib/WebSocketSoapServer.hpp>
#include <soaplib/SoapServer.hpp>
#include <soaplib/xml/Xml.hpp>
#include <iostream>

namespace soaplib {

WebSocketSoapServer::WebSocketSoapServer(
    SoapServer& service,
    const std::string& path,
    const WsServerConfig& config)
    : service_(service)
    , path_(path)
    , config_(config)
{
#ifdef CPPHTTPLIB_SSL_ENABLED
    if (!config_.ssl.certPath.empty() && !config_.ssl.keyPath.empty()) {
        svr_ = std::make_unique<httplib::SSLServer>(config_.ssl.certPath.c_str(), config_.ssl.keyPath.c_str());
    } else {
        svr_ = std::make_unique<httplib::Server>();
    }
#else
    svr_ = std::make_unique<httplib::Server>();
#endif

    // Apply config
    svr_->new_task_queue = [this] { return new httplib::ThreadPool(config_.threadCount); };
    svr_->set_keep_alive_timeout(config_.keepAliveTimeoutSeconds);
    svr_->set_read_timeout(config_.readTimeoutSeconds, 0);
    svr_->set_write_timeout(config_.writeTimeoutSeconds, 0);

    svr_->set_mount_point(path_, ""); // Required for WebSocket mount

    svr_->WebSocket(
        path_,
        [this](const httplib::Request& /*req*/, httplib::ws::WebSocket& ws) {
            std::string msg;
            while (ws.is_open()) {
                auto res = ws.read(msg);
                if (res != httplib::ws::ReadResult::Fail) {
                    try {
                        auto doc = xml::Document::ParseMemory(msg.c_str(), msg.length());
                        auto responseDoc = service_.HandleRequest(*doc);
                        if (responseDoc) {
                            ws.send(responseDoc->Serialize());
                        }
                    } catch (const std::exception& e) {
                        std::cerr << "WS Error: " << e.what() << std::endl;
                    }
                } else {
                    break;
                }
            }
        });
}

WebSocketSoapServer::~WebSocketSoapServer()
{
    Stop();
}

void WebSocketSoapServer::Listen(
    const std::string& host,
    int port)
{
    svr_->listen(host.c_str(), port);
}

void WebSocketSoapServer::Stop()
{
    if (svr_) svr_->stop();
}

bool WebSocketSoapServer::IsRunning() const
{
    return svr_ && svr_->is_running();
}

} // namespace soaplib
