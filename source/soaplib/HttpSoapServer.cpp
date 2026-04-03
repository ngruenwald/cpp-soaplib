#include <soaplib/HttpSoapServer.hpp>
#include <soaplib/SoapServer.hpp>
#include <soaplib/xml/xml.hpp>
#include <iostream>

namespace soaplib {

HttpSoapServer::HttpSoapServer(
    SoapServer& service,
    const std::string& path)
    : service_(service)
    , path_(path)
{
    auto handler = [this](const httplib::Request& req, httplib::Response& res) {
        try
        {
            std::unique_ptr<xml::Document> doc;
            if (req.method == "POST") {
                doc = xml::Document::ParseMemory(req.body.c_str(), req.body.size());
            } else {
                // For GET, we create an empty document or one based on query params
                // For now, just an empty one to trigger the dispatcher
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

    svr_.Post(path_, handler);
    svr_.Get(path_, handler);
}

HttpSoapServer::~HttpSoapServer()
{
    Stop();
}

void HttpSoapServer::Listen(
    const std::string& host,
    int port)
{
    svr_.listen(host.c_str(), port);
}

void HttpSoapServer::Stop()
{
    svr_.stop();
}

bool HttpSoapServer::IsRunning() const
{
    return svr_.is_running();
}

} // namespace soaplib
