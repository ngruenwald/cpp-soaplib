// ListOfCurrenciesByNameResponse

#include "ListOfCurrenciesByNameResponse.hpp"

#include <soaplib/ParseHelper.hpp>

#include "ArrayOftCurrency.hpp"

namespace country {

void ListOfCurrenciesByNameResponseFromXml(
    const soaplib::xml::Node& objNode,
    ListOfCurrenciesByNameResponse& obj)
{
    obj.ListOfCurrenciesByNameResult = soaplib::GetMandatory<ArrayOftCurrency>(objNode, "ListOfCurrenciesByNameResult", ArrayOftCurrencyFromXml);
}

ListOfCurrenciesByNameResponse ListOfCurrenciesByNameResponseFromXml(
    const soaplib::xml::Node& objNode)
{
    ListOfCurrenciesByNameResponse obj;
    ListOfCurrenciesByNameResponseFromXml(objNode, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> ListOfCurrenciesByNameResponsePtrFromXml(
    const soaplib::xml::Node& objNode)
{
    auto obj = std::make_unique<ListOfCurrenciesByNameResponse>();
    ListOfCurrenciesByNameResponseFromXml(objNode, *obj.get());
    return obj;
}

static void _ListOfCurrenciesByNameResponseToXml(
    const ListOfCurrenciesByNameResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& objNode)
{
    {
        auto pn = soaplib::AddChild(doc, objNode, "ListOfCurrenciesByNameResult", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        ArrayOftCurrencyToXml(obj.ListOfCurrenciesByNameResult, doc, pn, false);
    }
}

void ListOfCurrenciesByNameResponseToXml(
    const ListOfCurrenciesByNameResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode)
{
    if (createNode)
    {
        auto objNode = soaplib::AddChild(doc, parentNode, "ListOfCurrenciesByNameResponse", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        _ListOfCurrenciesByNameResponseToXml(obj, doc, objNode);
    }
    else
    {
        _ListOfCurrenciesByNameResponseToXml(obj, doc, parentNode);
    }
}

void ListOfCurrenciesByNameResponse::ToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& node) const
{
    soaplib::SetAnyTypeAttribute(doc, node, "ListOfCurrenciesByNameResponse", "http://www.oorsprong.org/websamples.countryinfo", "tns");
    _ListOfCurrenciesByNameResponseToXml(*this, doc, node);
}

} // namespace country
