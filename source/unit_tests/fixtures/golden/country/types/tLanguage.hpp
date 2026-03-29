// tLanguage
#pragma once

#include <memory>
#include <soaplib/basicTypes.hpp>


namespace soaplib { namespace xml { class Document; } }
namespace soaplib { namespace xml { class Node; } }

namespace country {

struct tLanguage
    : soaplib::SoapBaseType
{
    soaplib::String SISOCode;
    soaplib::String SName;

    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& node) const override;
};

void tLanguageFromXml(
    const soaplib::xml::Node& objNode,
    tLanguage& obj);

tLanguage tLanguageFromXml(
    const soaplib::xml::Node& objNode);

std::unique_ptr<soaplib::SoapBaseType> tLanguagePtrFromXml(
    const soaplib::xml::Node& objNode);

void tLanguageToXml(
    const tLanguage& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode);

} // namespace country
