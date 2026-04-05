// ListOfContinentsByCodeResponse

#include "ListOfContinentsByCodeResponse.hpp"

#include <soaplib/ParseHelper.hpp>

#include "ArrayOftContinent.hpp"

namespace country {

void ListOfContinentsByCodeResponseFromXml(
    const soaplib::xml::Node& objNode,
    ListOfContinentsByCodeResponse& obj)
{
    obj.ListOfContinentsByCodeResult = soaplib::GetMandatory<ArrayOftContinent>(objNode, "ListOfContinentsByCodeResult", ArrayOftContinentFromXml);
}

ListOfContinentsByCodeResponse ListOfContinentsByCodeResponseFromXml(
    const soaplib::xml::Node& objNode)
{
    ListOfContinentsByCodeResponse obj;
    ListOfContinentsByCodeResponseFromXml(objNode, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> ListOfContinentsByCodeResponsePtrFromXml(
    const soaplib::xml::Node& objNode)
{
    auto obj = std::make_unique<ListOfContinentsByCodeResponse>();
    ListOfContinentsByCodeResponseFromXml(objNode, *obj.get());
    return obj;
}

static void _ListOfContinentsByCodeResponseToXml(
    const ListOfContinentsByCodeResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& objNode)
{
    {
        auto pn = soaplib::AddChild(doc, objNode, "ListOfContinentsByCodeResult", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        ArrayOftContinentToXml(obj.ListOfContinentsByCodeResult, doc, pn, false);
    }
}

void ListOfContinentsByCodeResponseToXml(
    const ListOfContinentsByCodeResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode)
{
    if (createNode)
    {
        auto objNode = soaplib::AddChild(doc, parentNode, "ListOfContinentsByCodeResponse", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        _ListOfContinentsByCodeResponseToXml(obj, doc, objNode);
    }
    else
    {
        _ListOfContinentsByCodeResponseToXml(obj, doc, parentNode);
    }
}

void ListOfContinentsByCodeResponse::ToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& node) const
{
    soaplib::SetAnyTypeAttribute(doc, node, "ListOfContinentsByCodeResponse", "http://www.oorsprong.org/websamples.countryinfo", "tns");
    _ListOfContinentsByCodeResponseToXml(*this, doc, node);
}

} // namespace country
