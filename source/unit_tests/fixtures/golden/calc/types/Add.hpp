// Add
#pragma once

#include <memory>
#include <soaplib/BasicTypes.hpp>


namespace soaplib { namespace xml { class Document; } }
namespace soaplib { namespace xml { class Node; } }

namespace calc {

struct Add
    : soaplib::SoapBaseType
{
    soaplib::Int32 IntA;
    soaplib::Int32 IntB;

    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& node) const override;
};

void AddFromXml(
    const soaplib::xml::Node& objNode,
    Add& obj);

Add AddFromXml(
    const soaplib::xml::Node& objNode);

std::unique_ptr<soaplib::SoapBaseType> AddPtrFromXml(
    const soaplib::xml::Node& objNode);

void AddToXml(
    const Add& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode);

} // namespace calc
