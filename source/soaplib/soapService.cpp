#include "soapService.hpp"
#include "soapClient.hpp"
#include "soapException.hpp"

#include <sstream>

#include "xml/xml.hpp"

#include "types/uuid.hpp"

namespace soaplib {

SoapService::SoapService(
    const std::string& serviceAddress,
    const std::string& serviceNamespace)
    : client_(serviceAddress)
    , address_(serviceAddress)
    , namespace_(serviceNamespace)
{
    ParseServiceAddress(serviceAddress);
}

void SoapService::EnableHeader(
    bool enable)
{
    enableHeader_ = enable;
}

void SoapService::EnableLogging(
    bool enable)
{
    client_.EnableLogging(enable);
}

void SoapService::SetRequestTimeout(
    int timeoutSeconds)
{
    client_.SetReadTimeout(timeoutSeconds);
}

std::string SoapService::ns(
    const std::string& ns,
    const std::string& name)
{
    return ns + ":" + name;
}

std::unique_ptr<xml::Document> SoapService::Call(
    const xml::Document& request)
{
    return client_.Send(request);
}

std::unique_ptr<xml::Document> SoapService::Call(
    const xml::Document& request,
    int timeoutSeconds)
{
    return client_.Send(request, timeoutSeconds);
}

xml::Node SoapService::CreateEnvelope(
    xml::Document& doc,
    const std::string& soapAction)
{
    auto envelope = doc.CreateRootNode("Envelope");

    AddNamespace(doc, envelope, "http://tempuri.org/", "t");
    AddNamespace(doc, envelope, "http://www.w3.org/2005/08/addressing", "a");
    AddNamespace(doc, envelope, "http://www.w3.org/2001/XMLSchema-instance", "i");
    AddNamespace(doc, envelope, "http://www.w3.org/2003/05/soap-envelope", "s");

    //
    // header
    //

    if (enableHeader_)
    {
        auto header = AddChild(doc, envelope, "Header", "s");

        if (!soapAction.empty())
        {
            auto headerAction = AddChild(doc, header, "Action", "a");
            headerAction.SetProp("s:mustUnderstand", 1);
            headerAction.SetVal(soapAction);
        }

        auto urn = "urn:uuid:" + uuid::generate_string();
        auto messageID = AddChild(doc, header, "MessageID", "a");
        messageID.SetVal(urn);

        auto replyTo = AddChild(doc, header, "ReplyTo", "a");
        auto replyToAddress = AddChild(doc, replyTo, "Address", "a");
        replyToAddress.SetVal(AddrNamespace + "/anonymous");

        auto to = AddChild(doc, header, "To", "a");
        to.SetProp("s:mustUnderstand", 1);
        to.SetVal(address_);
    }

    //
    // body
    //

    auto body = AddChild(doc, envelope, "Body", "s");

    return body;
}

void SoapService::AddNamespace(
    xml::Document& doc,
    xml::Node& node,
    const std::string& href,
    const std::string& prefix)
{
    auto np = node.GetXmlNode();

    if (href.empty())
    {
        auto ns = xmlSearchNs(doc.GetXmlDoc(), np, BAD_CAST prefix.c_str());
        if (ns)
        {
            xmlSetNs(np, ns);
            return;
        }
    }

    xmlSetNs(np, xmlNewNs(np, BAD_CAST href.c_str(), BAD_CAST prefix.c_str()));
}

xml::Node SoapService::AddChild(
    xml::Document& doc,
    xml::Node& parentNode,
    const std::string& name,
    const std::string& prefix)
{
    auto node = parentNode.AddChild(name.c_str());
    AddNamespace(doc, node, {}, prefix);
    return node;
}

void SoapService::ParseServiceAddress(
    const std::string& serviceAddress)
{
    auto idxHost = serviceAddress.find("://");
    if (idxHost == std::string::npos)
    {
        throw soaplib::SoapException("invalid service address");
    }
    idxHost += 3;

    auto idxPath = serviceAddress.find('/', idxHost);
    if (idxPath == std::string::npos)
    {
        throw soaplib::SoapException("invalid service address");
    }

    auto idxService = serviceAddress.rfind('/');
//    if (idxService == idxPath)
//    {
//        throw soaplib::SoapException("invalid service address");
//    }
    idxService += 1;

    auto idxExt = std::string::size_type{};
    for (const auto& ext : {".svc", ".asmx", ".wso"})
    {
        idxExt = serviceAddress.find(ext, idxService);

        if (idxExt != std::string::npos)
        {
            break;
        }
    }
    if (idxExt == std::string::npos)
    {
        idxExt = serviceAddress.length();

        //throw soaplib::SoapException("invalid service address");
    }

    host_ = serviceAddress.substr(idxHost, idxPath - idxHost);
    path_ = serviceAddress.substr(idxPath, idxService - idxPath);
    service_ = serviceAddress.substr(idxService, idxExt - idxService);
}

} // namespace soaplib
