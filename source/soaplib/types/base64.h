//
//  base64 encoding and decoding with C++.
//  Version: 1.01.00
//

#ifndef BASE64_H_C0CE2A47_D10E_42C9_A27C_C883944E704A
#define BASE64_H_C0CE2A47_D10E_42C9_A27C_C883944E704A

#include <memory>
#include <string>
#include <soaplib/types/soapBaseType.hpp>


namespace soaplib {

namespace xml { class Node; };

/// Base64 Type
class Base64
    : public SoapBaseType
{
public:
    /// Creates empty instance
    Base64()
    {
    }

    /// Creates instance with data
    /// @param[in] data The data to hold
    Base64(const std::string& data)
        : data_(data)
    {
    }

    /// Retrieves the contained data
    /// @returns The data
    const std::string& Data() const
    {
        return data_;
    }

private:
    std::string data_;  ///< The data
};

} // namespace soaplib

/// Encodes the given data to Base64.
/// @param[in] bytes_to_encode The raw data
/// @param[in] in_len The size of the raw data
/// @returns The encoded data
std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len);

/// Decodes the given data from Base64.
/// @param[in] encoded_string The encoded input data
/// @returns The raw data
std::string base64_decode(std::string const& encoded_string);

/// Extracts and decodes data from XML.
/// @param[in] node The XML node
/// @param[out] obj The Base64 data
void Base64FromXml(const soaplib::xml::Node& node, soaplib::Base64& obj);

/// Extracts and decodes data from XML.
/// @param[in] node The XML node
/// @returns The Base64 data
soaplib::Base64 Base64FromXml(const soaplib::xml::Node& node);

/// Extracts and decodes data from XML, as pointer type.
/// @param[in] node The XML node
/// @returns Pointer holding the Base64 data.
/// @throws std::bad_alloc, ...
std::unique_ptr<soaplib::SoapBaseType> Base64PtrFomXml(const soaplib::xml::Node& node);

/// Encodes and writes data to XML.
/// @param[in] node The XML node
/// @param[in] value The Base64 object
void Base64ToXml(soaplib::xml::Node& node, const soaplib::Base64& value);

#endif /* BASE64_H_C0CE2A47_D10E_42C9_A27C_C883944E704A */
