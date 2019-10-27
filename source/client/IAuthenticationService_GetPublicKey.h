#pragma once

#include <libxmlwrp.hpp>
#include <string>

struct GetPublicKey
{

	void toXml(xml::Node& parentNode)
	{

	}
};

struct GetPublicKeyResult
{
	std::string value;

	static GetPublicKeyResult fromXml(xml::Node& objNode)
	{
		GetPublicKeyResult obj;
		obj.value = objNode.GetChild("PublicKey").GetStringVal();
		return obj;
	}
};

struct GetPublicKeyResponse
{
	GetPublicKeyResult result;

	static GetPublicKeyResponse fromXml(xml::Node& objNode)
	{
		GetPublicKeyResponse obj;
		auto n = objNode.GetChild("GetPublicKeyResult");
		obj.result = GetPublicKeyResult::fromXml(n);
		return obj;
	}
};
