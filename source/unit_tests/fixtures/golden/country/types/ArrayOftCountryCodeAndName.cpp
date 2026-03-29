// ArrayOftCountryCodeAndName

#include "ArrayOftCountryCodeAndName.hpp"

#include <soaplib/parseHelper.hpp>

#include "tCountryCodeAndName.hpp"

namespace country {

void ArrayOftCountryCodeAndNameFromXml(
    const soaplib::xml::Node& objNode,
    ArrayOftCountryCodeAndName& obj)
{
    obj.TCountryCodeAndName = soaplib::getMultiple<tCountryCodeAndName>(objNode, "tCountryCodeAndName", tCountryCodeAndNameFromXml);
}

ArrayOftCountryCodeAndName ArrayOftCountryCodeAndNameFromXml(
    const soaplib::xml::Node& objNode)
{
    ArrayOftCountryCodeAndName obj;
    ArrayOftCountryCodeAndNameFromXml(objNode, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> ArrayOftCountryCodeAndNamePtrFromXml(
    const soaplib::xml::Node& objNode)
{
    auto obj = std::make_unique<ArrayOftCountryCodeAndName>();
    ArrayOftCountryCodeAndNameFromXml(objNode, *obj.get());
    return obj;
}

static void _ArrayOftCountryCodeAndNameToXml(
    const ArrayOftCountryCodeAndName& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& objNode)
{
    for (const auto& entry : obj.TCountryCodeAndName)
    {
        auto pn = soaplib::addChild(doc, objNode, "tCountryCodeAndName", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        tCountryCodeAndNameToXml(entry, doc, pn, false);
    }
}

void ArrayOftCountryCodeAndNameToXml(
    const ArrayOftCountryCodeAndName& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode)
{
    if (createNode)
    {
        auto objNode = soaplib::addChild(doc, parentNode, "ArrayOftCountryCodeAndName", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        _ArrayOftCountryCodeAndNameToXml(obj, doc, objNode);
    }
    else
    {
        _ArrayOftCountryCodeAndNameToXml(obj, doc, parentNode);
    }
}

void ArrayOftCountryCodeAndName::ToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& node) const
{
    soaplib::setAnyTypeAttribute(doc, node, "ArrayOftCountryCodeAndName", "http://www.oorsprong.org/websamples.countryinfo", "tns");
    _ArrayOftCountryCodeAndNameToXml(*this, doc, node);
}

} // namespace country
