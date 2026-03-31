#pragma once

#include <string>
#include <memory>
#include <soaplib/xml/xml.hpp>

namespace soaplib {

/// Base class for SOAP components (client and server).
class SoapBase {
public:
    virtual ~SoapBase() = default;

    /// Enables or disables the SOAP header.
    /// @param[in] enable If true, SOAP headers are generated, otherwise not.
    void EnableHeader(
        bool enable);

protected:
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

    /// Formats a XML namespace.
    static std::string ns(
        const std::string& ns,
        const std::string& name);

protected:
    // Constants for common namespaces
    const std::string SoapNamespace = "http://www.w3.org/2003/05/soap-envelope";
    const std::string TempNamespace = "http://tempuri.org/";
    const std::string AddrNamespace = "http://www.w3.org/2005/08/addressing";
    const std::string InstNamespace = "http://www.w3.org/2001/XMLSchema-instance";

    bool enableHeader_{true};       ///< Indicates if SOAP headers are enabled
};

} // namespace soaplib
