#include "SoapFault.hpp"

namespace soaplib {

std::string SoapFault::GetCodeString() const {
    switch (Code) {
        case FaultCode::VersionMismatch:   return "VersionMismatch";
        case FaultCode::MustUnderstand:    return "MustUnderstand";
        case FaultCode::DataEncodingUnknown: return "DataEncodingUnknown";
        case FaultCode::Sender:            return "Sender";
        case FaultCode::Receiver:          return "Receiver";
        default:                           return "Receiver";
    }
}

void SoapFaultToXml(
    xml::Document& /*doc*/,
    xml::Node& faultNode,
    const SoapFault& fault)
{
    // Note: SOAP 1.2 elements are NOT prefixed in the Body, 
    // but they must be in the SOAP namespace.
    // The faultNode itself should already be in the SOAP namespace.

    auto code = faultNode.AddChild("Code");
    auto value = code.AddChild("Value");
    value.SetVal("s:" + fault.GetCodeString());

    auto reason = faultNode.AddChild("Reason");
    for (const auto& r : fault.Reasons) {
        auto text = reason.AddChild("Text");
        text.SetProp("xml:lang", r.Language.c_str());
        text.SetVal(r.Text);
    }

    if (!fault.Node.empty()) {
        faultNode.AddChild("Node").SetVal(fault.Node);
    }

    if (!fault.Role.empty()) {
        faultNode.AddChild("Role").SetVal(fault.Role);
    }

    if (!fault.Detail.empty()) {
        // Simple string injection for detail. 
        // In a more advanced impl, we would parse and append nodes.
        faultNode.AddChild("Detail").SetVal(fault.Detail);
    }
}

FaultCode FaultCodeFromString(const std::string& code) {
    auto pos = code.find(':');
    std::string val = (pos == std::string::npos) ? code : code.substr(pos + 1);

    if (val == "VersionMismatch") return FaultCode::VersionMismatch;
    if (val == "MustUnderstand")  return FaultCode::MustUnderstand;
    if (val == "DataEncodingUnknown") return FaultCode::DataEncodingUnknown;
    if (val == "Sender")           return FaultCode::Sender;
    return FaultCode::Receiver;
}

void SoapFaultFromXml(const xml::Node& faultNode, SoapFault& fault) {
    try {
        auto codeNode = faultNode.GetChild("Code");
        fault.Code = FaultCodeFromString(codeNode.GetChild("Value").GetStringVal());
    } catch (...) {}

    try {
        auto reasonNode = faultNode.GetChild("Reason");
        for (const auto& textNode : reasonNode.GetChildren("Text")) {
            FaultReason r;
            r.Text = textNode.GetStringVal();
            XML_OPTIONAL(r.Language = textNode.GetStringProp("xml:lang"));
            fault.Reasons.push_back(r);
        }
    } catch (...) {}

    XML_OPTIONAL(fault.Node = faultNode.GetChild("Node").GetStringVal());
    XML_OPTIONAL(fault.Role = faultNode.GetChild("Role").GetStringVal());
    XML_OPTIONAL(fault.Detail = faultNode.GetChild("Detail").GetStringVal());
}

} // namespace soaplib
