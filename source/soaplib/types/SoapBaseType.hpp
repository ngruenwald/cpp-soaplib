#pragma once

namespace soaplib {

namespace xml { class Document; }
namespace xml { class Node; }

/// Base type for all SOAP types
struct SoapBaseType
{
    /// Destructor
    virtual ~SoapBaseType() = default;

    /// Append type to "any" node
    virtual void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& anyNode) const = 0;
};

} // namespace soaplib
