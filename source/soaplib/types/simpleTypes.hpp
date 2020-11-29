#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include <soaplib/types/soapBaseType.hpp>


namespace xml { class Node; }

namespace soaplib {

/// Empty type
struct Void
    : SoapBaseType
{
};

/// Container for simple types
template<typename T>
struct TSimpleType
    : SoapBaseType
{
    typedef T ValueType;

    T Value;

    TSimpleType() = default;

    TSimpleType(const T& value)
        : Value{value}
    {
    }

    operator T&()
    {
        return Value;
    }

    operator T() const
    {
        return Value;
    }

    TSimpleType& operator=(const T& value)
    {
        Value = value;
        return *this;
    }

    bool operator==(const T& other)
    {
        return Value == other;
    }
};

//
// signed integers
//

typedef TSimpleType<int> Int;
typedef TSimpleType<std::int8_t> Int8;
typedef TSimpleType<std::int16_t> Int16;
typedef TSimpleType<std::int32_t> Int32;
typedef TSimpleType<std::int64_t> Int64;

//
// unsigned integers
//

typedef TSimpleType<unsigned int> UInt;
typedef TSimpleType<std::uint8_t> UInt8;
typedef TSimpleType<std::uint16_t> UInt16;
typedef TSimpleType<std::uint32_t> UInt32;
typedef TSimpleType<std::uint64_t> UInt64;

//
// strings
//

//typedef TSimpleType<std::string> String;
struct String
    : TSimpleType<std::string>
{
    String() = default;

    String(const std::string& value)
        : TSimpleType<std::string>(value)
    {
    }

    String(
        const char* value)
    {
        Value = value;
    }

    String& operator=(const char* value)
    {
        Value = value;
        return *this;
    }
};

//
// boolean
//

typedef TSimpleType<bool> Bool;

//
// floating
//

typedef TSimpleType<float> Float;
typedef TSimpleType<double> Double;

} // namespace soaplib


//
// void
//

void VoidFromXml(const xml::Node& node, soaplib::Void& obj);
soaplib::Void VoidFromXml(const xml::Node& node);
std::shared_ptr<soaplib::SoapBaseType> VoidPtrFomXml(const xml::Node& node);
void VoidToXml(xml::Node& node, const soaplib::Void& obj);

// TODO: quick fix ... solve this properly
namespace soaplib { soaplib::Void VoidFromXml(const xml::Node& node) { return ::VoidFromXml(node); } }

//
// signed integers
//

void IntFromXml(const xml::Node& node, soaplib::Int& value);
soaplib::Int IntFromXml(const xml::Node& node);
std::shared_ptr<soaplib::SoapBaseType> IntPtrFromXml(const xml::Node& node);
void IntToXml(xml::Node& node, const soaplib::Int& value);

void Int8FromXml(const xml::Node& node, soaplib::Int8& value);
soaplib::Int8 Int8FromXml(const xml::Node& node);
std::shared_ptr<soaplib::SoapBaseType> Int8PtrFromXml(const xml::Node& node);
void Int8ToXml(xml::Node& node, const soaplib::Int8& value);

void Int16FromXml(const xml::Node& node, soaplib::Int16& value);
soaplib::Int16 Int16FromXml(const xml::Node& node);
std::shared_ptr<soaplib::SoapBaseType> Int16PtrFromXml(const xml::Node& node);
void Int16ToXml(xml::Node& node, const soaplib::Int16& value);

void Int32FromXml(const xml::Node& node, soaplib::Int32& value);
soaplib::Int32 Int32FromXml(const xml::Node& node);
std::shared_ptr<soaplib::SoapBaseType> Int32PtrFromXml(const xml::Node& node);
void Int32ToXml(xml::Node& node, const soaplib::Int32& value);

void Int64FromXml(const xml::Node& node, soaplib::Int64& value);
soaplib::Int64 Int64FromXml(const xml::Node& node);
std::shared_ptr<soaplib::SoapBaseType> Int64PtrFromXml(const xml::Node& node);
void Int64ToXml(xml::Node& node, const soaplib::Int64& value);

//
// unsigned integers
//

void UIntFromXml(const xml::Node& node, soaplib::UInt& value);
soaplib::UInt UIntFromXml(const xml::Node& node);
std::shared_ptr<soaplib::SoapBaseType> UIntPtrFromXml(const xml::Node& node);
void UIntToXml(xml::Node& node, const soaplib::UInt& value);

void UInt8FromXml(const xml::Node& node, soaplib::UInt8& value);
soaplib::UInt8 UInt8FromXml(const xml::Node& node);
std::shared_ptr<soaplib::SoapBaseType> UInt8PtrFromXml(const xml::Node& node);
void UInt8ToXml(xml::Node& node, const soaplib::UInt8& value);

void UInt16FromXml(const xml::Node& node, soaplib::UInt16& value);
soaplib::UInt16 UInt16FromXml(const xml::Node& node);
std::shared_ptr<soaplib::SoapBaseType> UInt16PtrFromXml(const xml::Node& node);
void UInt16ToXml(xml::Node& node, const soaplib::UInt16& value);

void UInt32FromXml(const xml::Node& node, soaplib::UInt32& value);
soaplib::UInt32 UInt32FromXml(const xml::Node& node);
std::shared_ptr<soaplib::SoapBaseType> UInt32PtrFromXml(const xml::Node& node);
void UInt32ToXml(xml::Node& node, const soaplib::UInt32& value);

void UInt64FromXml(const xml::Node& node, soaplib::UInt64& value);
soaplib::UInt64 UInt64FromXml(const xml::Node& node);
std::shared_ptr<soaplib::SoapBaseType> UInt64PtrFromXml(const xml::Node& node);
void UInt64ToXml(xml::Node& node, const soaplib::UInt64& value);

//
// strings
//

void StringFromXml(const xml::Node& node, soaplib::String& value);
soaplib::String StringFromXml(const xml::Node& node);
std::shared_ptr<soaplib::SoapBaseType> StringPtrFromXml(const xml::Node& node);
void StringToXml(xml::Node& node, const soaplib::String& value);

//
// boolean
//

void BoolFromXml(const xml::Node& node, soaplib::Bool& value);
soaplib::Bool BoolFromXml(const xml::Node& node);
std::shared_ptr<soaplib::SoapBaseType> BoolPtrFromXml(const xml::Node& node);
void BoolToXml(xml::Node& node, const soaplib::Bool& value);

//
// floating
//

void FloatFromXml(const xml::Node& node, soaplib::Float& value);
soaplib::Float FloatFromXml(const xml::Node& node);
std::shared_ptr<soaplib::SoapBaseType> FloatPtrFromXml(const xml::Node& node);
void FloatToXml(xml::Node& node, const soaplib::Float& value);

void DoubleFromXml(const xml::Node& node, soaplib::Double& value);
soaplib::Double DoubleFromXml(const xml::Node& node);
std::shared_ptr<soaplib::SoapBaseType> DoublePtrFromXml(const xml::Node& node);
void DoubleToXml(xml::Node& node, const soaplib::Double& value);
