#pragma once

#include <memory>
#include <string>
#include <soaplib/types/SoapBaseType.hpp>

namespace soaplib {

namespace xml { class Node; };

class Base64
    : public SoapBaseType
{
public:
    Base64();
    explicit Base64(const std::string& data);

    const std::string& Data() const;

    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& anyNode) const override;

private:
    std::string data_;
};

} // namespace soaplib

std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len);
std::string base64_decode(std::string const& encoded_string);

void Base64FromXml(const soaplib::xml::Node& node, soaplib::Base64& obj);
soaplib::Base64 Base64FromXml(const soaplib::xml::Node& node);
std::unique_ptr<soaplib::SoapBaseType> Base64PtrFromXml(const soaplib::xml::Node& node);
void Base64ToXml(soaplib::xml::Node& node, const soaplib::Base64& value);
