// ListOfCountryNamesByName

#include "ListOfCountryNamesByName.hpp"

#include <soaplib/ParseHelper.hpp>


namespace country {

void ListOfCountryNamesByNameFromXml(
    const soaplib::xml::Node& objNode,
    ListOfCountryNamesByName& obj)
{
}

ListOfCountryNamesByName ListOfCountryNamesByNameFromXml(
    const soaplib::xml::Node& objNode)
{
    ListOfCountryNamesByName obj;
    ListOfCountryNamesByNameFromXml(objNode, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> ListOfCountryNamesByNamePtrFromXml(
    const soaplib::xml::Node& objNode)
{
    auto obj = std::make_unique<ListOfCountryNamesByName>();
    ListOfCountryNamesByNameFromXml(objNode, *obj.get());
    return obj;
}

static void _ListOfCountryNamesByNameToXml(
    const ListOfCountryNamesByName& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& objNode)
{
}

void ListOfCountryNamesByNameToXml(
    const ListOfCountryNamesByName& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode)
{
    if (createNode)
    {
        auto objNode = soaplib::AddChild(doc, parentNode, "ListOfCountryNamesByName", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        _ListOfCountryNamesByNameToXml(obj, doc, objNode);
    }
    else
    {
        _ListOfCountryNamesByNameToXml(obj, doc, parentNode);
    }
}

void ListOfCountryNamesByName::ToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& node) const
{
    soaplib::SetAnyTypeAttribute(doc, node, "ListOfCountryNamesByName", "http://www.oorsprong.org/websamples.countryinfo", "tns");
    _ListOfCountryNamesByNameToXml(*this, doc, node);
}

} // namespace country
