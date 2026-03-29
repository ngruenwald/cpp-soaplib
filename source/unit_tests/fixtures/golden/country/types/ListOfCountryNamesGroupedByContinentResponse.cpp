// ListOfCountryNamesGroupedByContinentResponse

#include "ListOfCountryNamesGroupedByContinentResponse.hpp"

#include <soaplib/parseHelper.hpp>

#include "ArrayOftCountryCodeAndNameGroupedByContinent.hpp"

namespace country {

void ListOfCountryNamesGroupedByContinentResponseFromXml(
    const soaplib::xml::Node& objNode,
    ListOfCountryNamesGroupedByContinentResponse& obj)
{
    obj.ListOfCountryNamesGroupedByContinentResult = soaplib::getMandatory<ArrayOftCountryCodeAndNameGroupedByContinent>(objNode, "ListOfCountryNamesGroupedByContinentResult", ArrayOftCountryCodeAndNameGroupedByContinentFromXml);
}

ListOfCountryNamesGroupedByContinentResponse ListOfCountryNamesGroupedByContinentResponseFromXml(
    const soaplib::xml::Node& objNode)
{
    ListOfCountryNamesGroupedByContinentResponse obj;
    ListOfCountryNamesGroupedByContinentResponseFromXml(objNode, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> ListOfCountryNamesGroupedByContinentResponsePtrFromXml(
    const soaplib::xml::Node& objNode)
{
    auto obj = std::make_unique<ListOfCountryNamesGroupedByContinentResponse>();
    ListOfCountryNamesGroupedByContinentResponseFromXml(objNode, *obj.get());
    return obj;
}

static void _ListOfCountryNamesGroupedByContinentResponseToXml(
    const ListOfCountryNamesGroupedByContinentResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& objNode)
{
    {
        auto pn = soaplib::addChild(doc, objNode, "ListOfCountryNamesGroupedByContinentResult", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        ArrayOftCountryCodeAndNameGroupedByContinentToXml(obj.ListOfCountryNamesGroupedByContinentResult, doc, pn, false);
    }
}

void ListOfCountryNamesGroupedByContinentResponseToXml(
    const ListOfCountryNamesGroupedByContinentResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode)
{
    if (createNode)
    {
        auto objNode = soaplib::addChild(doc, parentNode, "ListOfCountryNamesGroupedByContinentResponse", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        _ListOfCountryNamesGroupedByContinentResponseToXml(obj, doc, objNode);
    }
    else
    {
        _ListOfCountryNamesGroupedByContinentResponseToXml(obj, doc, parentNode);
    }
}

void ListOfCountryNamesGroupedByContinentResponse::ToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& node) const
{
    soaplib::setAnyTypeAttribute(doc, node, "ListOfCountryNamesGroupedByContinentResponse", "http://www.oorsprong.org/websamples.countryinfo", "tns");
    _ListOfCountryNamesGroupedByContinentResponseToXml(*this, doc, node);
}

} // namespace country
