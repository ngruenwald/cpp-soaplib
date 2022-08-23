#pragma once

#include <cassert>
#include <cstdint>
#include <memory>
#include <string>

#include <soaplib/types/soapBaseType.hpp>

namespace soaplib {

namespace xml { class Document; }
namespace xml { class Node; }

/// Empty type
struct Void
    : SoapBaseType
{
    /// Void to XML
    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& node) const override
    {
    }
};

/// Container for simple types
/// @tparam T Type of the contained value
template<typename T>
struct TSimpleType
    : SoapBaseType
{
    typedef T ValueType;    ///< The type of the contained value

    T Value;                ///< The contained value

    /// Default constructor
    TSimpleType() = default;

    /// Constructs the object with the given value
    /// @param[in] value The value
    TSimpleType(const T& value)
        : Value{value}
    {
    }

    /// Gets a reference to the value
    /// @returns Reference to the contained value
    operator T&()
    {
        return Value;
    }

    /// Gets the value
    /// @returns The contained valued
    operator T() const
    {
        return Value;
    }

    /// Assigns a (new) value
    /// @param[in] value The value
    /// @returns Reference to the object
    TSimpleType& operator=(const T& value)
    {
        Value = value;
        return *this;
    }

    /// Compares against the given value
    /// @param[in] other The value to compare against
    /// @returns The result of the value comparison
    bool operator==(const T& other)
    {
        return Value == other;
    }

    /// Type to XML
    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& node) const override;
};

//
// signed integers
//

typedef TSimpleType<int> Int;               ///< Signed integer type
typedef TSimpleType<std::int8_t> Int8;      ///<  8 bit signed integer type
typedef TSimpleType<std::int16_t> Int16;    ///< 16 bit signed integer type
typedef TSimpleType<std::int32_t> Int32;    ///< 32 bit signed integer type
typedef TSimpleType<std::int64_t> Int64;    ///< 64 bit signed integer type

//
// unsigned integers
//

typedef TSimpleType<unsigned int> UInt;     /// Unsigned integer type
typedef TSimpleType<std::uint8_t> UInt8;    ///<  8 bit unsigned integer type
typedef TSimpleType<std::uint16_t> UInt16;  ///< 16 bit unsigned integer type
typedef TSimpleType<std::uint32_t> UInt32;  ///< 32 bit unsigned integer type
typedef TSimpleType<std::uint64_t> UInt64;  ///< 64 bit unsigned integer type

//
// strings
//

/// String type
struct String
    : TSimpleType<std::string>
{
    /// Default constructor
    String() = default;

    /// Constructs a string object with the given string
    /// @param[in] value The string value
    explicit String(
        const std::string& value)
        : TSimpleType<std::string>(value)
    {
    }

    /// Constructs a string object with the given c-string
    /// @param[in] value The string value
    explicit String(
        const char* value)
    {
        Value = value;
    }

    /// Assigns a value
    /// @param[in] value The value
    /// @returns Reference to the object
    String& operator=(
        const std::string& value)
    {
        Value = value;
        return *this;
    }

    /// Assigns a value
    /// @param[in] value The value
    /// @returns Reference to the object
    String& operator=(
        const char* value)
    {
        Value = value;
        return *this;
    }

    /// Gets the string
    /// @returns The contained string
    const std::string& str() const
    {
        return Value;
    }
};

//
// boolean
//

typedef TSimpleType<bool> Bool;         ///< Boolean type

//
// floating
//

typedef TSimpleType<float> Float;       ///< Float value
typedef TSimpleType<double> Double;     ///< Double value

} // namespace soaplib


//
// void
//

/// Extracts a void from XML
/// @param[in] node XML node
/// @param[out] obj Void object
void VoidFromXml(const soaplib::xml::Node& node, soaplib::Void& obj);

/// Extracts a void from XML
/// @param[in] node XML node
/// @returns Void object
soaplib::Void VoidFromXml(const soaplib::xml::Node& node);

/// Extracts a void from XML, as pointer type
/// @param[in] node XML node
/// @returns A pointer holding the void object
std::unique_ptr<soaplib::SoapBaseType> VoidPtrFomXml(const soaplib::xml::Node& node);

/// Writes a void to XML
/// @param[in] node XML node
/// @param[in] obj Void object
void VoidToXml(soaplib::xml::Node& node, const soaplib::Void& obj);

// // TODO: quick fix ... solve this properly
namespace soaplib { inline soaplib::Void VoidFromXml(const soaplib::xml::Node& node) { return ::VoidFromXml(node); } }

