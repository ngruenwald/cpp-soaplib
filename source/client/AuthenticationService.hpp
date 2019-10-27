#pragma once

#include <soaplib/soapService.hpp>

class AuthenticationService
	: public soaplib::SoapService
{
	const char* ServiceNamespace = "http://schemas.datacontract.org/2004/07/Eurofunk.Idds.Administration.Authentication.DC";

public:
	// http://192.168.68.7:8091/SR_1_17/AWS/AuthenticationService.svc
	AuthenticationService(
		const std::string& serviceAddress);

	std::string GetPublicKey();

	void LogOnBasic(
		const std::string& username,
		const std::string& password);

	void LogOff();

public:
	void Login(
		const std::string& username,
		const std::string& password);

public:
	const std::string& Token() const
	{
		return token_;
	}

private:
	std::string token_;
};
