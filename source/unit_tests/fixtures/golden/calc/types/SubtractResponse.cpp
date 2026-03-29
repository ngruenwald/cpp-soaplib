// SubtractResponse

#include "SubtractResponse.hpp"

#include <soaplib/parseHelper.hpp>


namespace calc {

void SubtractResponseFromXml(
    const soaplib::xml::Node& objNode,
    SubtractResponse& obj)
{
    obj.SubtractResult = soaplib::getMandatory<soaplib::Int32>(objNode, "SubtractResult", Int32FromXml);
}

SubtractResponse SubtractResponseFromXml(
    const soaplib::xml::Node& objNode)
{
    SubtractResponse obj;
    SubtractResponseFromXml(objNode, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> SubtractResponsePtrFromXml(
    const soaplib::xml::Node& objNode)
{
    auto obj = std::make_unique<SubtractResponse>();
    SubtractResponseFromXml(objNode, *obj.get());
    return obj;
}

static void _SubtractResponseToXml(
    const SubtractResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& objNode)
{
    {
        auto pn = soaplib::addChild(doc, objNode, "SubtractResult", "http://tempuri.org/", "");
        Int32ToXml(pn, obj.SubtractResult);
    }
}

void SubtractResponseToXml(
    const SubtractResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode)
{
    if (createNode)
    {
        auto objNode = soaplib::addChild(doc, parentNode, "SubtractResponse", "http://tempuri.org/", "");
        _SubtractResponseToXml(obj, doc, objNode);
    }
    else
    {
        _SubtractResponseToXml(obj, doc, parentNode);
    }
}

void SubtractResponse::ToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& node) const
{
    soaplib::setAnyTypeAttribute(doc, node, "SubtractResponse", "http://tempuri.org/", "");
    _SubtractResponseToXml(*this, doc, node);
}

} // namespace calc
