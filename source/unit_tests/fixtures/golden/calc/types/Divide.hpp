// Divide
#pragma once

#include <memory>
#include <soaplib/BasicTypes.hpp>


namespace soaplib { namespace xml { class Document; } }
namespace soaplib { namespace xml { class Node; } }

namespace calc {

struct Divide
    : soaplib::SoapBaseType
{
    soaplib::Int32 IntA;
    soaplib::Int32 IntB;

    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& node) const override;
};

void DivideFromXml(
    const soaplib::xml::Node& objNode,
    Divide& obj);

Divide DivideFromXml(
    const soaplib::xml::Node& objNode);

std::unique_ptr<soaplib::SoapBaseType> DividePtrFromXml(
    const soaplib::xml::Node& objNode);

void DivideToXml(
    const Divide& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode);

} // namespace calc
