#pragma once

#include <memory>
#include <string>

#include <soaplib/xml/xml.hpp>

#include <soaplib/soapClient.hpp>

namespace soaplib {

/// Base class for SOAP services.
class SoapService
{
protected:
    // TODO: check it these are still used
    const std::string SoapNamespace = "http://www.w3.org/2003/05/soap-envelope";    ///< SOAP namespace
    const std::string TempNamespace = "http://tempuri.org/";                        ///< TempUri namespace
    const std::string AddrNamespace = "http://www.w3.org/2005/08/addressing";       ///< Addressing namespace
    const std::string InstNamespace = "http://www.w3.org/2001/XMLSchema-instance";  ///< Instance namespace

public:
    /// Creates a SoapService instance
    /// @param[in] serviceAddress URL of the SOAP service
    /// @param[in] serviceNamespace XML namespace
    /// @throws soap::SoapException
    SoapService(
        const std::string& serviceAddress,
        const std::string& serviceNamespace);

    /// Enables or disables the SOAP header.
    /// @param[in] enable If true, SOAP headers are generated, otherwise not.
    void EnableHeader(
        bool enable);

    /// Enables logging of the transmitted HTTP messages.
    /// @param[in] enable If true, logging will be enabled.
    void EnableLogging(
        bool enable);

    /// Sets the HTTP request timeout.
    /// @param[in] timeoutSeconds The request timeout in seconds.
    void SetRequestTimeout(
        int timeoutSeconds);

    /// Enables or disables SSL certificate verification.
    /// @param[in] sslVerify If true, SSL certificate verification is enabled.
    void SetSslVerify(
        bool sslVerify);

protected:
    // TODO: check if these are still used
    std::string SoapNS() const { return "s"; }  ///< Default SOAP namespace prefix
    std::string TempNS() const { return "t"; }  ///< Default TempUri namespace prefix
    std::string AddrNS() const { return "a"; }  ///< Default Addressing namespace prefix
    std::string InstNS() const { return "i"; }  ///< Default Instance namespace prefix
    std::string NS()     const { return "e"; }  ///< Default namespace prefix

    /// Formats a XML namespace.
    /// @param[in] ns XML namespace
    /// @param[in] name Name prefix
    /// @returns The formatted namespace
    static std::string ns(
        const std::string& ns,
        const std::string& name);

protected:
    /// Executes a method call using the defaut request timeout.
    /// @param[in] request Request data as XML document
    /// @returns Response data as XML document
    std::unique_ptr<xml::Document> Call(
        const xml::Document& request);

    /// Executes a method call using a custom timeout.
    /// @param[in] request Request data as XML document
    /// @param[in] timeoutSeconds Request timeout in seconds
    /// @returns Response data as XML document
    std::unique_ptr<xml::Document> Call(
        const xml::Document& request,
        int timeoutSeconds);

    /// Creates a SOAP Envelope element.
    /// @param[in] doc XML document
    /// @param[in] soapAction The SOAP Action name
    /// @returns The envelope element
    xml::Node CreateEnvelope(
        xml::Document& doc,
        const std::string& soapAction);

    /// Adds a namespace.
    /// @param[in] doc XML document
    /// @param[in] node XML node for which to set the namespace
    /// @param[in] href XML namespace URL
    /// @param[in] prefix XML namespace prefix
    void AddNamespace(
        xml::Document& doc,
        xml::Node& node,
        const std::string& href,
        const std::string& prefix);

    /// Adds a XML child element.
    /// @param[in] doc XML document
    /// @param[in] parentNode XML parent element
    /// @param[in] name Element name
    /// @param[in] prefix XML namespace prefix
    xml::Node AddChild(
        xml::Document& doc,
        xml::Node& parentNode,
        const std::string& name,
        const std::string& prefix);

private:
    void ParseServiceAddress(
        const std::string& serviceAddress);

protected:
    soaplib::SoapClient client_;    ///< SOAP client instance

    std::string address_;           ///< SOAP service URL
    std::string namespace_;         ///< SOAP service XML namespace

    std::string host_;              ///< SOAP service host address
    std::string path_;              ///< SOAP service path
    std::string service_;           ///< SOAP service name

    bool enableHeader_{true};       ///< Indicates if SOAP headers are enabled
};

} // namespace soaplib
