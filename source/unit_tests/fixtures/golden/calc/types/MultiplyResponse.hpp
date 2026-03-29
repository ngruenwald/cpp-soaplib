// MultiplyResponse
#pragma once

#include <memory>
#include <soaplib/basicTypes.hpp>


namespace soaplib { namespace xml { class Document; } }
namespace soaplib { namespace xml { class Node; } }

namespace calc {

struct MultiplyResponse
    : soaplib::SoapBaseType
{
    soaplib::Int32 MultiplyResult;

    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& node) const override;
};

void MultiplyResponseFromXml(
    const soaplib::xml::Node& objNode,
    MultiplyResponse& obj);

MultiplyResponse MultiplyResponseFromXml(
    const soaplib::xml::Node& objNode);

std::unique_ptr<soaplib::SoapBaseType> MultiplyResponsePtrFromXml(
    const soaplib::xml::Node& objNode);

void MultiplyResponseToXml(
    const MultiplyResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode);

} // namespace calc
