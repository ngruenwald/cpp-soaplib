#include "simpleTypes.hpp"

#include "xml/xml.hpp"


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

#define _VAL_FROM_TO_XML(TYPE)  \
    _FROM_XML_REF(TYPE)         \
    _FROM_XML_RET(TYPE)         \
    _FROM_XML_PTR(TYPE)         \
    _TO_XML(TYPE)



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

_VAL_FROM_TO_XML(Int)
_VAL_FROM_TO_XML(Int8)
_VAL_FROM_TO_XML(Int16)
_VAL_FROM_TO_XML(Int32)
_VAL_FROM_TO_XML(Int64)

//
// unsigned integers
//

_VAL_FROM_TO_XML(UInt)
_VAL_FROM_TO_XML(UInt8)
_VAL_FROM_TO_XML(UInt16)
_VAL_FROM_TO_XML(UInt32)
_VAL_FROM_TO_XML(UInt64)

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
