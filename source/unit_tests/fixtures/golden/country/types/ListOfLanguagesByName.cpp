// ListOfLanguagesByName

#include "ListOfLanguagesByName.hpp"

#include <soaplib/parseHelper.hpp>


namespace country {

void ListOfLanguagesByNameFromXml(
    const soaplib::xml::Node& objNode,
    ListOfLanguagesByName& obj)
{
}

ListOfLanguagesByName ListOfLanguagesByNameFromXml(
    const soaplib::xml::Node& objNode)
{
    ListOfLanguagesByName obj;
    ListOfLanguagesByNameFromXml(objNode, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> ListOfLanguagesByNamePtrFromXml(
    const soaplib::xml::Node& objNode)
{
    auto obj = std::make_unique<ListOfLanguagesByName>();
    ListOfLanguagesByNameFromXml(objNode, *obj.get());
    return obj;
}

static void _ListOfLanguagesByNameToXml(
    const ListOfLanguagesByName& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& objNode)
{
}

void ListOfLanguagesByNameToXml(
    const ListOfLanguagesByName& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode)
{
    if (createNode)
    {
        auto objNode = soaplib::addChild(doc, parentNode, "ListOfLanguagesByName", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        _ListOfLanguagesByNameToXml(obj, doc, objNode);
    }
    else
    {
        _ListOfLanguagesByNameToXml(obj, doc, parentNode);
    }
}

void ListOfLanguagesByName::ToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& node) const
{
    soaplib::setAnyTypeAttribute(doc, node, "ListOfLanguagesByName", "http://www.oorsprong.org/websamples.countryinfo", "tns");
    _ListOfLanguagesByNameToXml(*this, doc, node);
}

} // namespace country
