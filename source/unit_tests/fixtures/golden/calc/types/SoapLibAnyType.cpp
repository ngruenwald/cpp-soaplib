// SoapLibAnyType

#include <string>
#include <map>
#include "SoapLibAnyType.hpp"

#include <soaplib/basicTypes.hpp>
#include <soaplib/parseHelper.hpp>

#include "Add.hpp"
#include "AddResponse.hpp"
#include "Subtract.hpp"
#include "SubtractResponse.hpp"
#include "Multiply.hpp"
#include "MultiplyResponse.hpp"
#include "Divide.hpp"
#include "DivideResponse.hpp"

namespace calc {

using namespace ::soaplib;

static std::map<std::string, std::unique_ptr<soaplib::SoapBaseType>(*)(const soaplib::xml::Node&)> typeMap =
{
    { "Add", AddPtrFromXml },
    { "AddResponse", AddResponsePtrFromXml },
    { "Subtract", SubtractPtrFromXml },
    { "SubtractResponse", SubtractResponsePtrFromXml },
    { "Multiply", MultiplyPtrFromXml },
    { "MultiplyResponse", MultiplyResponsePtrFromXml },
    { "Divide", DividePtrFromXml },
    { "DivideResponse", DivideResponsePtrFromXml },
};

static void addNamespace(
    soaplib::xml::Node& node,
    const char* prefix,
    const char* href)
{
    auto np = node.GetXmlNode();
    xmlSetNs(np, xmlNewNs(np, BAD_CAST href, BAD_CAST prefix));
}

void SoapLibAnyTypeFromXml(
    const soaplib::xml::Node& objNode,
    SoapLibAnyType& obj)
{
    auto type = objNode.GetStringProp("type");
    auto idx = type.find(':');
    if (idx != std::string::npos)
    {
        type = type.substr(idx + 1);
    }
    auto it = typeMap.find(type);
    if (it != typeMap.end())
    {
        obj.Value = it->second(objNode);
    }
}

SoapLibAnyType SoapLibAnyTypeFromXml(
    const soaplib::xml::Node& objNode)
{
    SoapLibAnyType obj;
    SoapLibAnyTypeFromXml(objNode, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> SoapLibAnyTypePtrFromXml(
    const soaplib::xml::Node& node)
{
    return {}; // TODO
}

static void _SoapLibAnyTypeToXml(
    const SoapLibAnyType& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& objNode)
{
    if (obj.Value)
    {
        obj.Value->ToAnyXml(doc, objNode);
    }
}

void SoapLibAnyTypeToXml(
    const SoapLibAnyType& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode)
{
    if (createNode)
    {
        auto objNode = soaplib::addChild(doc, parentNode, "SoapLibAnyType", "", "");
        _SoapLibAnyTypeToXml(obj, doc, objNode);
    }
    else
    {
        _SoapLibAnyTypeToXml(obj, doc, parentNode);
    }
}

} // namespace calc
