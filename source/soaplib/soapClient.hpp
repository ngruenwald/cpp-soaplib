#pragma once

#include <memory>
#include <string>

#include <soaplib/xml/document.hpp>

namespace soaplib {

class SoapClient
{
public:
	SoapClient(
		const std::string& serviceAddress,
		int timeoutSeconds = 5);

	~SoapClient();

    void EnableLogging(
        bool enable);

    void SetReadTimeout(
        int timeoutSeconds);

	std::unique_ptr<xml::Document> Send(
		const xml::Document& request);

    std::unique_ptr<xml::Document> Send(
        const xml::Document& request,
        int timeoutSeconds);

private:
	void extractAddressParts(
		const std::string& serviceAddress);

private:
	std::string host_;
	int port_;
	std::string path_;
	int timeout_;	// seconds
    bool logging_ = false;

};

} // namespace soaplib
