// LanguageISOCode
#pragma once

#include <memory>
#include <soaplib/basicTypes.hpp>


namespace soaplib { namespace xml { class Document; } }
namespace soaplib { namespace xml { class Node; } }

namespace country {

struct LanguageISOCode
    : soaplib::SoapBaseType
{
    soaplib::String SLanguageName;

    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& node) const override;
};

void LanguageISOCodeFromXml(
    const soaplib::xml::Node& objNode,
    LanguageISOCode& obj);

LanguageISOCode LanguageISOCodeFromXml(
    const soaplib::xml::Node& objNode);

std::unique_ptr<soaplib::SoapBaseType> LanguageISOCodePtrFromXml(
    const soaplib::xml::Node& objNode);

void LanguageISOCodeToXml(
    const LanguageISOCode& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode);

} // namespace country
