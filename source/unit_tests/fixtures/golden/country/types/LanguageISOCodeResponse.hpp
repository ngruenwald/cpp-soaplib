// LanguageISOCodeResponse
#pragma once

#include <memory>
#include <soaplib/basicTypes.hpp>


namespace soaplib { namespace xml { class Document; } }
namespace soaplib { namespace xml { class Node; } }

namespace country {

struct LanguageISOCodeResponse
    : soaplib::SoapBaseType
{
    soaplib::String LanguageISOCodeResult;

    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& node) const override;
};

void LanguageISOCodeResponseFromXml(
    const soaplib::xml::Node& objNode,
    LanguageISOCodeResponse& obj);

LanguageISOCodeResponse LanguageISOCodeResponseFromXml(
    const soaplib::xml::Node& objNode);

std::unique_ptr<soaplib::SoapBaseType> LanguageISOCodeResponsePtrFromXml(
    const soaplib::xml::Node& objNode);

void LanguageISOCodeResponseToXml(
    const LanguageISOCodeResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode);

} // namespace country
