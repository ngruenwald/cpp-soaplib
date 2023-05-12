#include "soapClient.hpp"
#include "soapException.hpp"

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <sstream>

#include <cpp-httplib/httplib.h>

namespace soaplib {

SoapClient::SoapClient(
    const std::string& serviceAddress,
    int timeoutSeconds,
    bool sslVerify)
    : timeout_{timeoutSeconds}
    , sslVerify_{sslVerify}
{
    extractAddressParts(serviceAddress);
}

SoapClient::~SoapClient()
{
}

void SoapClient::EnableLogging(
    bool enable)
{
    logging_ = enable;
}

void SoapClient::SetReadTimeout(
    int timeoutSeconds)
{
    timeout_ = timeoutSeconds;
}

void SoapClient::SetSslVerify(
    bool sslVerify)
{
    sslVerify_ = sslVerify;
}

std::unique_ptr<xml::Document> SoapClient::Send(
    const xml::Document& request)
{
    return Send(request, timeout_);
}

std::unique_ptr<xml::Document> SoapClient::Send(
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

    httplib::Client cli{host_.c_str()};

    cli.set_read_timeout(timeoutSeconds, 0);
    cli.set_compress(true);
    cli.set_decompress(true);
    cli.set_keep_alive(true);

#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
    cli.enable_server_certificate_verification(sslVerify_);
#endif

    auto response = cli.Post(path_.c_str(), content, contentType.c_str());

    if (!response)
    {
        throw SoapException("request failed");
    }

    if (response->status != 200)
    {
        throw SoapException("request was not successful: " + std::to_string(response->status));
    }

    if (logging_)
    {
        std::cout << response->body << std::endl << std::flush;
    }

    return std::make_unique<xml::Document>(
        response->body.c_str(),
        response->body.length()
    );
}

void SoapClient::extractAddressParts(
    const std::string& serviceAddress)
{
    auto idxHost = serviceAddress.find("://");
    idxHost = idxHost == std::string::npos ? 0 : idxHost + 3;

    auto idxPath = serviceAddress.find("/", idxHost);

    if (idxPath != std::string::npos)
    {
        host_ = serviceAddress.substr(0, idxPath);
        path_ = serviceAddress.substr(idxPath);
    }
    else
    {
        host_ = serviceAddress;
    }
}

} // namespace soaplib
