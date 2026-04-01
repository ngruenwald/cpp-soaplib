#pragma once

#include <string>
#include <vector>
#include <memory>
#include <soaplib/xml/xml.hpp>

namespace soaplib {

/// Standard SOAP 1.2 Fault Codes
enum class FaultCode {
    VersionMismatch,
    MustUnderstand,
    DataEncodingUnknown,
    Sender,
    Receiver
};

/// Represents a SOAP 1.2 Fault Reason text
struct FaultReason {
    std::string Text;
    std::string Language = "en"; // xml:lang
};

/// Represents a SOAP 1.2 Fault
struct SoapFault {
    FaultCode Code = FaultCode::Receiver;
    std::vector<FaultReason> Reasons;
    std::string Node; // URI
    std::string Role; // URI
    
    // For now, we store detail as a string or raw XML. 
    // In the future, we could use a list of xml::Nodes.
    std::string Detail; 

    /// Helper to add a reason
    void AddReason(const std::string& text, const std::string& lang = "en") {
        Reasons.push_back({text, lang});
    }

    /// Converts the fault to a string representation of the code
    std::string GetCodeString() const;
};

/// Serializes a SoapFault to an XML node
void SoapFaultToXml(
    xml::Document& doc,
    xml::Node& faultNode,
    const SoapFault& fault);

/// Deserializes a SoapFault from an XML node
void SoapFaultFromXml(
    const xml::Node& faultNode,
    SoapFault& fault);

/// Helper to parse FaultCode from string
FaultCode FaultCodeFromString(
    const std::string& code);

} // namespace soaplib
