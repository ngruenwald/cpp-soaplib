#include <soaplib/HttpSoapTransport.hpp>
#include <soaplib/soapException.hpp>

#include <iostream>
#include <httplib.h>

namespace soaplib {

HttpSoapTransport::HttpSoapTransport(
    const std::string& serviceAddress,
    const HttpConfig& config)
    : config_(config)
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
    config_.readTimeout = timeoutSeconds;
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

    std::string url = scheme_ + "://" + host_;
    if ((scheme_ == "http" && port_ != 80) || (scheme_ == "https" && port_ != 443)) {
        url += ":" + std::to_string(port_);
    }
    
    std::unique_ptr<httplib::Client> cli;
    if (scheme_ == "https" && !config_.ssl.clientCertPath.empty() && !config_.ssl.clientKeyPath.empty()) {
        cli = std::make_unique<httplib::Client>(url, config_.ssl.clientCertPath, config_.ssl.clientKeyPath);
    } else {
        cli = std::make_unique<httplib::Client>(url);
    }
    
    // Apply config
    cli->set_connection_timeout(config_.connectionTimeout, 0);
    cli->set_read_timeout(timeoutSeconds > 0 ? timeoutSeconds : config_.readTimeout, 0);
    cli->set_write_timeout(config_.writeTimeout, 0);
    
    cli->set_compress(config_.compress);
    cli->set_decompress(config_.decompress);
    cli->set_keep_alive(config_.keepAlive);
    cli->set_follow_location(config_.followRedirects);

#ifdef CPPHTTPLIB_SSL_ENABLED
    // SSL Config
    if (scheme_ == "https") {
        cli->enable_server_certificate_verification(config_.ssl.verifyServerCertificate);
        if (!config_.ssl.caCertPath.empty()) {
            cli->set_ca_cert_path(config_.ssl.caCertPath.c_str());
        }
    }
#endif

    if (!config_.proxy.host.empty()) {
        cli->set_proxy(config_.proxy.host.c_str(), config_.proxy.port);
        if (!config_.proxy.username.empty()) {
            cli->set_proxy_basic_auth(config_.proxy.username.c_str(), config_.proxy.password.c_str());
        }
    }

    httplib::Headers headers;
    headers.emplace("Content-Type", contentType);
    headers.emplace("Accept", "application/soap+xml, text/xml");
    
    for (const auto& h : config_.customHeaders) {
        headers.emplace(h.first, h.second);
    }
    
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
        response = cli->Post(path_.c_str(), headers, content, contentType.c_str());
    } else {
        if (logging_)
        {
            std::cout << "GET " << path_ << std::endl << std::flush;
        }
        response = cli->Get(path_.c_str(), headers);
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
    if (idxHost != std::string::npos) {
        scheme_ = serviceAddress.substr(0, idxHost);
        idxHost += 3;
    } else {
        scheme_ = "http";
        idxHost = 0;
    }

    auto idxPort = serviceAddress.find(":", idxHost);
    auto idxPath = serviceAddress.find("/", idxHost);

    if (idxPort != std::string::npos && (idxPath == std::string::npos || idxPort < idxPath))
    {
        host_ = serviceAddress.substr(idxHost, idxPort - idxHost);
        idxPort = idxPort + 1;
        auto p = serviceAddress.substr(idxPort, idxPath == std::string::npos ? std::string::npos : idxPath - idxPort);
        port_ = std::stoi(p);
    }
    else
    {
        host_ = serviceAddress.substr(idxHost, idxPath == std::string::npos ? std::string::npos : idxPath - idxHost);
        port_ = (scheme_ == "https") ? 443 : 80;
    }

    if (idxPath != std::string::npos) {
        path_ = serviceAddress.substr(idxPath);
    } else {
        path_ = "/";
    }
}

} // namespace soaplib
