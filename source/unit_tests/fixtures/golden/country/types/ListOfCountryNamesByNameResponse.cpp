// ListOfCountryNamesByNameResponse

#include "ListOfCountryNamesByNameResponse.hpp"

#include <soaplib/parseHelper.hpp>

#include "ArrayOftCountryCodeAndName.hpp"

namespace country {

void ListOfCountryNamesByNameResponseFromXml(
    const soaplib::xml::Node& objNode,
    ListOfCountryNamesByNameResponse& obj)
{
    obj.ListOfCountryNamesByNameResult = soaplib::getMandatory<ArrayOftCountryCodeAndName>(objNode, "ListOfCountryNamesByNameResult", ArrayOftCountryCodeAndNameFromXml);
}

ListOfCountryNamesByNameResponse ListOfCountryNamesByNameResponseFromXml(
    const soaplib::xml::Node& objNode)
{
    ListOfCountryNamesByNameResponse obj;
    ListOfCountryNamesByNameResponseFromXml(objNode, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> ListOfCountryNamesByNameResponsePtrFromXml(
    const soaplib::xml::Node& objNode)
{
    auto obj = std::make_unique<ListOfCountryNamesByNameResponse>();
    ListOfCountryNamesByNameResponseFromXml(objNode, *obj.get());
    return obj;
}

static void _ListOfCountryNamesByNameResponseToXml(
    const ListOfCountryNamesByNameResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& objNode)
{
    {
        auto pn = soaplib::addChild(doc, objNode, "ListOfCountryNamesByNameResult", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        ArrayOftCountryCodeAndNameToXml(obj.ListOfCountryNamesByNameResult, doc, pn, false);
    }
}

void ListOfCountryNamesByNameResponseToXml(
    const ListOfCountryNamesByNameResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode)
{
    if (createNode)
    {
        auto objNode = soaplib::addChild(doc, parentNode, "ListOfCountryNamesByNameResponse", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        _ListOfCountryNamesByNameResponseToXml(obj, doc, objNode);
    }
    else
    {
        _ListOfCountryNamesByNameResponseToXml(obj, doc, parentNode);
    }
}

void ListOfCountryNamesByNameResponse::ToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& node) const
{
    soaplib::setAnyTypeAttribute(doc, node, "ListOfCountryNamesByNameResponse", "http://www.oorsprong.org/websamples.countryinfo", "tns");
    _ListOfCountryNamesByNameResponseToXml(*this, doc, node);
}

} // namespace country