//
// signed integers
//

/// Extracts a signed integer from XML
/// @param[in] node XML node
/// @param[out] value Value object
void IntFromXml(const soaplib::xml::Node& node, soaplib::Int& value);

/// Extracts a signed integer from XML
/// @param[in] node XML node
/// @returns Value object
soaplib::Int IntFromXml(const soaplib::xml::Node& node);

/// Extracts a signed integer from XML, as pointer type
/// @param[in] node XML node
/// @returns Pointer holding the value object
std::unique_ptr<soaplib::SoapBaseType> IntPtrFromXml(const soaplib::xml::Node& node);

/// Writes a signed integer to XML
/// @param[in] node XML node
/// @param[in] value Value object
void IntToXml(soaplib::xml::Node& node, const soaplib::Int& value);

/// Writes a signed integer to XML
/// @param[in] doc XML document
/// @param[in] anyNode XML node
/// @param[in] value Value object
void IntToAnyXml(soaplib::xml::Document& doc, soaplib::xml::Node& anyNode, const soaplib::Int& value);

/// Extracts an 8-bit signed integer from XML
/// @param[in] node XML node
/// @param[out] value Value object
void Int8FromXml(const soaplib::xml::Node& node, soaplib::Int8& value);

/// Extracts an 8-bit signed integer from XML
/// @param[in] node XML node
/// @returns Value object
soaplib::Int8 Int8FromXml(const soaplib::xml::Node& node);

/// Extracts an 8-bit signed integer from XML, as pointer type
/// @param[in] node XML node
/// @returns Pointer holding the value object
std::unique_ptr<soaplib::SoapBaseType> Int8PtrFromXml(const soaplib::xml::Node& node);

/// Writes an 8-bit signed integer to XML
/// @param[in] node XML node
/// @param[in] value Value object
void Int8ToXml(soaplib::xml::Node& node, const soaplib::Int8& value);

/// Writes an 8-bit signed integer to XML
/// @param[in] doc XML document
/// @param[in] anyNode XML node
/// @param[in] value Value object
void Int8ToAnyXml(soaplib::xml::Document& doc, soaplib::xml::Node& anyNode, const soaplib::Int8& value);

/// Extracts a 16-bit signed integer from XML
/// @param[in] node XML node
/// @param[out] value Value object
void Int16FromXml(const soaplib::xml::Node& node, soaplib::Int16& value);

/// Extracts a 16-bit signed integer from XML
/// @param[in] node XML node
/// @returns Value object
soaplib::Int16 Int16FromXml(const soaplib::xml::Node& node);

/// Extracts a 16-bit signed integer from XML, as pointer type
/// @param[in] node XML node
/// @returns Pointer holding the value object
std::unique_ptr<soaplib::SoapBaseType> Int16PtrFromXml(const soaplib::xml::Node& node);

/// Writes a 16-bit signed integer to XML
/// @param[in] node XML node
/// @param[in] value Value object
void Int16ToXml(soaplib::xml::Node& node, const soaplib::Int16& value);

/// Writes a 16-bit signed integer to XML
/// @param[in] doc XML document
/// @param[in] anyNode XML node
/// @param[in] value Value object
void Int16ToAnyXml(soaplib::xml::Document& doc, soaplib::xml::Node& anyNode, const soaplib::Int16& value);

/// Extracts a 32-bit signed integer from XML
/// @param[in] node XML node
/// @param[out] value Value object
void Int32FromXml(const soaplib::xml::Node& node, soaplib::Int32& value);

/// Extracts a 32-bit signed integer from XML
/// @param[in] node XML node
/// @returns Value object
soaplib::Int32 Int32FromXml(const soaplib::xml::Node& node);

/// Extracts a 32-bit signed integer from XML, as pointer type
/// @param[in] node XML node
/// @returns Pointer holding the value object
std::unique_ptr<soaplib::SoapBaseType> Int32PtrFromXml(const soaplib::xml::Node& node);

/// Writes a 32-bit signed integer to XML
/// @param[in] node XML node
/// @param[in] value Value object
void Int32ToXml(soaplib::xml::Node& node, const soaplib::Int32& value);

/// Writes a 32-bit signed integer to XML
/// @param[in] doc XML document
/// @param[in] anyNode XML node
/// @param[in] value Value object
void Int32ToAnyXml(soaplib::xml::Document& doc, soaplib::xml::Node& anyNode, const soaplib::Int32& value);

