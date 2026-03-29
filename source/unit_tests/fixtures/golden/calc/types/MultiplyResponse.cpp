// MultiplyResponse

#include "MultiplyResponse.hpp"

#include <soaplib/parseHelper.hpp>


namespace calc {

void MultiplyResponseFromXml(
    const soaplib::xml::Node& objNode,
    MultiplyResponse& obj)
{
    obj.MultiplyResult = soaplib::getMandatory<soaplib::Int32>(objNode, "MultiplyResult", Int32FromXml);
}

MultiplyResponse MultiplyResponseFromXml(
    const soaplib::xml::Node& objNode)
{
    MultiplyResponse obj;
    MultiplyResponseFromXml(objNode, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> MultiplyResponsePtrFromXml(
    const soaplib::xml::Node& objNode)
{
    auto obj = std::make_unique<MultiplyResponse>();
    MultiplyResponseFromXml(objNode, *obj.get());
    return obj;
}

static void _MultiplyResponseToXml(
    const MultiplyResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& objNode)
{
    {
        auto pn = soaplib::addChild(doc, objNode, "MultiplyResult", "http://tempuri.org/", "");
        Int32ToXml(pn, obj.MultiplyResult);
    }
}

void MultiplyResponseToXml(
    const MultiplyResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode)
{
    if (createNode)
    {
        auto objNode = soaplib::addChild(doc, parentNode, "MultiplyResponse", "http://tempuri.org/", "");
        _MultiplyResponseToXml(obj, doc, objNode);
    }
    else
    {
        _MultiplyResponseToXml(obj, doc, parentNode);
    }
}

void MultiplyResponse::ToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& node) const
{
    soaplib::setAnyTypeAttribute(doc, node, "MultiplyResponse", "http://tempuri.org/", "");
    _MultiplyResponseToXml(*this, doc, node);
}

} // namespace calc
