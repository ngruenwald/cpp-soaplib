// IAuthenticationService.h
#pragma once

#include <soaplib/soapService.hpp>

//#include "IAuthenticationService_Echo.h"
//#include "IAuthenticationService_LogOnBasic.h"
//#include "IAuthenticationService_LogOnLdap.h"
//#include "IAuthenticationService_LogOff.h"
#include "IAuthenticationService_GetPublicKey.h"
//#include "IAuthenticationService_RefreshToken.h"
//#include "IAuthenticationService_GetVersionInformation.h"
//#include "IAuthenticationService_GetDbVersionInformation.h"
//#include "IAuthenticationService_GetLogLevel.h"
//#include "IAuthenticationService_SetLogLevel.h"
//#include "IAuthenticationService_GetLogOnStat.h"
//#include "IAuthenticationService_GetConfiguredLdapCodes.h"

class IAuthenticationService
	: public soaplib::SoapService
{
public:
	IAuthenticationService(const std::string& serviceAddress);
	virtual ~IAuthenticationService();
	//EchoResponse Echo(const Echo& input);
	//LogOnBasicResponse LogOnBasic(const LogOnBasic& input);
	//LogOnLdapResponse LogOnLdap(const LogOnLdap& input);
	//LogOffResponse LogOff(const LogOff& input);
	GetPublicKeyResponse GetPublicKey(const ::GetPublicKey& input);
	//RefreshTokenResponse RefreshToken(const RefreshToken& input);
	//GetVersionInformationResponse GetVersionInformation(const GetVersionInformation& input);
	//GetDbVersionInformationResponse GetDbVersionInformation(const GetDbVersionInformation& input);
	//GetLogLevelResponse GetLogLevel(const GetLogLevel& input);
	//SetLogLevelResponse SetLogLevel(const SetLogLevel& input);
	//GetLogOnStatResponse GetLogOnStat(const GetLogOnStat& input);
	//GetConfiguredLdapCodesResponse GetConfiguredLdapCodes(const GetConfiguredLdapCodes& input);

}; // IAuthenticationService

class MyAuthenticationService
	: public IAuthenticationService
{
public:
	MyAuthenticationService(
		const std::string& serviceAddress)
		: IAuthenticationService(serviceAddress)
	{
	}

	std::string GetPublicKey2()
	{
		::GetPublicKey input;
		::GetPublicKeyResponse output = IAuthenticationService::GetPublicKey(input);
		return output.result.value;
	}
};