/// Extracts a 64-bit signed integer from XML
/// @param[in] node XML node
/// @param[out] value Value object
void Int64FromXml(const soaplib::xml::Node& node, soaplib::Int64& value);

/// Extracts a 64-bit signed integer from XML
/// @param[in] node XML node
/// @returns Value object
soaplib::Int64 Int64FromXml(const soaplib::xml::Node& node);

/// Extracts a 64-bit signed integer from XML, as pointer type
/// @param[in] node XML node
/// @returns Pointer holding the value object
std::unique_ptr<soaplib::SoapBaseType> Int64PtrFromXml(const soaplib::xml::Node& node);

/// Writes a 64-bit signed integer to XML
/// @param[in] node XML node
/// @param[in] value Value object
void Int64ToXml(soaplib::xml::Node& node, const soaplib::Int64& value);

/// Writes an 64-bit signed integer to XML
/// @param[in] doc XML document
/// @param[in] anyNode XML node
/// @param[in] value Value object
void Int64ToAnyXml(soaplib::xml::Document& doc, soaplib::xml::Node& anyNode, const soaplib::Int64& value);

//
// unsigned integers
//

/// Extracts an unsigned integer from XML
/// @param[in] node XML node
/// @param[out] value Value object
void UIntFromXml(const soaplib::xml::Node& node, soaplib::UInt& value);

/// Extracts an unsigned integer from XML
/// @param[in] node XML node
/// @returns Value object
soaplib::UInt UIntFromXml(const soaplib::xml::Node& node);

/// Extracts an unsigned integer from XML, as pointer type
/// @param[in] node XML node
/// @returns Pointer holding the value object
std::unique_ptr<soaplib::SoapBaseType> UIntPtrFromXml(const soaplib::xml::Node& node);

/// Writes an unsigned integer to XML
/// @param[in] node XML node
/// @param[in] value Value object
void UIntToXml(soaplib::xml::Node& node, const soaplib::UInt& value);

/// Writes an unsigned signed integer to XML
/// @param[in] doc XML document
/// @param[in] anyNode XML node
/// @param[in] value Value object
void UIntToAnyXml(soaplib::xml::Document& doc, soaplib::xml::Node& anyNode, const soaplib::UInt& value);

/// Extracts an 8-bit unsigned integer from XML
/// @param[in] node XML node
/// @param[out] value Value object
void UInt8FromXml(const soaplib::xml::Node& node, soaplib::UInt8& value);

/// Extracts an 8-bit unsigned integer from XML
/// @param[in] node XML node
/// @returns Value object
soaplib::UInt8 UInt8FromXml(const soaplib::xml::Node& node);

/// Extracts an 8-bit unsigned integer from XML, as pointer type
/// @param[in] node XML node
/// @returns Pointer holding the value object
std::unique_ptr<soaplib::SoapBaseType> UInt8PtrFromXml(const soaplib::xml::Node& node);

/// Writes an 8-bit unsigned integer to XML
/// @param[in] node XML node
/// @param[in] value Value object
void UInt8ToXml(soaplib::xml::Node& node, const soaplib::UInt8& value);

/// Writes an 8-bit unsigned signed integer to XML
/// @param[in] doc XML document
/// @param[in] anyNode XML node
/// @param[in] value Value object
void UInt8ToAnyXml(soaplib::xml::Document& doc, soaplib::xml::Node& anyNode, const soaplib::UInt8& value);

/// Extracts a 16-bit unsigned integer from XML
/// @param[in] node XML node
/// @param[out] value Value object
void UInt16FromXml(const soaplib::xml::Node& node, soaplib::UInt16& value);

/// Extracts a 16-bit unsigned integer from XML
/// @param[in] node XML node
/// @returns Value object
soaplib::UInt16 UInt16FromXml(const soaplib::xml::Node& node);

/// Extracts a 16-bit unsigned integer from XML, as pointer type
/// @param[in] node XML node
/// @returns Pointer holding the value object
std::unique_ptr<soaplib::SoapBaseType> UInt16PtrFromXml(const soaplib::xml::Node& node);

/// Writes a 16-bit unsigned integer to XML
/// @param[in] node XML node
/// @param[in] value Value object
void UInt16ToXml(soaplib::xml::Node& node, const soaplib::UInt16& value);

