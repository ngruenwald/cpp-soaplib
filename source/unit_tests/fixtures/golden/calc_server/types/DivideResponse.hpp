// DivideResponse
#pragma once

#include <memory>
#include <soaplib/basicTypes.hpp>


namespace soaplib { namespace xml { class Document; } }
namespace soaplib { namespace xml { class Node; } }

namespace calc {

struct DivideResponse
    : soaplib::SoapBaseType
{
    soaplib::Int32 DivideResult;

    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& node) const override;
};

void DivideResponseFromXml(
    const soaplib::xml::Node& objNode,
    DivideResponse& obj);

DivideResponse DivideResponseFromXml(
    const soaplib::xml::Node& objNode);

std::unique_ptr<soaplib::SoapBaseType> DivideResponsePtrFromXml(
    const soaplib::xml::Node& objNode);

void DivideResponseToXml(
    const DivideResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode);

} // namespace calc
