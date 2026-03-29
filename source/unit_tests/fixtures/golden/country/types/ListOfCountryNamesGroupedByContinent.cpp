// ListOfCountryNamesGroupedByContinent

#include "ListOfCountryNamesGroupedByContinent.hpp"

#include <soaplib/parseHelper.hpp>


namespace country {

void ListOfCountryNamesGroupedByContinentFromXml(
    const soaplib::xml::Node& objNode,
    ListOfCountryNamesGroupedByContinent& obj)
{
}

ListOfCountryNamesGroupedByContinent ListOfCountryNamesGroupedByContinentFromXml(
    const soaplib::xml::Node& objNode)
{
    ListOfCountryNamesGroupedByContinent obj;
    ListOfCountryNamesGroupedByContinentFromXml(objNode, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> ListOfCountryNamesGroupedByContinentPtrFromXml(
    const soaplib::xml::Node& objNode)
{
    auto obj = std::make_unique<ListOfCountryNamesGroupedByContinent>();
    ListOfCountryNamesGroupedByContinentFromXml(objNode, *obj.get());
    return obj;
}

static void _ListOfCountryNamesGroupedByContinentToXml(
    const ListOfCountryNamesGroupedByContinent& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& objNode)
{
}

void ListOfCountryNamesGroupedByContinentToXml(
    const ListOfCountryNamesGroupedByContinent& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode)
{
    if (createNode)
    {
        auto objNode = soaplib::addChild(doc, parentNode, "ListOfCountryNamesGroupedByContinent", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        _ListOfCountryNamesGroupedByContinentToXml(obj, doc, objNode);
    }
    else
    {
        _ListOfCountryNamesGroupedByContinentToXml(obj, doc, parentNode);
    }
}

void ListOfCountryNamesGroupedByContinent::ToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& node) const
{
    soaplib::setAnyTypeAttribute(doc, node, "ListOfCountryNamesGroupedByContinent", "http://www.oorsprong.org/websamples.countryinfo", "tns");
    _ListOfCountryNamesGroupedByContinentToXml(*this, doc, node);
}

} // namespace country
