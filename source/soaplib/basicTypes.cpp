#include "basicTypes.hpp"

#include "soaplib/xml/xml.hpp"

#if 0

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

//namespace soaplib {

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

std::int8_t int8_tFromXml(
    const soaplib::xml::Node& node)
{
	return _valueFromXml<std::int8_t>(node);
}

void int8_tToXml(
    soaplib::xml::Node& node,
    std::int8_t value)
{
    _valueToXml(node, value);
}

std::int16_t int16_tFromXml(
    const soaplib::xml::Node& node)
{
    return _valueFromXml<std::int16_t>(node);
}

void int16_tToXml(
    soaplib::xml::Node& node,
    std::int16_t value)
{
    _valueToXml(node, value);
}

std::int32_t int32_tFromXml(
    const soaplib::xml::Node& node)
{
    return _valueFromXml<std::int32_t>(node);
}

void int32_tToXml(
    soaplib::xml::Node& node,
    std::int32_t value)
{
    _valueToXml(node, value);
}

std::int64_t int64_tFromXml(
    const soaplib::xml::Node& node)
{
    return _valueFromXml<std::int64_t>(node);
}

void int64_tToXml(
    soaplib::xml::Node& node,
    std::int64_t value)
{
    _valueToXml(node, value);
}


std::uint8_t uint8_tFromXml(
    const soaplib::xml::Node& node)
{
    return _valueFromXml<std::uint8_t>(node);
}

void uint8_tToXml(
    soaplib::xml::Node& node,
    std::uint8_t value)
{
    _valueToXml(node, value);
}

std::uint16_t uint16_tFromXml(
    const soaplib::xml::Node& node)
{
    return _valueFromXml<std::uint16_t>(node);
}

void uint16_tToXml(
    soaplib::xml::Node& node,
    std::uint16_t value)
{
    _valueToXml(node, value);
}

std::uint32_t uint32_tFromXml(
    const soaplib::xml::Node& node)
{
    return _valueFromXml<std::uint32_t>(node);
}

void uint32_tToXml(
    soaplib::xml::Node& node,
    std::uint32_t value)
{
    _valueToXml(node, value);
}

std::uint64_t uint64_tFromXml(
    const soaplib::xml::Node& node)
{
    return _valueFromXml<std::uint64_t>(node);
}

void uint64_tToXml(
    soaplib::xml::Node& node,
    std::uint64_t value)
{
    _valueToXml(node, value);
}


std::string stringFromXml(
    const soaplib::xml::Node& node)
{
    return node.GetStringVal();
}

void stringToXml(
    soaplib::xml::Node& node,
    const std::string& value)
{
    node.SetVal(value);
}


bool boolFromXml(
    const soaplib::xml::Node& node)
{
    auto s = node.GetStringVal();

    if (_strCaseCompare(s, "true") || s == "1")
    {
        return true;
    }

    return false;
}

void boolToXml(
    soaplib::xml::Node& node,
    bool value)
{
    node.SetVal(value ? "true" : "false");
}


float floatFromXml(
    const soaplib::xml::Node& node)
{
    auto s = node.GetStringVal();
    return std::stof(s);
}

void floatToXml(
    soaplib::xml::Node& node,
    float value)
{
    node.SetVal(std::to_string(value));
}

double doubleFromXml(
    const soaplib::xml::Node& node)
{
    auto s = node.GetStringVal();
    return std::stod(s);
}

void doubleToXml(
    soaplib::xml::Node& node,
    double value)
{
    node.SetVal(std::to_string(value));
}


soaplib::Base64 Base64FromXml(
    const soaplib::xml::Node& node)
{
	auto s = node.GetStringVal();
	return soaplib::Base64(base64_decode(s));
}

void Base64ToXml(
    soaplib::xml::Node& node,
    const soaplib::Base64& value)
{
    auto ptr = reinterpret_cast<const unsigned char*>(value.Data().c_str());
    auto len = value.Data().length();
    node.SetVal(base64_encode(ptr, len));
}


soaplib::Timestamp TimestampFromXml(
    const soaplib::xml::Node& node)
{
    auto s = node.GetStringVal();
    return soaplib::Timestamp::FromString(s);
}

void TimestampToXml(
    soaplib::xml::Node& node,
    const soaplib::Timestamp& value)
{
    node.SetVal(soaplib::Timestamp::ToString(value));
}

soaplib::Duration DurationFromXml(
    const soaplib::xml::Node& node)
{
    auto s = node.GetStringVal();
    return soaplib::Duration::FromString(s);
}

void DurationToXml(
    soaplib::xml::Node& node,
    const soaplib::Duration& value)
{
    node.SetVal(soaplib::Duration::ToString(value));
}


soaplib::uuid uuidFromXml(
    const soaplib::xml::Node& node)
{
    auto s = node.GetStringVal();
	soaplib::uuid u;
	u.from_string(s);
    return u;
}

void uuidToXml(
    soaplib::xml::Node& node,
    const soaplib::uuid& value)
{
    node.SetVal(value.to_string());
}

//} // namespace soaplib
#endif
