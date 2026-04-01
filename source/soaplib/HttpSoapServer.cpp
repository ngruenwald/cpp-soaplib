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
    svr_.Post(path_, [this](const httplib::Request& req, httplib::Response& res) {
        try
        {
            auto doc = xml::Document::ParseMemory(req.body.c_str(), req.body.size());
            auto responseDoc = service_.HandleRequest(*doc);
            
            if (responseDoc)
            {
                // SOAP 1.2 specifies that Faults should be returned with a 500 status code
                // by the HTTP binding, BUT our parser needs to see it. 
                // Let's check if the response is a Fault.
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
    });
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
