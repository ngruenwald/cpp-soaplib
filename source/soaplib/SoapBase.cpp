#include "SoapBase.hpp"
#include "parseHelper.hpp"
#include "types/uuid.hpp"

#include <libxml/tree.h>

namespace soaplib {

void SoapBase::EnableHeader(bool enable)
{
    enableHeader_ = enable;
}

xml::Node SoapBase::CreateEnvelope(
    xml::Document& doc,
    const std::string& soapAction,
    const std::string& serviceAddress)
{
    resetPrefix();

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

        if (!serviceAddress.empty())
        {
            auto to = AddChild(doc, header, "To", "a");
            to.SetProp("s:mustUnderstand", 1);
            to.SetVal(serviceAddress);
        }
    }

    //
    // body
    //

    auto body = AddChild(doc, envelope, "Body", "s");

    return body;
}

void SoapBase::AddNamespace(
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

xml::Node SoapBase::AddChild(
    xml::Document& doc,
    xml::Node& parentNode,
    const std::string& name,
    const std::string& prefix)
{
    auto node = parentNode.AddChild(name.c_str());
    AddNamespace(doc, node, {}, prefix);
    return node;
}

std::string SoapBase::ns(
    const std::string& ns,
    const std::string& name)
{
    return ns + ":" + name;
}

} // namespace soaplib
