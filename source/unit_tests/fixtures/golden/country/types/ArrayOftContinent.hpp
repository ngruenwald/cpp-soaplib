// ArrayOftContinent
#pragma once

#include <memory>
#include <soaplib/BasicTypes.hpp>

#include "tContinent.hpp"

namespace soaplib { namespace xml { class Document; } }
namespace soaplib { namespace xml { class Node; } }

namespace country {
struct tContinent;

struct ArrayOftContinent
    : soaplib::SoapBaseType
{
    std::vector<::country::tContinent> TContinent;

    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& node) const override;
};

void ArrayOftContinentFromXml(
    const soaplib::xml::Node& objNode,
    ArrayOftContinent& obj);

ArrayOftContinent ArrayOftContinentFromXml(
    const soaplib::xml::Node& objNode);

std::unique_ptr<soaplib::SoapBaseType> ArrayOftContinentPtrFromXml(
    const soaplib::xml::Node& objNode);

void ArrayOftContinentToXml(
    const ArrayOftContinent& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode);

} // namespace country
