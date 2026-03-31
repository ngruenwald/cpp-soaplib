// AddResponse
#pragma once

#include <memory>
#include <soaplib/basicTypes.hpp>


namespace soaplib { namespace xml { class Document; } }
namespace soaplib { namespace xml { class Node; } }

namespace calc {

struct AddResponse
    : soaplib::SoapBaseType
{
    soaplib::Int32 AddResult;

    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& node) const override;
};

void AddResponseFromXml(
    const soaplib::xml::Node& objNode,
    AddResponse& obj);

AddResponse AddResponseFromXml(
    const soaplib::xml::Node& objNode);

std::unique_ptr<soaplib::SoapBaseType> AddResponsePtrFromXml(
    const soaplib::xml::Node& objNode);

void AddResponseToXml(
    const AddResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode);

} // namespace calc
