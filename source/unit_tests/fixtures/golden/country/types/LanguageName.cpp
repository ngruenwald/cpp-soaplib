// LanguageName

#include "LanguageName.hpp"

#include <soaplib/parseHelper.hpp>


namespace country {

void LanguageNameFromXml(
    const soaplib::xml::Node& objNode,
    LanguageName& obj)
{
    obj.SISOCode = soaplib::getMandatory<soaplib::String>(objNode, "sISOCode", StringFromXml);
}

LanguageName LanguageNameFromXml(
    const soaplib::xml::Node& objNode)
{
    LanguageName obj;
    LanguageNameFromXml(objNode, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> LanguageNamePtrFromXml(
    const soaplib::xml::Node& objNode)
{
    auto obj = std::make_unique<LanguageName>();
    LanguageNameFromXml(objNode, *obj.get());
    return obj;
}

static void _LanguageNameToXml(
    const LanguageName& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& objNode)
{
    {
        auto pn = soaplib::addChild(doc, objNode, "sISOCode", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        StringToXml(pn, obj.SISOCode);
    }
}

void LanguageNameToXml(
    const LanguageName& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode)
{
    if (createNode)
    {
        auto objNode = soaplib::addChild(doc, parentNode, "LanguageName", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        _LanguageNameToXml(obj, doc, objNode);
    }
    else
    {
        _LanguageNameToXml(obj, doc, parentNode);
    }
}

void LanguageName::ToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& node) const
{
    soaplib::setAnyTypeAttribute(doc, node, "LanguageName", "http://www.oorsprong.org/websamples.countryinfo", "tns");
    _LanguageNameToXml(*this, doc, node);
}

} // namespace country
