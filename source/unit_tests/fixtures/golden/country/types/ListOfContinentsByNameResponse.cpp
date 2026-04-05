// ListOfContinentsByNameResponse

#include "ListOfContinentsByNameResponse.hpp"

#include <soaplib/ParseHelper.hpp>

#include "ArrayOftContinent.hpp"

namespace country {

void ListOfContinentsByNameResponseFromXml(
    const soaplib::xml::Node& objNode,
    ListOfContinentsByNameResponse& obj)
{
    obj.ListOfContinentsByNameResult = soaplib::GetMandatory<ArrayOftContinent>(objNode, "ListOfContinentsByNameResult", ArrayOftContinentFromXml);
}

ListOfContinentsByNameResponse ListOfContinentsByNameResponseFromXml(
    const soaplib::xml::Node& objNode)
{
    ListOfContinentsByNameResponse obj;
    ListOfContinentsByNameResponseFromXml(objNode, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> ListOfContinentsByNameResponsePtrFromXml(
    const soaplib::xml::Node& objNode)
{
    auto obj = std::make_unique<ListOfContinentsByNameResponse>();
    ListOfContinentsByNameResponseFromXml(objNode, *obj.get());
    return obj;
}

static void _ListOfContinentsByNameResponseToXml(
    const ListOfContinentsByNameResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& objNode)
{
    {
        auto pn = soaplib::AddChild(doc, objNode, "ListOfContinentsByNameResult", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        ArrayOftContinentToXml(obj.ListOfContinentsByNameResult, doc, pn, false);
    }
}

void ListOfContinentsByNameResponseToXml(
    const ListOfContinentsByNameResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode)
{
    if (createNode)
    {
        auto objNode = soaplib::AddChild(doc, parentNode, "ListOfContinentsByNameResponse", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        _ListOfContinentsByNameResponseToXml(obj, doc, objNode);
    }
    else
    {
        _ListOfContinentsByNameResponseToXml(obj, doc, parentNode);
    }
}

void ListOfContinentsByNameResponse::ToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& node) const
{
    soaplib::SetAnyTypeAttribute(doc, node, "ListOfContinentsByNameResponse", "http://www.oorsprong.org/websamples.countryinfo", "tns");
    _ListOfContinentsByNameResponseToXml(*this, doc, node);
}

} // namespace country
