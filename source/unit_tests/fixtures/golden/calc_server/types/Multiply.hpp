// Multiply
#pragma once

#include <memory>
#include <soaplib/BasicTypes.hpp>


namespace soaplib { namespace xml { class Document; } }
namespace soaplib { namespace xml { class Node; } }

namespace calc {

struct Multiply
    : soaplib::SoapBaseType
{
    soaplib::Int32 IntA;
    soaplib::Int32 IntB;

    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& node) const override;
};

void MultiplyFromXml(
    const soaplib::xml::Node& objNode,
    Multiply& obj);

Multiply MultiplyFromXml(
    const soaplib::xml::Node& objNode);

std::unique_ptr<soaplib::SoapBaseType> MultiplyPtrFromXml(
    const soaplib::xml::Node& objNode);

void MultiplyToXml(
    const Multiply& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode);

} // namespace calc
