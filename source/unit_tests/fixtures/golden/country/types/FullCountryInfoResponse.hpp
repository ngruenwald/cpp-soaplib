// FullCountryInfoResponse
#pragma once

#include <memory>
#include <soaplib/basicTypes.hpp>

#include "tCountryInfo.hpp"

namespace soaplib { namespace xml { class Document; } }
namespace soaplib { namespace xml { class Node; } }

namespace country {

struct FullCountryInfoResponse
    : soaplib::SoapBaseType
{
    ::country::tCountryInfo FullCountryInfoResult;

    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& node) const override;
};

void FullCountryInfoResponseFromXml(
    const soaplib::xml::Node& objNode,
    FullCountryInfoResponse& obj);

FullCountryInfoResponse FullCountryInfoResponseFromXml(
    const soaplib::xml::Node& objNode);

std::unique_ptr<soaplib::SoapBaseType> FullCountryInfoResponsePtrFromXml(
    const soaplib::xml::Node& objNode);

void FullCountryInfoResponseToXml(
    const FullCountryInfoResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode);

} // namespace country
