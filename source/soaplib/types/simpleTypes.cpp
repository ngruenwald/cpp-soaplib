#include "simpleTypes.hpp"

#include "xml/xml.hpp"
#include "parseHelper.hpp"


#define XS_HREF "http://www.w3.org/2001/XMLSchema"
#define XS_PREFIX "xxs"

#define TNS_HREF "http://schemas.microsoft.com/2003/10/Serialization/"
#define TNS_PREFIX "tns"


//
// helpers
//

#ifdef WIN32
#include <string.h>
inline bool _strCaseCompare(
    const std::string& a,
    const std::string& b)
{
    return _stricmp(a.c_str(), b.c_str()) == 0;
}
#else
#include <strings.h>
inline bool _strCaseCompare(
    const std::string& a,
    const std::string& b)
{
    return strcasecmp(a.c_str(), b.c_str()) == 0;
}
#endif


template<typename Tvalue>
Tvalue _valueFromXml(
    const soaplib::xml::Node& node)
{
    auto s = node.GetStringVal();
    auto v = std::stoll(s);
    return static_cast<Tvalue>(v);
}

template<typename T>
void _valueToXml(
    soaplib::xml::Node& node,
    T value)
{
    node.SetVal(std::to_string(value));
}

// ugly but much less typing ;)

#define _FROM_XML_REF(TYPE)                                                    \
    void TYPE ## FromXml(const soaplib::xml::Node& node, soaplib::TYPE& obj)   \
    {                                                                          \
        obj.Value = _valueFromXml<soaplib::TYPE::ValueType>(node);             \
    }

#define _FROM_XML_RET(TYPE)                                                    \
    soaplib::TYPE TYPE ## FromXml(const soaplib::xml::Node& node)              \
    {                                                                          \
        soaplib::TYPE obj;                                                     \
        TYPE ## FromXml(node, obj);                                            \
        return obj;                                                            \
    }

#define _FROM_XML_PTR(TYPE)                                                    \
    std::unique_ptr<soaplib::SoapBaseType> TYPE ## PtrFromXml(                 \
        const soaplib::xml::Node& node)                                        \
    {                                                                          \
        auto ptr = std::make_unique<soaplib::TYPE>();                          \
        TYPE ## FromXml(node, *ptr.get());                                     \
        return ptr;                                                            \
    }

#define _TO_XML(TYPE)                                                          \
    void TYPE ## ToXml(soaplib::xml::Node& node, const soaplib::TYPE& value)   \
    {                                                                          \
        _valueToXml(node, value.Value);                                        \
    }

#define _TO_ANY_XML(TYPE, TYPE_NAM, TYPE_HRF, TYPE_PFX)                        \
    void TYPE ## ToAnyXml(                                                     \
        soaplib::xml::Document& doc,                                           \
        soaplib::xml::Node& node,                                              \
        const soaplib::TYPE& value)                                            \
    {                                                                          \
        soaplib::setAnyTypeAttribute(doc, node, TYPE_NAM, TYPE_HRF, TYPE_PFX); \
        TYPE ## ToXml(node, value);                                            \
    }


#define _VAL_FROM_TO_XML(TYPE, TYPE_NAME, TYPE_HREF, TYPE_PREFIX)   \
    _FROM_XML_REF(TYPE)                                             \
    _FROM_XML_RET(TYPE)                                             \
    _FROM_XML_PTR(TYPE)                                             \
    _TO_XML(TYPE)                                                   \
    _TO_ANY_XML(TYPE, TYPE_NAME, TYPE_HREF, TYPE_PREFIX)


//
// void
//

void VoidFromXml(
    const soaplib::xml::Node& node,
    soaplib::Void& obj)
{
}

_FROM_XML_RET(Void)
_FROM_XML_PTR(Void)

void VoidToXml(
    soaplib::xml::Node& node,
    const soaplib::Void& obj)
{
}

//
// signed integers
//

_VAL_FROM_TO_XML(Int,   "int",   XS_HREF,  XS_PREFIX)
_VAL_FROM_TO_XML(Int8,  "char",  TNS_HREF, TNS_PREFIX)
_VAL_FROM_TO_XML(Int16, "short", XS_HREF,  XS_PREFIX)
_VAL_FROM_TO_XML(Int32, "int",   XS_HREF,  XS_PREFIX)
_VAL_FROM_TO_XML(Int64, "long",  XS_HREF,  XS_PREFIX)

//
// unsigned integers
//

_VAL_FROM_TO_XML(UInt,   "unsignedInt",   XS_HREF, XS_PREFIX)
_VAL_FROM_TO_XML(UInt8,  "unsignedByte",  XS_HREF, XS_PREFIX)
_VAL_FROM_TO_XML(UInt16, "unsignedShort", XS_HREF, XS_PREFIX)
_VAL_FROM_TO_XML(UInt32, "unsignedInt",   XS_HREF, XS_PREFIX)
_VAL_FROM_TO_XML(UInt64, "unsignedLong",  XS_HREF, XS_PREFIX)

//
// strings
//

void StringFromXml(
    const soaplib::xml::Node& node,
    soaplib::String& value)
{
    value.Value = node.GetStringVal();
}

_FROM_XML_RET(String)
_FROM_XML_PTR(String)

void StringToXml(
    soaplib::xml::Node& node,
    const soaplib::String& value)
{
    node.SetVal(value.Value);
}

void StringToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& anyNode,
    const soaplib::String& value)
{
    soaplib::setAnyTypeAttribute(doc, anyNode, "string", XS_HREF, XS_PREFIX);
    StringToXml(anyNode, value);
}

//
// boolean
//

soaplib::Bool BoolFromXml(
    const soaplib::xml::Node& node)
{
    auto s = node.GetStringVal();

    if (_strCaseCompare(s, "true") || s == "1")
    {
        return true;
    }

    return false;
}

_FROM_XML_REF(Bool)
_FROM_XML_PTR(Bool)

void BoolToXml(
    soaplib::xml::Node& node,
    const soaplib::Bool& value)
{
    node.SetVal(value ? "true" : "false");
}

void BoolToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& anyNode,
    const soaplib::Bool& value)
{
    soaplib::setAnyTypeAttribute(doc, anyNode, "boolean", XS_HREF, XS_PREFIX);
    BoolToXml(anyNode, value);
}

//
// floating
//

soaplib::Float FloatFromXml(
    const soaplib::xml::Node& node)
{
    auto s = node.GetStringVal();
    return std::stof(s);
}

_FROM_XML_REF(Float)
_FROM_XML_PTR(Float)

void FloatToXml(
    soaplib::xml::Node& node,
    const soaplib::Float& value)
{
    node.SetVal(std::to_string(value.Value));
}

void FloatToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& anyNode,
    const soaplib::Float& value)
{
    soaplib::setAnyTypeAttribute(doc, anyNode, "float", XS_HREF, XS_PREFIX);
    FloatToXml(anyNode, value);
}

soaplib::Double DoubleFromXml(
    const soaplib::xml::Node& node)
{
    auto s = node.GetStringVal();
    return std::stod(s);
}

_FROM_XML_REF(Double)
_FROM_XML_PTR(Double)

void DoubleToXml(
    soaplib::xml::Node& node,
    const soaplib::Double& value)
{
    node.SetVal(std::to_string(value.Value));
}

void DoubleToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& anyNode,
    const soaplib::Double& value)
{
    soaplib::setAnyTypeAttribute(doc, anyNode, "double", XS_HREF, XS_PREFIX);
    DoubleToXml(anyNode, value);
}
