// CountryFlagResponse
#pragma once

#include <memory>
#include <soaplib/BasicTypes.hpp>


namespace soaplib { namespace xml { class Document; } }
namespace soaplib { namespace xml { class Node; } }

namespace country {

struct CountryFlagResponse
    : soaplib::SoapBaseType
{
    soaplib::String CountryFlagResult;

    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& node) const override;
};

void CountryFlagResponseFromXml(
    const soaplib::xml::Node& objNode,
    CountryFlagResponse& obj);

CountryFlagResponse CountryFlagResponseFromXml(
    const soaplib::xml::Node& objNode);

std::unique_ptr<soaplib::SoapBaseType> CountryFlagResponsePtrFromXml(
    const soaplib::xml::Node& objNode);

void CountryFlagResponseToXml(
    const CountryFlagResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode);

} // namespace country
