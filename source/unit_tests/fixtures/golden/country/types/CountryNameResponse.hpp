// CountryNameResponse
#pragma once

#include <memory>
#include <soaplib/basicTypes.hpp>


namespace soaplib { namespace xml { class Document; } }
namespace soaplib { namespace xml { class Node; } }

namespace country {

struct CountryNameResponse
    : soaplib::SoapBaseType
{
    soaplib::String CountryNameResult;

    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& node) const override;
};

void CountryNameResponseFromXml(
    const soaplib::xml::Node& objNode,
    CountryNameResponse& obj);

CountryNameResponse CountryNameResponseFromXml(
    const soaplib::xml::Node& objNode);

std::unique_ptr<soaplib::SoapBaseType> CountryNameResponsePtrFromXml(
    const soaplib::xml::Node& objNode);

void CountryNameResponseToXml(
    const CountryNameResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode);

} // namespace country
