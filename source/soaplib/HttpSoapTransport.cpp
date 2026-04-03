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
    int timeoutSeconds,
    const std::string& soapAction,
    HttpMethod method)
{
    constexpr bool prettyXml = false;

    // Detect version from the document namespace
    SoapVersion version = SoapVersion::Soap12;
    try {
        auto xmlDoc = request.GetXmlDoc();
        if (xmlDoc && xmlDoc->children && xmlDoc->children->ns && xmlDoc->children->ns->href) {
            std::string ns = (const char*)xmlDoc->children->ns->href;
            if (ns.find("http://schemas.xmlsoap.org/soap/envelope/") != std::string::npos) {
                version = SoapVersion::Soap11;
            }
        }
    } catch (...) {}

    std::string contentType;
    if (version == SoapVersion::Soap11) {
        contentType = "text/xml; charset=utf-8";
    } else {
        contentType = "application/soap+xml; charset=utf-8";
        if (!soapAction.empty() && method == HttpMethod::Post) {
            contentType += "; action=\"" + soapAction + "\"";
        }
    }

    httplib::Client cli(host_.c_str(), port_);
    cli.set_read_timeout(timeoutSeconds, 0);
    cli.set_compress(true);
    cli.set_decompress(true);
    cli.set_keep_alive(true);

    httplib::Headers headers;
    headers.emplace("Content-Type", contentType);
    headers.emplace("Accept", "application/soap+xml, text/xml");
    
    if (version == SoapVersion::Soap11 && !soapAction.empty() && method == HttpMethod::Post) {
        headers.emplace("SOAPAction", "\"" + soapAction + "\"");
    }

    httplib::Result response;
    if (method == HttpMethod::Post) {
        const std::string content = request.Serialize("UTF-8", prettyXml);
        if (logging_)
        {
            std::cout << "POST " << path_ << "\n" << content << std::endl << std::flush;
        }
        response = cli.Post(path_.c_str(), headers, content, contentType.c_str());
    } else {
        if (logging_)
        {
            std::cout << "GET " << path_ << std::endl << std::flush;
        }
        response = cli.Get(path_.c_str(), headers);
    }

    if (!response)
    {
        throw SoapException("request failed");
    }

    if (logging_ && method == HttpMethod::Post)
    {
        std::cout << response->body << std::endl << std::flush;
    }

    if (response->body.empty()) {
        if (response->status != 200 && response->status != 202) {
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
        if (faults.empty()) {
            // SOAP 1.1 uses lowercase 'fault' or 'Fault'? Spec says 'Fault' but let's check both
            faults = body.GetChildren("fault"); 
        }
        
        if (!faults.empty()) {
            SoapFault fault;
            SoapFaultFromXml(faults[0], fault, version);
            throw SoapFaultException(fault);
        }
    } catch (const SoapFaultException&) {
        throw;
    } catch (...) {
    }

    if (response->status != 200 && response->status != 202)
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
