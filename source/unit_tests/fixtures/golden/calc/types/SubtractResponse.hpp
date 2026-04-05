// SubtractResponse
#pragma once

#include <memory>
#include <soaplib/BasicTypes.hpp>


namespace soaplib { namespace xml { class Document; } }
namespace soaplib { namespace xml { class Node; } }

namespace calc {

struct SubtractResponse
    : soaplib::SoapBaseType
{
    soaplib::Int32 SubtractResult;

    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& node) const override;
};

void SubtractResponseFromXml(
    const soaplib::xml::Node& objNode,
    SubtractResponse& obj);

SubtractResponse SubtractResponseFromXml(
    const soaplib::xml::Node& objNode);

std::unique_ptr<soaplib::SoapBaseType> SubtractResponsePtrFromXml(
    const soaplib::xml::Node& objNode);

void SubtractResponseToXml(
    const SubtractResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode);

} // namespace calc
