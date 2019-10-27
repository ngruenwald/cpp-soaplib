#include "soapClient.hpp"
#include "soapException.hpp"

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <sstream>

#include <httplib/httplib.h>
#include <libxmlwrp.hpp>

namespace soaplib {

SoapClient::SoapClient(
    const std::string& serviceAddress,
	int timeoutSeconds)
	: timeout_(timeoutSeconds)
{
	extractAddressParts(serviceAddress);
}

SoapClient::~SoapClient()
{
}

std::shared_ptr<xml::Document> SoapClient::Send(
	const xml::Document& request)
{
	const std::string contentType = "application/soap+xml; charset=utf-8";
	const std::string content = request.ToString();

	std::cout << content << std::endl <<std::flush;
//#define TEST
#ifndef TEST
	httplib::Client cli(host_.c_str(), port_, timeout_);
	auto response = cli.Post(path_.c_str(), content, contentType.c_str());

	if (!response)
	{
		throw SoapException("request failed");
	}

	if (response->status != 200)
	{
		throw SoapException("request was not successful: " + response->status);
	}

	return std::make_shared<xml::Document>(
		response->body.c_str(),
		response->body.length()
	);
#else
	const char* body = R"eom(
<s:Envelope xmlns:s="http://www.w3.org/2003/05/soap-envelope" xmlns:a="http://www.w3.org/2005/08/addressing">
    <s:Header>
        <a:Action s:mustUnderstand="1">http://tempuri.org/IAuthenticationService/GetPublicKeyResponse</a:Action>
        <a:RelatesTo>urn:uuid:873acb0c-99e4-4fc0-a0b5-ab11922c2e4d</a:RelatesTo>
    </s:Header>
    <s:Body>
        <GetPublicKeyResponse xmlns="http://tempuri.org/">
            <GetPublicKeyResult xmlns:b="http://schemas.datacontract.org/2004/07/Eurofunk.Idds.Administration.Authentication.DC" xmlns:i="http://www.w3.org/2001/XMLSchema-instance">
                <LocalizableMessages i:nil="true" xmlns="http://schemas.datacontract.org/2004/07/Eurofunk.Idds.Administration.Common.DataContract" xmlns:c="http://schemas.datacontract.org/2004/07/Eurofunk.Idds.Administration.Common.DC"/>
                <b:PublicKey>&lt;RSAKeyValue&gt;&lt;Modulus&gt;&lt;/Modulus&gt;&lt;Exponent&gt;AQAB&lt;/Exponent&gt;&lt;/RSAKeyValue&gt;</b:PublicKey>
            </GetPublicKeyResult>
        </GetPublicKeyResponse>
    </s:Body>
</s:Envelope>
	)eom";

	return std::make_shared<xml::Document>(body, strlen(body));
#endif
}

void SoapClient::extractAddressParts(
	const std::string& serviceAddress)
{
	auto idxHost = serviceAddress.find("://");
	idxHost = idxHost == std::string::npos ? 0 : idxHost + 3;

	auto idxPort = serviceAddress.find(":", idxHost);
	auto idxPath = serviceAddress.find("/", idxHost);

	if (idxPort != std::string::npos)
	{
		host_ = serviceAddress.substr(idxHost, idxPort - idxHost);
		idxPort = idxPort + 1;
		auto p = serviceAddress.substr(idxPort, idxPath - idxPort);
		port_ = std::stoi(p);
	}
	else
	{
		host_ = serviceAddress.substr(idxHost, idxPath - idxHost);
		port_ = 80;
	}

	path_ = serviceAddress.substr(idxPath);
}

} // namespace soaplib
