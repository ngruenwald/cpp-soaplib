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
    Close();
}

void WebSocketSoapTransport::EnableLogging(bool enable)
{
    logging_ = enable;
}

void WebSocketSoapTransport::SetReadTimeout(int timeoutSeconds)
{
    timeout_ = timeoutSeconds;
}

void WebSocketSoapTransport::SetResponseHandler(ResponseHandler handler)
{
    responseHandler_ = handler;
}

bool WebSocketSoapTransport::connect()
{
    if (ws_client_ && ws_client_->is_valid() && ws_client_->is_open()) {
        return true;
    }

    running_ = false;
    if (readThread_.joinable()) readThread_.join();

    ws_client_ = std::make_unique<httplib::ws::WebSocketClient>(address_);
    ws_client_->set_read_timeout(timeout_, 0);
    
    if (!ws_client_->connect()) {
        ws_client_.reset();
        return false;
    }

    running_ = true;
    readThread_ = std::thread([this]() {
        std::string msg;
        while (running_ && ws_client_ && ws_client_->is_open()) {
            auto res = ws_client_->read(msg);
            if (res != httplib::ws::ReadResult::Fail) {
                if (logging_) {
                    std::cout << "WS Async Recv: " << msg << std::endl;
                }
                
                try {
                    auto doc = xml::Document::ParseMemory(msg.c_str(), msg.length());
                    
                    std::lock_guard<std::mutex> lock(mtx_);
                    if (!currentResponse_) {
                        currentResponse_ = std::move(doc);
                        cv_.notify_one();
                    } else if (responseHandler_) {
                        responseHandler_(std::move(doc));
                    }
                } catch (...) {}
            } else {
                break;
            }
        }
        running_ = false;
    });

    return true;
}

std::unique_ptr<xml::Document> WebSocketSoapTransport::Send(
    const xml::Document& request,
    int timeoutSeconds,
    const std::string& /*soapAction*/,
    HttpMethod /*method*/)
{
    if (!connect()) {
        throw SoapException("WebSocket connection failed to " + address_);
    }

    {
        std::lock_guard<std::mutex> lock(mtx_);
        currentResponse_.reset();
    }

    std::string content = request.Serialize("UTF-8", false);
    if (logging_) {
        std::cout << "WS Send: " << content << std::endl;
    }

    if (!ws_client_->send(content)) {
        Close();
        throw SoapException("WebSocket send failed");
    }

    std::unique_lock<std::mutex> lock(mtx_);
    if (cv_.wait_for(lock, std::chrono::seconds(timeoutSeconds), [this]{ return (bool)currentResponse_ || !running_; })) {
        if (!currentResponse_) throw SoapException("WebSocket connection closed while waiting for response");
        return std::move(currentResponse_);
    }

    throw SoapException("WebSocket read timed out");
}

void WebSocketSoapTransport::Close()
{
    running_ = false;
    if (ws_client_) {
        ws_client_->close();
    }
    if (readThread_.joinable()) {
        readThread_.join();
    }
    ws_client_.reset();
}

} // namespace soaplib
