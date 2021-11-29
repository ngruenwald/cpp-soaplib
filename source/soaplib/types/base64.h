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

class Base64
    : public SoapBaseType
{
public:
	Base64()
	{
	}

	Base64(const std::string& data)
		: data_(data)
	{
	}

	const std::string& Data() const
	{
		return data_;
	}

private:
	std::string data_;
};

} // namespace soaplib

std::string base64_encode(unsigned char const* , unsigned int len);
std::string base64_decode(std::string const& s);

void Base64FromXml(const soaplib::xml::Node& node, soaplib::Base64& obj);
soaplib::Base64 Base64FromXml(const soaplib::xml::Node& node);
std::shared_ptr<soaplib::SoapBaseType> Base64PtrFomXml(const soaplib::xml::Node& node);
void Base64ToXml(soaplib::xml::Node& node, const soaplib::Base64& value);

#endif /* BASE64_H_C0CE2A47_D10E_42C9_A27C_C883944E704A */
