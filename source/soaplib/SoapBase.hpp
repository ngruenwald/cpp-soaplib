#pragma once

#include <string>
#include <memory>
#include <set>
#include <soaplib/xml/Xml.hpp>

namespace soaplib {

/// Supported SOAP Versions
enum class SoapVersion {
    Soap11,
    Soap12
};

/// Base class for SOAP components (client and server).
class SoapBase {
public:
    virtual ~SoapBase() = default;

    /// Sets the SOAP version.
    void SetSoapVersion(SoapVersion version);

    /// Sets the SOAP role/actor URI.
    void SetRole(const std::string& role);

    /// Enables or disables the SOAP header.
    /// @param[in] enable If true, SOAP headers are generated, otherwise not.
    void EnableHeader(
        bool enable);

    /// Registers a header name as "understood".
    /// @param[in] name The element name of the header
    /// @param[in] ns Optional namespace URI
    void RegisterUnderstoodHeader(
        const std::string& name,
        const std::string& ns = "");

protected:
    /// Validates that all 'mustUnderstand' headers are recognized.
    /// @param[in] envelope The SOAP envelope node
    /// @throws SoapFaultException if an unknown mandatory header is found.
    void ValidateHeaders(
        const xml::Node& envelope) const;

    /// Creates a SOAP Envelope element.
    /// @param[in] doc XML document
    /// @param[in] soapAction The SOAP Action name
    /// @param[in] serviceAddress Optional service address for the 'To' header
    /// @returns The body element of the envelope
    xml::Node CreateEnvelope(
        xml::Document& doc,
        const std::string& soapAction,
        const std::string& serviceAddress = "");

    /// Adds a namespace to a node.
    void AddNamespace(
        xml::Document& doc,
        xml::Node& node,
        const std::string& href,
        const std::string& prefix);

    /// Adds a XML child element with a namespace prefix.
    xml::Node AddChild(
        xml::Document& doc,
        xml::Node& parentNode,
        const std::string& name,
        const std::string& prefix);

    /// Sets a SOAP header attribute (version-aware).
    void SetHeaderAttribute(
        xml::Node& headerNode,
        const std::string& name,
        const std::string& value);

    /// Formats a XML namespace.
    static std::string Ns(
        const std::string& ns,
        const std::string& name);

protected:
    // Constants for common namespaces
    const std::string Soap11Namespace = "http://schemas.xmlsoap.org/soap/envelope/";
    const std::string Soap12Namespace = "http://www.w3.org/2003/05/soap-envelope";
    
    const std::string TempNamespace = "http://tempuri.org/";
    const std::string AddrNamespace = "http://www.w3.org/2005/08/addressing";
    const std::string InstNamespace = "http://www.w3.org/2001/XMLSchema-instance";

    SoapVersion version_{SoapVersion::Soap12}; ///< Current SOAP version
    bool enableHeader_{true};       ///< Indicates if SOAP headers are enabled
    std::string role_;              ///< SOAP role (1.2) or actor (1.1) URI
    std::set<std::pair<std::string, std::string>> understoodHeaders_; ///< Registry of understood headers (name, ns)
};

} // namespace soaplib
