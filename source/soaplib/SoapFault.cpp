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

static void SubcodeToXml(xml::Node& parent, const std::shared_ptr<SoapSubcode>& sub) {
    if (!sub) return;
    auto subNode = parent.AddChild("Subcode");
    auto valNode = subNode.AddChild("Value");
    
    std::string prefix = sub->Prefix.empty() ? "m" : sub->Prefix;
    if (!sub->Namespace.empty()) {
        // We need a way to add namespace to the document/node properly if not there
        // For now simple prefix:value
        valNode.SetVal(prefix + ":" + sub->Value);
    } else {
        valNode.SetVal(sub->Value);
    }

    if (sub->Subcode) {
        SubcodeToXml(subNode, sub->Subcode);
    }
}

void SoapFaultToXml(
    xml::Document& /*doc*/,
    xml::Node& faultNode,
    const SoapFault& fault,
    SoapVersion version)
{
    if (version == SoapVersion::Soap12) {
        auto code = faultNode.AddChild("Code");
        auto value = code.AddChild("Value");
        value.SetVal("s:" + fault.GetCodeString());

        if (fault.Subcode) {
            SubcodeToXml(code, fault.Subcode);
        }

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
            faultNode.AddChild("Detail").SetVal(fault.Detail);
        }
    } else {
        // SOAP 1.1
        faultNode.AddChild("faultcode").SetVal("s:" + fault.GetCodeString());
        faultNode.AddChild("faultstring").SetVal(fault.Reasons.empty() ? "" : fault.Reasons[0].Text);
        
        if (!fault.Role.empty()) {
            faultNode.AddChild("faultactor").SetVal(fault.Role);
        }

        if (!fault.Detail.empty()) {
            faultNode.AddChild("detail").SetVal(fault.Detail);
        }
    }
}

FaultCode FaultCodeFromString(const std::string& code) {
    auto pos = code.find(':');
    std::string val = (pos == std::string::npos) ? code : code.substr(pos + 1);

    if (val == "VersionMismatch") return FaultCode::VersionMismatch;
    if (val == "MustUnderstand")  return FaultCode::MustUnderstand;
    if (val == "DataEncodingUnknown") return FaultCode::DataEncodingUnknown;
    if (val == "Sender" || val == "Client") return FaultCode::Sender;
    if (val == "Receiver" || val == "Server") return FaultCode::Receiver;
    return FaultCode::Receiver;
}

static std::shared_ptr<SoapSubcode> SubcodeFromXml(const xml::Node& subNode) {
    auto sub = std::make_shared<SoapSubcode>();
    try {
        auto valNode = subNode.GetChild("Value");
        std::string val = valNode.GetStringVal();
        auto pos = val.find(':');
        if (pos != std::string::npos) {
            sub->Prefix = val.substr(0, pos);
            sub->Value = val.substr(pos + 1);
        } else {
            sub->Value = val;
        }
    } catch (...) {}

    try {
        auto nestedSub = subNode.GetChild("Subcode");
        sub->Subcode = SubcodeFromXml(nestedSub);
    } catch (...) {}

    return sub;
}

void SoapFaultFromXml(const xml::Node& faultNode, SoapFault& fault, SoapVersion version) {
    if (version == SoapVersion::Soap12) {
        try {
            auto codeNode = faultNode.GetChild("Code");
            fault.Code = FaultCodeFromString(codeNode.GetChild("Value").GetStringVal());
            
            try {
                auto subNode = codeNode.GetChild("Subcode");
                fault.Subcode = SubcodeFromXml(subNode);
            } catch (...) {}
        } catch (...) {}

        try {
            auto reasonNode = faultNode.GetChild("Reason");
            for (const auto& textNode : reasonNode.GetChildren("Text")) {
                FaultReason r;
                r.Text = textNode.GetStringVal();
                XML_OPTIONAL(r.Language = textNode.GetStringProp("lang", "http://www.w3.org/XML/1998/namespace"));
                fault.Reasons.push_back(r);
            }
        } catch (...) {}

        XML_OPTIONAL(fault.Node = faultNode.GetChild("Node").GetStringVal());
        XML_OPTIONAL(fault.Role = faultNode.GetChild("Role").GetStringVal());
        XML_OPTIONAL(fault.Detail = faultNode.GetChild("Detail").GetStringVal());
    } else {
        // SOAP 1.1
        XML_OPTIONAL(fault.Code = FaultCodeFromString(faultNode.GetChild("faultcode").GetStringVal()));
        try {
            FaultReason r;
            r.Text = faultNode.GetChild("faultstring").GetStringVal();
            fault.Reasons.push_back(r);
        } catch (...) {}
        XML_OPTIONAL(fault.Role = faultNode.GetChild("faultactor").GetStringVal());
        XML_OPTIONAL(fault.Detail = faultNode.GetChild("detail").GetStringVal());
    }
}

} // namespace soaplib
