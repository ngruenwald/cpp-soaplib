// Subtract
#pragma once

#include <memory>
#include <soaplib/BasicTypes.hpp>


namespace soaplib { namespace xml { class Document; } }
namespace soaplib { namespace xml { class Node; } }

namespace calc {

struct Subtract
    : soaplib::SoapBaseType
{
    soaplib::Int32 IntA;
    soaplib::Int32 IntB;

    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& node) const override;
};

void SubtractFromXml(
    const soaplib::xml::Node& objNode,
    Subtract& obj);

Subtract SubtractFromXml(
    const soaplib::xml::Node& objNode);

std::unique_ptr<soaplib::SoapBaseType> SubtractPtrFromXml(
    const soaplib::xml::Node& objNode);

void SubtractToXml(
    const Subtract& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode);

} // namespace calc
