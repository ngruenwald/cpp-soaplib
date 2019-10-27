#pragma once

#include <memory>
#include <string>

namespace xml { class Document; }

namespace soaplib {

class SoapClient
{
public:
	SoapClient(
		const std::string& serviceAddress,
		int timeoutSeconds = 5);

	~SoapClient();

	std::shared_ptr<xml::Document> Send(
		const xml::Document& request);

private:
	void extractAddressParts(
		const std::string& serviceAddress);

private:
	std::string host_;
	int port_;
	std::string path_;
	int timeout_;	// seconds

};

} // namespace soaplib
