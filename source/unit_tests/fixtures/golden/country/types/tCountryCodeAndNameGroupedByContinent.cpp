// tCountryCodeAndNameGroupedByContinent

#include "tCountryCodeAndNameGroupedByContinent.hpp"

#include <soaplib/ParseHelper.hpp>

#include "tContinent.hpp"
#include "ArrayOftCountryCodeAndName.hpp"

namespace country {

void tCountryCodeAndNameGroupedByContinentFromXml(
    const soaplib::xml::Node& objNode,
    tCountryCodeAndNameGroupedByContinent& obj)
{
    obj.Continent = soaplib::GetMandatory<tContinent>(objNode, "Continent", tContinentFromXml);
    obj.CountryCodeAndNames = soaplib::GetMandatory<ArrayOftCountryCodeAndName>(objNode, "CountryCodeAndNames", ArrayOftCountryCodeAndNameFromXml);
}

tCountryCodeAndNameGroupedByContinent tCountryCodeAndNameGroupedByContinentFromXml(
    const soaplib::xml::Node& objNode)
{
    tCountryCodeAndNameGroupedByContinent obj;
    tCountryCodeAndNameGroupedByContinentFromXml(objNode, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> tCountryCodeAndNameGroupedByContinentPtrFromXml(
    const soaplib::xml::Node& objNode)
{
    auto obj = std::make_unique<tCountryCodeAndNameGroupedByContinent>();
    tCountryCodeAndNameGroupedByContinentFromXml(objNode, *obj.get());
    return obj;
}

static void _tCountryCodeAndNameGroupedByContinentToXml(
    const tCountryCodeAndNameGroupedByContinent& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& objNode)
{
    {
        auto pn = soaplib::AddChild(doc, objNode, "Continent", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        tContinentToXml(obj.Continent, doc, pn, false);
    }
    {
        auto pn = soaplib::AddChild(doc, objNode, "CountryCodeAndNames", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        ArrayOftCountryCodeAndNameToXml(obj.CountryCodeAndNames, doc, pn, false);
    }
}

void tCountryCodeAndNameGroupedByContinentToXml(
    const tCountryCodeAndNameGroupedByContinent& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode)
{
    if (createNode)
    {
        auto objNode = soaplib::AddChild(doc, parentNode, "tCountryCodeAndNameGroupedByContinent", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        _tCountryCodeAndNameGroupedByContinentToXml(obj, doc, objNode);
    }
    else
    {
        _tCountryCodeAndNameGroupedByContinentToXml(obj, doc, parentNode);
    }
}

void tCountryCodeAndNameGroupedByContinent::ToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& node) const
{
    soaplib::SetAnyTypeAttribute(doc, node, "tCountryCodeAndNameGroupedByContinent", "http://www.oorsprong.org/websamples.countryinfo", "tns");
    _tCountryCodeAndNameGroupedByContinentToXml(*this, doc, node);
}

} // namespace country
