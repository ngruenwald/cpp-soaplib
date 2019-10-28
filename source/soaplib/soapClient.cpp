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

	httplib::Client cli(host_.c_str(), port_, timeout_);
    cli.set_keep_alive_max_count(5);
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
