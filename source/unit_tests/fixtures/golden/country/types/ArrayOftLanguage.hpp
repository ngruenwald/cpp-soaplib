// ArrayOftLanguage
#pragma once

#include <memory>
#include <soaplib/BasicTypes.hpp>

#include "tLanguage.hpp"

namespace soaplib { namespace xml { class Document; } }
namespace soaplib { namespace xml { class Node; } }

namespace country {

struct ArrayOftLanguage
    : soaplib::SoapBaseType
{
    std::vector<::country::tLanguage> TLanguage;

    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& node) const override;
};

void ArrayOftLanguageFromXml(
    const soaplib::xml::Node& objNode,
    ArrayOftLanguage& obj);

ArrayOftLanguage ArrayOftLanguageFromXml(
    const soaplib::xml::Node& objNode);

std::unique_ptr<soaplib::SoapBaseType> ArrayOftLanguagePtrFromXml(
    const soaplib::xml::Node& objNode);

void ArrayOftLanguageToXml(
    const ArrayOftLanguage& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode);

} // namespace country
