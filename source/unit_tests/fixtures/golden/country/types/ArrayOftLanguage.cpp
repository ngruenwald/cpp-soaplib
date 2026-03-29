// ArrayOftLanguage

#include "ArrayOftLanguage.hpp"

#include <soaplib/parseHelper.hpp>

#include "tLanguage.hpp"

namespace country {

void ArrayOftLanguageFromXml(
    const soaplib::xml::Node& objNode,
    ArrayOftLanguage& obj)
{
    obj.TLanguage = soaplib::getMultiple<tLanguage>(objNode, "tLanguage", tLanguageFromXml);
}

ArrayOftLanguage ArrayOftLanguageFromXml(
    const soaplib::xml::Node& objNode)
{
    ArrayOftLanguage obj;
    ArrayOftLanguageFromXml(objNode, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> ArrayOftLanguagePtrFromXml(
    const soaplib::xml::Node& objNode)
{
    auto obj = std::make_unique<ArrayOftLanguage>();
    ArrayOftLanguageFromXml(objNode, *obj.get());
    return obj;
}

static void _ArrayOftLanguageToXml(
    const ArrayOftLanguage& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& objNode)
{
    for (const auto& entry : obj.TLanguage)
    {
        auto pn = soaplib::addChild(doc, objNode, "tLanguage", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        tLanguageToXml(entry, doc, pn, false);
    }
}

void ArrayOftLanguageToXml(
    const ArrayOftLanguage& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode)
{
    if (createNode)
    {
        auto objNode = soaplib::addChild(doc, parentNode, "ArrayOftLanguage", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        _ArrayOftLanguageToXml(obj, doc, objNode);
    }
    else
    {
        _ArrayOftLanguageToXml(obj, doc, parentNode);
    }
}

void ArrayOftLanguage::ToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& node) const
{
    soaplib::setAnyTypeAttribute(doc, node, "ArrayOftLanguage", "http://www.oorsprong.org/websamples.countryinfo", "tns");
    _ArrayOftLanguageToXml(*this, doc, node);
}

} // namespace country
