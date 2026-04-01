#include <soaplib/HttpSoapTransport.hpp>
#include <soaplib/soapException.hpp>

#include <iostream>
#include <httplib.h>

namespace soaplib {

HttpSoapTransport::HttpSoapTransport(
    const std::string& serviceAddress,
    int timeoutSeconds)
    : timeout_(timeoutSeconds)
{
    extractAddressParts(serviceAddress);
}

HttpSoapTransport::~HttpSoapTransport()
{
}

void HttpSoapTransport::EnableLogging(
    bool enable)
{
    logging_ = enable;
}

void HttpSoapTransport::SetReadTimeout(
    int timeoutSeconds)
{
    timeout_ = timeoutSeconds;
}

std::unique_ptr<xml::Document> HttpSoapTransport::Send(
    const xml::Document& request,
    int timeoutSeconds)
{
    constexpr bool prettyXml = false;

    const std::string contentType = "application/soap+xml; charset=utf-8";
    const std::string content = request.Serialize("UTF-8", prettyXml);

    if (logging_)
    {
        std::cout << content << std::endl << std::flush;
    }

    httplib::Client cli(host_.c_str(), port_);
    cli.set_read_timeout(timeoutSeconds, 0);
    cli.set_compress(true);
    cli.set_decompress(true);
    cli.set_keep_alive(true);

    auto response = cli.Post(path_.c_str(), content, contentType.c_str());

    if (!response)
    {
        throw SoapException("request failed");
    }

    if (logging_)
    {
        std::cout << response->body << std::endl << std::flush;
    }

    if (response->body.empty()) {
        if (response->status != 200) {
            throw SoapException("request failed with status " + std::to_string(response->status));
        }
        return nullptr;
    }

    auto doc = xml::Document::ParseMemory(
        response->body.c_str(),
        response->body.length()
    );

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
        throw; // rethrow our structured fault
    } catch (...) {
        // Not a SOAP Fault or malformed XML
    }

    if (response->status != 200)
    {
        throw SoapException("request was not successful: " + std::to_string(response->status) + ". Body: " + response->body);
    }

    return doc;
}

void HttpSoapTransport::extractAddressParts(
    const std::string& serviceAddress)
{
    auto idxHost = serviceAddress.find("://");
    idxHost = idxHost == std::string::npos ? 0 : idxHost + 3;

    auto idxPort = serviceAddress.find(":", idxHost);
    auto idxPath = serviceAddress.find("/", idxHost);

    if (idxPort != std::string::npos)
    {
        host_ = serviceAddress.substr(idxHost, idxPort - idxHost);
        idxPort = idxPort + 1;
        auto p = serviceAddress.substr(idxPort, idxPath - idxPort);
        port_ = std::stoi(p);
    }
    else
    {
        host_ = serviceAddress.substr(idxHost, idxPath - idxHost);
        port_ = 80;
    }

    path_ = serviceAddress.substr(idxPath);
}

} // namespace soaplib
