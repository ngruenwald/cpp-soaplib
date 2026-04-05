#pragma once

#include <string>
#include <vector>
#include <memory>
#include <soaplib/xml/Xml.hpp>
#include <soaplib/SoapBase.hpp>

namespace soaplib {

/// Standard SOAP Fault Codes
enum class FaultCode {
    VersionMismatch,
    MustUnderstand,
    DataEncodingUnknown,
    Sender,
    Receiver
};

/// Represents a SOAP 1.2 Subcode
struct SoapSubcode {
    std::string Value; // QName as string
    std::string Prefix;
    std::string Namespace;
    std::shared_ptr<SoapSubcode> Subcode; // Recursive for hierarchical subcodes
};

/// Represents a SOAP Fault Reason text
struct FaultReason {
    std::string Text;
    std::string Language = "en"; // xml:lang
};

/// Represents a SOAP Fault
struct SoapFault {
    FaultCode Code = FaultCode::Receiver;
    std::shared_ptr<SoapSubcode> Subcode; // SOAP 1.2 only
    
    std::vector<FaultReason> Reasons;
    std::string Node; // URI
    std::string Role; // URI
    
    // For now, we store detail as a string or raw XML. 
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
    const SoapFault& fault,
    SoapVersion version = SoapVersion::Soap12);

/// Deserializes a SoapFault from an XML node
void SoapFaultFromXml(
    const xml::Node& faultNode,
    SoapFault& fault,
    SoapVersion version = SoapVersion::Soap12);

/// Helper to parse FaultCode from string
FaultCode FaultCodeFromString(
    const std::string& code);

} // namespace soaplib
