// tContinent
#pragma once

#include <memory>
#include <soaplib/BasicTypes.hpp>


namespace soaplib { namespace xml { class Document; } }
namespace soaplib { namespace xml { class Node; } }

namespace country {

struct tContinent
    : soaplib::SoapBaseType
{
    soaplib::String SCode;
    soaplib::String SName;

    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& node) const override;
};

void tContinentFromXml(
    const soaplib::xml::Node& objNode,
    tContinent& obj);

tContinent tContinentFromXml(
    const soaplib::xml::Node& objNode);

std::unique_ptr<soaplib::SoapBaseType> tContinentPtrFromXml(
    const soaplib::xml::Node& objNode);

void tContinentToXml(
    const tContinent& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode);

} // namespace country
