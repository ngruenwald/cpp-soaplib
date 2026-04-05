// AddResponse

#include "AddResponse.hpp"

#include <soaplib/ParseHelper.hpp>


namespace calc {

void AddResponseFromXml(
    const soaplib::xml::Node& objNode,
    AddResponse& obj)
{
    obj.AddResult = soaplib::GetMandatory<soaplib::Int32>(objNode, "AddResult", Int32FromXml);
}

AddResponse AddResponseFromXml(
    const soaplib::xml::Node& objNode)
{
    AddResponse obj;
    AddResponseFromXml(objNode, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> AddResponsePtrFromXml(
    const soaplib::xml::Node& objNode)
{
    auto obj = std::make_unique<AddResponse>();
    AddResponseFromXml(objNode, *obj.get());
    return obj;
}

static void _AddResponseToXml(
    const AddResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& objNode)
{
    {
        auto pn = soaplib::AddChild(doc, objNode, "AddResult", "http://tempuri.org/", "");
        Int32ToXml(pn, obj.AddResult);
    }
}

void AddResponseToXml(
    const AddResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode)
{
    if (createNode)
    {
        auto objNode = soaplib::AddChild(doc, parentNode, "AddResponse", "http://tempuri.org/", "");
        _AddResponseToXml(obj, doc, objNode);
    }
    else
    {
        _AddResponseToXml(obj, doc, parentNode);
    }
}

void AddResponse::ToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& node) const
{
    soaplib::SetAnyTypeAttribute(doc, node, "AddResponse", "http://tempuri.org/", "");
    _AddResponseToXml(*this, doc, node);
}

} // namespace calc
