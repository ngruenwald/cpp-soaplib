// ListOfContinentsByName

#include "ListOfContinentsByName.hpp"

#include <soaplib/parseHelper.hpp>


namespace country {

void ListOfContinentsByNameFromXml(
    const soaplib::xml::Node& objNode,
    ListOfContinentsByName& obj)
{
}

ListOfContinentsByName ListOfContinentsByNameFromXml(
    const soaplib::xml::Node& objNode)
{
    ListOfContinentsByName obj;
    ListOfContinentsByNameFromXml(objNode, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> ListOfContinentsByNamePtrFromXml(
    const soaplib::xml::Node& objNode)
{
    auto obj = std::make_unique<ListOfContinentsByName>();
    ListOfContinentsByNameFromXml(objNode, *obj.get());
    return obj;
}

static void _ListOfContinentsByNameToXml(
    const ListOfContinentsByName& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& objNode)
{
}

void ListOfContinentsByNameToXml(
    const ListOfContinentsByName& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode)
{
    if (createNode)
    {
        auto objNode = soaplib::addChild(doc, parentNode, "ListOfContinentsByName", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        _ListOfContinentsByNameToXml(obj, doc, objNode);
    }
    else
    {
        _ListOfContinentsByNameToXml(obj, doc, parentNode);
    }
}

void ListOfContinentsByName::ToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& node) const
{
    soaplib::setAnyTypeAttribute(doc, node, "ListOfContinentsByName", "http://www.oorsprong.org/websamples.countryinfo", "tns");
    _ListOfContinentsByNameToXml(*this, doc, node);
}

} // namespace country
