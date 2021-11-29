#pragma once

#include <soaplib/xml.hpp>
#include <string>

struct GetPublicKey
{

	void toXml(soaplib::xml::Node& parentNode)
	{

	}
};

struct GetPublicKeyResult
{
	std::string value;

	static GetPublicKeyResult fromXml(soaplib::xml::Node& objNode)
	{
		GetPublicKeyResult obj;
		obj.value = objNode.GetChild("PublicKey").GetStringVal();
		return obj;
	}
};

struct GetPublicKeyResponse
{
	GetPublicKeyResult result;

	static GetPublicKeyResponse fromXml(soaplib::xml::Node& objNode)
	{
		GetPublicKeyResponse obj;
		auto n = objNode.GetChild("GetPublicKeyResult");
		obj.result = GetPublicKeyResult::fromXml(n);
		return obj;
	}
};
