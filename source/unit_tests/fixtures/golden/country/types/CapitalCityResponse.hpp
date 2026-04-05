// CapitalCityResponse
#pragma once

#include <memory>
#include <soaplib/BasicTypes.hpp>


namespace soaplib { namespace xml { class Document; } }
namespace soaplib { namespace xml { class Node; } }

namespace country {

struct CapitalCityResponse
    : soaplib::SoapBaseType
{
    soaplib::String CapitalCityResult;

    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& node) const override;
};

void CapitalCityResponseFromXml(
    const soaplib::xml::Node& objNode,
    CapitalCityResponse& obj);

CapitalCityResponse CapitalCityResponseFromXml(
    const soaplib::xml::Node& objNode);

std::unique_ptr<soaplib::SoapBaseType> CapitalCityResponsePtrFromXml(
    const soaplib::xml::Node& objNode);

void CapitalCityResponseToXml(
    const CapitalCityResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode);

} // namespace country
