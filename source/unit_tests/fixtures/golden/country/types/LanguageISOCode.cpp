// LanguageISOCode

#include "LanguageISOCode.hpp"

#include <soaplib/parseHelper.hpp>


namespace country {

void LanguageISOCodeFromXml(
    const soaplib::xml::Node& objNode,
    LanguageISOCode& obj)
{
    obj.SLanguageName = soaplib::getMandatory<soaplib::String>(objNode, "sLanguageName", StringFromXml);
}

LanguageISOCode LanguageISOCodeFromXml(
    const soaplib::xml::Node& objNode)
{
    LanguageISOCode obj;
    LanguageISOCodeFromXml(objNode, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> LanguageISOCodePtrFromXml(
    const soaplib::xml::Node& objNode)
{
    auto obj = std::make_unique<LanguageISOCode>();
    LanguageISOCodeFromXml(objNode, *obj.get());
    return obj;
}

static void _LanguageISOCodeToXml(
    const LanguageISOCode& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& objNode)
{
    {
        auto pn = soaplib::addChild(doc, objNode, "sLanguageName", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        StringToXml(pn, obj.SLanguageName);
    }
}

void LanguageISOCodeToXml(
    const LanguageISOCode& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode)
{
    if (createNode)
    {
        auto objNode = soaplib::addChild(doc, parentNode, "LanguageISOCode", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        _LanguageISOCodeToXml(obj, doc, objNode);
    }
    else
    {
        _LanguageISOCodeToXml(obj, doc, parentNode);
    }
}

void LanguageISOCode::ToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& node) const
{
    soaplib::setAnyTypeAttribute(doc, node, "LanguageISOCode", "http://www.oorsprong.org/websamples.countryinfo", "tns");
    _LanguageISOCodeToXml(*this, doc, node);
}

} // namespace country
