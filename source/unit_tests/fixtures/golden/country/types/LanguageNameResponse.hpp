// LanguageNameResponse
#pragma once

#include <memory>
#include <soaplib/basicTypes.hpp>


namespace soaplib { namespace xml { class Document; } }
namespace soaplib { namespace xml { class Node; } }

namespace country {

struct LanguageNameResponse
    : soaplib::SoapBaseType
{
    soaplib::String LanguageNameResult;

    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& node) const override;
};

void LanguageNameResponseFromXml(
    const soaplib::xml::Node& objNode,
    LanguageNameResponse& obj);

LanguageNameResponse LanguageNameResponseFromXml(
    const soaplib::xml::Node& objNode);

std::unique_ptr<soaplib::SoapBaseType> LanguageNameResponsePtrFromXml(
    const soaplib::xml::Node& objNode);

void LanguageNameResponseToXml(
    const LanguageNameResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode);

} // namespace country