/// Writes an 16-bit unsigned signed integer to XML
/// @param[in] doc XML document
/// @param[in] anyNode XML node
/// @param[in] value Value object
void UInt16ToAnyXml(soaplib::xml::Document& doc, soaplib::xml::Node& anyNode, const soaplib::UInt16& value);

/// Extracts a 32-bit unsigned integer from XML
/// @param[in] node XML node
/// @param[out] value Value object
void UInt32FromXml(const soaplib::xml::Node& node, soaplib::UInt32& value);

/// Extracts a 32-bit unsigned integer from XML
/// @param[in] node XML node
/// @returns Value object
soaplib::UInt32 UInt32FromXml(const soaplib::xml::Node& node);

/// Extracts a 32-bit unsigned integer from XML, as pointer type
/// @param[in] node XML node
/// @returns Pointer holding the value object
std::unique_ptr<soaplib::SoapBaseType> UInt32PtrFromXml(const soaplib::xml::Node& node);

/// Writes a 32-bit unsigned integer to XML
/// @param[in] node XML node
/// @param[in] value Value object
void UInt32ToXml(soaplib::xml::Node& node, const soaplib::UInt32& value);

/// Writes an 32-bit unsigned signed integer to XML
/// @param[in] doc XML document
/// @param[in] anyNode XML node
/// @param[in] value Value object
void UInt32ToAnyXml(soaplib::xml::Document& doc, soaplib::xml::Node& anyNode, const soaplib::UInt32& value);

/// Extracts a 64-bit unsigned integer from XML
/// @param[in] node XML node
/// @param[out] value Value object
void UInt64FromXml(const soaplib::xml::Node& node, soaplib::UInt64& value);

/// Extracts a 64-bit unsigned integer from XML
/// @param[in] node XML node
/// @returns Value object
soaplib::UInt64 UInt64FromXml(const soaplib::xml::Node& node);

/// Extracts a 64-bit unsigned integer from XML, as pointer type
/// @param[in] node XML node
/// @returns Pointer holding the value object
std::unique_ptr<soaplib::SoapBaseType> UInt64PtrFromXml(const soaplib::xml::Node& node);

/// Writes a 64-bit unsigned integer to XML
/// @param[in] node XML node
/// @param[in] value Value object
void UInt64ToXml(soaplib::xml::Node& node, const soaplib::UInt64& value);

/// Writes an 64-bit unsigned signed integer to XML
/// @param[in] doc XML document
/// @param[in] anyNode XML node
/// @param[in] value Value object
void UInt64ToAnyXml(soaplib::xml::Document& doc, soaplib::xml::Node& anyNode, const soaplib::UInt64& value);

//
// strings
//

/// Extracts a string from XML
/// @param[in] node XML node
/// @param[out] value Value object
void StringFromXml(const soaplib::xml::Node& node, soaplib::String& value);

/// Extracts a string from XML
/// @param[in] node XML node
/// @returns Value object
soaplib::String StringFromXml(const soaplib::xml::Node& node);

/// Extracts a string from XML, as pointer type
/// @param[in] node XML node
/// @returns Pointer holding the value object
std::unique_ptr<soaplib::SoapBaseType> StringPtrFromXml(const soaplib::xml::Node& node);

/// Writes a string to XML
/// @param[in] node XML node
/// @param[in] value Value object
void StringToXml(soaplib::xml::Node& node, const soaplib::String& value);

/// Writes a string to XML
/// @param[in] doc XML document
/// @param[in] anyNode XML node
/// @param[in] value Value object
void StringToAnyXml(soaplib::xml::Document& doc, soaplib::xml::Node& anyNode, const soaplib::String& value);

//
// boolean
//

/// Extracts a boolean from XML
/// @param[in] node XML node
/// @param[out] value Value object
void BoolFromXml(const soaplib::xml::Node& node, soaplib::Bool& value);

/// Extracts a boolean from XML
/// @param[in] node XML node
/// @returns Value object
soaplib::Bool BoolFromXml(const soaplib::xml::Node& node);

/// Extracts a boolean from XML, as pointer type
/// @param[in] node XML node
/// @returns Pointer holding the value object
std::unique_ptr<soaplib::SoapBaseType> BoolPtrFromXml(const soaplib::xml::Node& node);

/// Writes a boolean to XML
/// @param[in] node XML node
/// @param[in] value Value object
void BoolToXml(soaplib::xml::Node& node, const soaplib::Bool& value);

