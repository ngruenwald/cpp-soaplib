// LanguageName
#pragma once

#include <memory>
#include <soaplib/BasicTypes.hpp>


namespace soaplib { namespace xml { class Document; } }
namespace soaplib { namespace xml { class Node; } }

namespace country {

struct LanguageName
    : soaplib::SoapBaseType
{
    soaplib::String SISOCode;

    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& node) const override;
};

void LanguageNameFromXml(
    const soaplib::xml::Node& objNode,
    LanguageName& obj);

LanguageName LanguageNameFromXml(
    const soaplib::xml::Node& objNode);

std::unique_ptr<soaplib::SoapBaseType> LanguageNamePtrFromXml(
    const soaplib::xml::Node& objNode);

void LanguageNameToXml(
    const LanguageName& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode);

} // namespace country
