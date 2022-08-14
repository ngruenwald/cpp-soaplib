#pragma once

#include <memory>
#include <string>

#include <soaplib/xml/xml.hpp>

#include <soaplib/soapClient.hpp>

namespace soaplib {

class SoapService
{
protected:
	const std::string SoapNamespace = "http://www.w3.org/2003/05/soap-envelope";
	const std::string TempNamespace = "http://tempuri.org/";
	const std::string AddrNamespace = "http://www.w3.org/2005/08/addressing";
	const std::string InstNamespace = "http://www.w3.org/2001/XMLSchema-instance";

public:
	SoapService(
		const std::string& serviceAddress,
		const std::string& serviceNamespace);

    void EnableHeader(
        bool enable);

    void EnableLogging(
        bool enable);

    void SetRequestTimeout(
        int timeoutSeconds);

protected:
	std::string SoapNS() const { return "s"; }
	std::string TempNS() const { return "t"; }
	std::string AddrNS() const { return "a"; }
	std::string InstNS() const { return "i"; }
	std::string NS()     const { return "e"; }

	static std::string ns(
		const std::string& ns,
		const std::string& name);

protected:
	std::unique_ptr<xml::Document> Call(
		const xml::Document& request);

    std::unique_ptr<xml::Document> Call(
		const xml::Document& request,
        int timeoutSeconds);

	xml::Node CreateEnvelope(
		xml::Document& doc,
		const std::string& soapAction);

	void AddNamespace(
		xml::Document& doc,
		xml::Node& node,
		const std::string& href,
		const std::string& prefix);

	xml::Node AddChild(
		xml::Document& doc,
		xml::Node& parentNode,
		const std::string& name,
		const std::string& prefix);

private:
	void ParseServiceAddress(
		const std::string& serviceAddress);

protected:
	soaplib::SoapClient client_;

	std::string address_;
	std::string namespace_;

	std::string host_;
	std::string path_;
	std::string service_;

    bool enableHeader_{true};
};

} // namespace soaplib
