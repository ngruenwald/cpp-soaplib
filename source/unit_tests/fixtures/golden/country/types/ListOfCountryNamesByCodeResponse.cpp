// ListOfCountryNamesByCodeResponse

#include "ListOfCountryNamesByCodeResponse.hpp"

#include <soaplib/parseHelper.hpp>

#include "ArrayOftCountryCodeAndName.hpp"

namespace country {

void ListOfCountryNamesByCodeResponseFromXml(
    const soaplib::xml::Node& objNode,
    ListOfCountryNamesByCodeResponse& obj)
{
    obj.ListOfCountryNamesByCodeResult = soaplib::getMandatory<ArrayOftCountryCodeAndName>(objNode, "ListOfCountryNamesByCodeResult", ArrayOftCountryCodeAndNameFromXml);
}

ListOfCountryNamesByCodeResponse ListOfCountryNamesByCodeResponseFromXml(
    const soaplib::xml::Node& objNode)
{
    ListOfCountryNamesByCodeResponse obj;
    ListOfCountryNamesByCodeResponseFromXml(objNode, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> ListOfCountryNamesByCodeResponsePtrFromXml(
    const soaplib::xml::Node& objNode)
{
    auto obj = std::make_unique<ListOfCountryNamesByCodeResponse>();
    ListOfCountryNamesByCodeResponseFromXml(objNode, *obj.get());
    return obj;
}

static void _ListOfCountryNamesByCodeResponseToXml(
    const ListOfCountryNamesByCodeResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& objNode)
{
    {
        auto pn = soaplib::addChild(doc, objNode, "ListOfCountryNamesByCodeResult", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        ArrayOftCountryCodeAndNameToXml(obj.ListOfCountryNamesByCodeResult, doc, pn, false);
    }
}

void ListOfCountryNamesByCodeResponseToXml(
    const ListOfCountryNamesByCodeResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode)
{
    if (createNode)
    {
        auto objNode = soaplib::addChild(doc, parentNode, "ListOfCountryNamesByCodeResponse", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        _ListOfCountryNamesByCodeResponseToXml(obj, doc, objNode);
    }
    else
    {
        _ListOfCountryNamesByCodeResponseToXml(obj, doc, parentNode);
    }
}

void ListOfCountryNamesByCodeResponse::ToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& node) const
{
    soaplib::setAnyTypeAttribute(doc, node, "ListOfCountryNamesByCodeResponse", "http://www.oorsprong.org/websamples.countryinfo", "tns");
    _ListOfCountryNamesByCodeResponseToXml(*this, doc, node);
}

} // namespace country
