#include <soaplib/soapService.hpp>
#include <soaplib/HttpSoapTransport.hpp>
#include <soaplib/soapException.hpp>

#include <sstream>
#include <utility>

#include <soaplib/xml/xml.hpp>

namespace soaplib {

SoapService::SoapService(
    std::unique_ptr<SoapTransport> transport,
    const std::string& serviceNamespace)
    : transport_(std::move(transport))
    , namespace_(serviceNamespace)
{
}

SoapService::SoapService(
    const std::string& serviceAddress,
    const std::string& serviceNamespace)
    : transport_(std::make_unique<HttpSoapTransport>(serviceAddress))
    , address_(serviceAddress)
    , namespace_(serviceNamespace)
{
}

SoapService::~SoapService()
{
}

void SoapService::EnableLogging(
    bool enable)
{
    if (transport_) transport_->EnableLogging(enable);
}

void SoapService::SetRequestTimeout(
    int timeoutSeconds)
{
    defaultTimeout_ = timeoutSeconds;
    if (transport_) transport_->SetReadTimeout(timeoutSeconds);
}

std::unique_ptr<xml::Document> SoapService::Call(
    const xml::Document& request)
{
    return Call(request, defaultTimeout_);
}

std::unique_ptr<xml::Document> SoapService::Call(
    const xml::Document& request,
    int timeoutSeconds)
{
    if (!transport_)
    {
        throw SoapException("No transport configured");
    }
    return transport_->Send(request, timeoutSeconds);
}

} // namespace soaplib
