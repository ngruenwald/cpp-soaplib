#include <soaplib/WebSocketSoapTransport.hpp>
#include <soaplib/soapException.hpp>
#include <soaplib/xml/xml.hpp>
#include <iostream>

namespace soaplib {

WebSocketSoapTransport::WebSocketSoapTransport(
    const std::string& wsAddress,
    int timeoutSeconds)
    : address_(wsAddress)
    , timeout_(timeoutSeconds)
{
}

WebSocketSoapTransport::~WebSocketSoapTransport()
{
    if (ws_client_) {
        ws_client_->close();
    }
}

void WebSocketSoapTransport::EnableLogging(bool enable)
{
    logging_ = enable;
}

void WebSocketSoapTransport::SetReadTimeout(int timeoutSeconds)
{
    timeout_ = timeoutSeconds;
    if (ws_client_) {
        ws_client_->set_read_timeout(timeoutSeconds, 0);
    }
}

bool WebSocketSoapTransport::connect()
{
    if (ws_client_ && ws_client_->is_valid() && ws_client_->is_open()) {
        return true;
    }

    ws_client_ = std::make_unique<httplib::ws::WebSocketClient>(address_);
    ws_client_->set_read_timeout(timeout_, 0);
    
    if (!ws_client_->connect()) {
        ws_client_.reset();
        return false;
    }

    return true;
}

std::unique_ptr<xml::Document> WebSocketSoapTransport::Send(
    const xml::Document& request,
    int /*timeoutSeconds*/,
    const std::string& /*soapAction*/,
    HttpMethod /*method*/)
{
    if (!connect()) {
        throw SoapException("WebSocket connection failed to " + address_);
    }

    std::string content = request.Serialize("UTF-8", false);
    if (logging_) {
        std::cout << "WS Send: " << content << std::endl;
    }

    if (!ws_client_->send(content)) {
        ws_client_.reset();
        throw SoapException("WebSocket send failed");
    }

    std::string response_msg;
    auto res = ws_client_->read(response_msg);

    if (res == httplib::ws::ReadResult::Fail) {
        ws_client_.reset();
        throw SoapException("WebSocket read failed or timed out");
    }

    if (logging_) {
        std::cout << "WS Recv: " << response_msg << std::endl;
    }

    auto doc = xml::Document::ParseMemory(response_msg.c_str(), response_msg.length());

    // Check for SOAP Fault
    try {
        auto root = doc->GetRootNode();
        auto body = root.GetChild("Body");
        auto faults = body.GetChildren("Fault");
        if (!faults.empty()) {
            SoapFault fault;
            SoapFaultFromXml(faults[0], fault);
            throw SoapFaultException(fault);
        }
    } catch (const SoapFaultException&) {
        throw;
    } catch (...) {
    }

    return doc;
}

void WebSocketSoapTransport::Close()
{
    if (ws_client_) {
        ws_client_->close();
        ws_client_.reset();
    }
}

} // namespace soaplib
