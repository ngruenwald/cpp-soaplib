#include <soaplib/HttpSoapServer.hpp>
#include <soaplib/SoapServer.hpp>
#include <soaplib/xml/xml.hpp>
#include <iostream>

namespace soaplib {

HttpSoapServer::HttpSoapServer(
    SoapServer& service,
    const std::string& path,
    const HttpServerConfig& config)
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
    svr_->set_payload_max_length(config_.payloadMaxLength);
    svr_->set_keep_alive_timeout(config_.keepAliveTimeoutSeconds);
    svr_->set_read_timeout(config_.readTimeoutSeconds, 0);
    svr_->set_write_timeout(config_.writeTimeoutSeconds, 0);

    auto handler = [this](const httplib::Request& req, httplib::Response& res) {
        try
        {
            std::unique_ptr<xml::Document> doc;
            if (req.method == "POST") {
                doc = xml::Document::ParseMemory(req.body.c_str(), req.body.size());
            } else {
                // For GET, we create an empty document
                doc = std::make_unique<xml::Document>();
                doc->CreateRootNode("Envelope").AddChild("Body");
            }

            auto responseDoc = service_.HandleRequest(*doc);
            
            if (responseDoc)
            {
                bool isFault = false;
                try {
                    isFault = !responseDoc->GetRootNode().GetChild("Body").GetChildren("Fault").empty();
                } catch (...) {}

                res.status = isFault ? 500 : 200;
                res.set_content(responseDoc->Serialize(), "application/soap+xml");
            }
            else
            {
                res.status = 500;
                res.set_content("Internal Server Error: No response generated", "text/plain");
            }
        }
        catch (const std::exception& e)
        {
            std::cerr << "SOAP Error: " << e.what() << std::endl;
            res.status = 500;
            res.set_content(std::string("Internal Server Error: ") + e.what(), "text/plain");
        }
        catch (...)
        {
            std::cerr << "SOAP Error: Unknown exception" << std::endl;
            res.status = 500;
            res.set_content("Internal Server Error: Unknown exception", "text/plain");
        }
    };

    svr_->Post(path_, handler);
    svr_->Get(path_, handler);
}

HttpSoapServer::~HttpSoapServer()
{
    Stop();
}

void HttpSoapServer::Listen(
    const std::string& host,
    int port)
{
    svr_->listen(host.c_str(), port);
}

void HttpSoapServer::Stop()
{
    if (svr_) svr_->stop();
}

bool HttpSoapServer::IsRunning() const
{
    return svr_ && svr_->is_running();
}

} // namespace soaplib
