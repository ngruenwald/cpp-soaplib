// DivideResponse

#include "DivideResponse.hpp"

#include <soaplib/ParseHelper.hpp>


namespace calc {

void DivideResponseFromXml(
    const soaplib::xml::Node& objNode,
    DivideResponse& obj)
{
    obj.DivideResult = soaplib::GetMandatory<soaplib::Int32>(objNode, "DivideResult", Int32FromXml);
}

DivideResponse DivideResponseFromXml(
    const soaplib::xml::Node& objNode)
{
    DivideResponse obj;
    DivideResponseFromXml(objNode, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> DivideResponsePtrFromXml(
    const soaplib::xml::Node& objNode)
{
    auto obj = std::make_unique<DivideResponse>();
    DivideResponseFromXml(objNode, *obj.get());
    return obj;
}

static void _DivideResponseToXml(
    const DivideResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& objNode)
{
    {
        auto pn = soaplib::AddChild(doc, objNode, "DivideResult", "http://tempuri.org/", "");
        Int32ToXml(pn, obj.DivideResult);
    }
}

void DivideResponseToXml(
    const DivideResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode)
{
    if (createNode)
    {
        auto objNode = soaplib::AddChild(doc, parentNode, "DivideResponse", "http://tempuri.org/", "");
        _DivideResponseToXml(obj, doc, objNode);
    }
    else
    {
        _DivideResponseToXml(obj, doc, parentNode);
    }
}

void DivideResponse::ToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& node) const
{
    soaplib::SetAnyTypeAttribute(doc, node, "DivideResponse", "http://tempuri.org/", "");
    _DivideResponseToXml(*this, doc, node);
}

} // namespace calc
