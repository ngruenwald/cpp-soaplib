#pragma once

#include <memory>
#include <string>

#include <soaplib/xml/xml.hpp>
#include <soaplib/SoapBase.hpp>
#include <soaplib/SoapTransport.hpp>

namespace soaplib {

/// Base class for SOAP services.
class SoapService : public SoapBase
{
public:
    /// Creates a SoapService instance using a specific transport.
    /// @param[in] transport The transport implementation to use
    /// @param[in] serviceNamespace XML namespace
    SoapService(
        std::unique_ptr<SoapTransport> transport,
        const std::string& serviceNamespace);

    /// Creates a SoapService instance with default HTTP transport.
    /// @param[in] serviceAddress URL of the SOAP service
    /// @param[in] serviceNamespace XML namespace
    SoapService(
        const std::string& serviceAddress,
        const std::string& serviceNamespace);

    /// Destructs the instance.
    virtual ~SoapService();

    /// Enables logging of the transmitted HTTP messages.
    /// @param[in] enable If true, logging will be enabled.
    void EnableLogging(
        bool enable);

    /// Sets the HTTP request timeout.
    /// @param[in] timeoutSeconds The request timeout in seconds.
    void SetRequestTimeout(
        int timeoutSeconds);

protected:
    // Redundant namespace helpers (TODO: check if generated code uses these)
    std::string SoapNS() const { return "s"; }  ///< Default SOAP namespace prefix
    std::string TempNS() const { return "t"; }  ///< Default TempUri namespace prefix
    std::string AddrNS() const { return "a"; }  ///< Default Addressing namespace prefix
    std::string InstNS() const { return "i"; }  ///< Default Instance namespace prefix
    std::string NS()     const { return "e"; }  ///< Default namespace prefix

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

protected:
    std::unique_ptr<SoapTransport> transport_; ///< SOAP transport instance

    std::string address_;           ///< SOAP service URL (might be empty if custom transport used)
    std::string namespace_;         ///< SOAP service XML namespace

    int defaultTimeout_ = 5;        ///< Default timeout in seconds
};

} // namespace soaplib
