#include "SoapBase.hpp"
#include "ParseHelper.hpp"
#include "SoapException.hpp"
#include "types/Uuid.hpp"

#include <libxml/tree.h>

namespace soaplib {

void SoapBase::SetSoapVersion(SoapVersion version)
{
    version_ = version;
}

void SoapBase::SetRole(const std::string& role)
{
    role_ = role;
}

void SoapBase::EnableHeader(bool enable)
{
    enableHeader_ = enable;
}

void SoapBase::RegisterUnderstoodHeader(const std::string& name, const std::string& ns) {
    understoodHeaders_.insert({name, ns});
}

void SoapBase::ValidateHeaders(const xml::Node& envelope) const {
    xml::Node header;
    try {
        header = envelope.GetChild("Header");
    } catch (...) {
        return; // No header, nothing to validate
    }

    const std::string& soapNs = (version_ == SoapVersion::Soap11) ? Soap11Namespace : Soap12Namespace;
    const std::string roleAttr = (version_ == SoapVersion::Soap11) ? "actor" : "role";

    for (const auto& hNode : header.GetChildren(nullptr)) {
        // Check if header is intended for us
        std::string hRole = hNode.GetStringProp(roleAttr.c_str(), soapNs);
        if (hRole.empty()) {
            try { hRole = hNode.GetStringProp(roleAttr.c_str()); } catch(...) {}
        }

        // Default role is 'next' (1.2) or empty (1.1)
        bool isForUs = hRole.empty() || hRole == role_ || 
                       (version_ == SoapVersion::Soap12 && hRole == "http://www.w3.org/2003/05/soap-envelope/role/next") ||
                       (version_ == SoapVersion::Soap11 && hRole == "http://schemas.xmlsoap.org/soap/actor/next");

        if (!isForUs) continue;

        std::string mustUnderstand = hNode.GetStringProp("mustUnderstand", soapNs);
        if (mustUnderstand.empty()) {
            // Check without namespace as some services might not prefix it correctly
            try {
                mustUnderstand = hNode.GetStringProp("mustUnderstand");
            } catch (...) {}
        }

        bool mandatory = (mustUnderstand == "1" || mustUnderstand == "true");
        if (mandatory) {
            std::string name = hNode.GetName();
            std::string ns = "";
            if (hNode.GetXmlNode()->ns && hNode.GetXmlNode()->ns->href) {
                ns = (const char*)hNode.GetXmlNode()->ns->href;
            }

            if (understoodHeaders_.count({name, ns}) == 0 && understoodHeaders_.count({name, ""}) == 0) {
                SoapFault fault;
                fault.Code = FaultCode::MustUnderstand;
                fault.AddReason("Header not understood: " + name);
                throw SoapFaultException(fault);
            }
        }
    }
}

void SoapBase::SetHeaderAttribute(
    xml::Node& headerNode,
    const std::string& name,
    const std::string& value)
{
    // Relay is only for SOAP 1.2
    if (name == "relay" && version_ == SoapVersion::Soap11) {
        return;
    }

    // Role (1.2) vs Actor (1.1)
    std::string attrName = name;
    if (name == "role" && version_ == SoapVersion::Soap11) {
        attrName = "actor";
    }

    headerNode.SetProp(("s:" + attrName).c_str(), value.c_str());
}

xml::Node SoapBase::CreateEnvelope(
    xml::Document& doc,
    const std::string& soapAction,
    const std::string& serviceAddress)
{
    ResetPrefix();

    auto envelope = doc.CreateRootNode("Envelope");

    const std::string& soapNs = (version_ == SoapVersion::Soap11) ? Soap11Namespace : Soap12Namespace;

    AddNamespace(doc, envelope, "http://tempuri.org/", "t");
    AddNamespace(doc, envelope, "http://www.w3.org/2005/08/addressing", "a");
    AddNamespace(doc, envelope, "http://www.w3.org/2001/XMLSchema-instance", "i");
    AddNamespace(doc, envelope, soapNs, "s");

    //
    // header
    //

    if (enableHeader_)
    {
        auto header = AddChild(doc, envelope, "Header", "s");

        if (!soapAction.empty())
        {
            auto headerAction = AddChild(doc, header, "Action", "a");
            SetHeaderAttribute(headerAction, "mustUnderstand", "1");
            headerAction.SetVal(soapAction);
        }

        auto urn = "urn:uuid:" + Uuid::GenerateString();
        auto messageID = AddChild(doc, header, "MessageID", "a");
        messageID.SetVal(urn);

        auto replyTo = AddChild(doc, header, "ReplyTo", "a");
        auto replyToAddress = AddChild(doc, replyTo, "Address", "a");
        replyToAddress.SetVal(AddrNamespace + "/anonymous");

        if (!serviceAddress.empty())
        {
            auto to = AddChild(doc, header, "To", "a");
            SetHeaderAttribute(to, "mustUnderstand", "1");
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

std::string SoapBase::Ns(
    const std::string& ns,
    const std::string& name)
{
    return ns + ":" + name;
}

} // namespace soaplib