/// Writes a boolean to XML
/// @param[in] doc XML document
/// @param[in] anyNode XML node
/// @param[in] value Value object
void BoolToAnyXml(soaplib::xml::Document& doc, soaplib::xml::Node& anyNode, const soaplib::Bool& value);

//
// floating
//

/// Extracts a float value from XML
/// @param[in] node XML node
/// @param[out] value Value object
void FloatFromXml(const soaplib::xml::Node& node, soaplib::Float& value);

/// Extracts a float value from XML
/// @param[in] node XML node
/// @returns Value object
soaplib::Float FloatFromXml(const soaplib::xml::Node& node);

/// Extracts a float value from XML, as pointer type
/// @param[in] node XML node
/// @returns Pointer holding the value object
std::unique_ptr<soaplib::SoapBaseType> FloatPtrFromXml(const soaplib::xml::Node& node);

/// Writes a float value to XML
/// @param[in] node XML node
/// @param[in] value Value object
void FloatToXml(soaplib::xml::Node& node, const soaplib::Float& value);

/// Writes a float value to XML
/// @param[in] doc XML document
/// @param[in] anyNode XML node
/// @param[in] value Value object
void FloatToAnyXml(soaplib::xml::Document& doc, soaplib::xml::Node& anyNode, const soaplib::Float& value);

/// Extracts a double value from XML
/// @param[in] node XML node
/// @param[out] value Value object
void DoubleFromXml(const soaplib::xml::Node& node, soaplib::Double& value);

/// Extracts a double value from XML
/// @param[in] node XML node
/// @returns Value object
soaplib::Double DoubleFromXml(const soaplib::xml::Node& node);

/// Extracts a double value from XML, as pointer type
/// @param[in] node XML node
/// @returns Pointer holding the value object
std::unique_ptr<soaplib::SoapBaseType> DoublePtrFromXml(const soaplib::xml::Node& node);

/// Writes a double value to XML
/// @param[in] node XML node
/// @param[in] value Value object
void DoubleToXml(soaplib::xml::Node& node, const soaplib::Double& value);

/// Writes a double value to XML
/// @param[in] doc XML document
/// @param[in] anyNode XML node
/// @param[in] value Value object
void DoubleToAnyXml(soaplib::xml::Document& doc, soaplib::xml::Node& anyNode, const soaplib::Double& value);

namespace soaplib {

//template<typename T> void TSimpleType<T>::ToAnyXml(xml::Document& d, xml::Node& n) const { }

template<> inline void TSimpleType<std::string>::ToAnyXml(xml::Document& d, xml::Node& n) const
{
    auto ptr = dynamic_cast<const String*>(this);
    assert(ptr);
    StringToAnyXml(d, n, *ptr);
}

template<> inline void TSimpleType<std::int8_t>::ToAnyXml(xml::Document& d, xml::Node& n) const { Int8ToAnyXml(d, n, *this); }
template<> inline void TSimpleType<std::int16_t>::ToAnyXml(xml::Document& d, xml::Node& n) const { Int16ToAnyXml(d, n, *this); }
template<> inline void TSimpleType<std::int32_t>::ToAnyXml(xml::Document& d, xml::Node& n) const { Int32ToAnyXml(d, n, *this); }
template<> inline void TSimpleType<std::int64_t>::ToAnyXml(xml::Document& d, xml::Node& n) const { Int64ToAnyXml(d, n, *this); }

template<> inline void TSimpleType<std::uint8_t>::ToAnyXml(xml::Document& d, xml::Node& n) const { UInt8ToAnyXml(d, n, *this); }
template<> inline void TSimpleType<std::uint16_t>::ToAnyXml(xml::Document& d, xml::Node& n) const { UInt16ToAnyXml(d, n, *this); }
template<> inline void TSimpleType<std::uint32_t>::ToAnyXml(xml::Document& d, xml::Node& n) const { UInt32ToAnyXml(d, n, *this); }
template<> inline void TSimpleType<std::uint64_t>::ToAnyXml(xml::Document& d, xml::Node& n) const { UInt64ToAnyXml(d, n, *this); }

template<> inline void TSimpleType<bool>::ToAnyXml(xml::Document& d, xml::Node& n) const { BoolToAnyXml(d, n, *this); }

template<> inline void TSimpleType<float>::ToAnyXml(xml::Document& d, xml::Node& n) const { FloatToAnyXml(d, n, *this); }
template<> inline void TSimpleType<double>::ToAnyXml(xml::Document& d, xml::Node& n) const { DoubleToAnyXml(d, n, *this); }

}
