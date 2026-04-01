#include <soaplib/WebSocketSoapServer.hpp>
#include <soaplib/SoapServer.hpp>
#include <soaplib/xml/xml.hpp>
#include <iostream>

namespace soaplib {

WebSocketSoapServer::WebSocketSoapServer(
    SoapServer& service,
    const std::string& path)
    : service_(service)
    , path_(path)
{
    svr_.WebSocket(path_, [this](const httplib::Request& /*req*/, httplib::ws::WebSocket& ws) {
        std::string msg;
        while (ws.is_open()) {
            auto res = ws.read(msg);
            if (res != httplib::ws::ReadResult::Fail) {
                auto doc = xml::Document::ParseMemory(msg.c_str(), msg.length());
                auto responseDoc = service_.HandleRequest(*doc);
                if (responseDoc) {
                    ws.send(responseDoc->Serialize("UTF-8", false));
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
    svr_.listen(host.c_str(), port);
}

void WebSocketSoapServer::Stop()
{
    svr_.stop();
}

bool WebSocketSoapServer::IsRunning() const
{
    return svr_.is_running();
}

} // namespace soaplib
