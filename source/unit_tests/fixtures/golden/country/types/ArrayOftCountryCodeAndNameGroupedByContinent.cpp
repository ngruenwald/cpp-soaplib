// ArrayOftCountryCodeAndNameGroupedByContinent

#include "ArrayOftCountryCodeAndNameGroupedByContinent.hpp"

#include <soaplib/ParseHelper.hpp>

#include "tCountryCodeAndNameGroupedByContinent.hpp"

namespace country {

void ArrayOftCountryCodeAndNameGroupedByContinentFromXml(
    const soaplib::xml::Node& objNode,
    ArrayOftCountryCodeAndNameGroupedByContinent& obj)
{
    obj.TCountryCodeAndNameGroupedByContinent = soaplib::GetMultiple<tCountryCodeAndNameGroupedByContinent>(objNode, "tCountryCodeAndNameGroupedByContinent", tCountryCodeAndNameGroupedByContinentFromXml);
}

ArrayOftCountryCodeAndNameGroupedByContinent ArrayOftCountryCodeAndNameGroupedByContinentFromXml(
    const soaplib::xml::Node& objNode)
{
    ArrayOftCountryCodeAndNameGroupedByContinent obj;
    ArrayOftCountryCodeAndNameGroupedByContinentFromXml(objNode, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> ArrayOftCountryCodeAndNameGroupedByContinentPtrFromXml(
    const soaplib::xml::Node& objNode)
{
    auto obj = std::make_unique<ArrayOftCountryCodeAndNameGroupedByContinent>();
    ArrayOftCountryCodeAndNameGroupedByContinentFromXml(objNode, *obj.get());
    return obj;
}

static void _ArrayOftCountryCodeAndNameGroupedByContinentToXml(
    const ArrayOftCountryCodeAndNameGroupedByContinent& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& objNode)
{
    for (const auto& entry : obj.TCountryCodeAndNameGroupedByContinent)
    {
        auto pn = soaplib::AddChild(doc, objNode, "tCountryCodeAndNameGroupedByContinent", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        tCountryCodeAndNameGroupedByContinentToXml(entry, doc, pn, false);
    }
}

void ArrayOftCountryCodeAndNameGroupedByContinentToXml(
    const ArrayOftCountryCodeAndNameGroupedByContinent& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode)
{
    if (createNode)
    {
        auto objNode = soaplib::AddChild(doc, parentNode, "ArrayOftCountryCodeAndNameGroupedByContinent", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        _ArrayOftCountryCodeAndNameGroupedByContinentToXml(obj, doc, objNode);
    }
    else
    {
        _ArrayOftCountryCodeAndNameGroupedByContinentToXml(obj, doc, parentNode);
    }
}

void ArrayOftCountryCodeAndNameGroupedByContinent::ToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& node) const
{
    soaplib::SetAnyTypeAttribute(doc, node, "ArrayOftCountryCodeAndNameGroupedByContinent", "http://www.oorsprong.org/websamples.countryinfo", "tns");
    _ArrayOftCountryCodeAndNameGroupedByContinentToXml(*this, doc, node);
}

} // namespace country
