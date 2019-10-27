// IAuthenticationService.cpp
#include "IAuthenticationService.h"

IAuthenticationService::IAuthenticationService(const std::string& serviceAddress)
	: soaplib::SoapService(
		serviceAddress,
		"http://schemas.datacontract.org/2004/07/Eurofunk.Idds.Administration.Authentication.DC"
	  )
{
}

IAuthenticationService::~IAuthenticationService()
{
}
//
//EchoResponse IAuthenticationService::Echo(const Echo& input)
//{
//	return {}; // TODO
//}
//
//LogOnBasicResponse IAuthenticationService::LogOnBasic(const LogOnBasic& input)
//{
//	return {}; // TODO
//}
//
//LogOnLdapResponse IAuthenticationService::LogOnLdap(const LogOnLdap& input)
//{
//	return {}; // TODO
//}
//
//LogOffResponse IAuthenticationService::LogOff(const LogOff& input)
//{
//	return {}; // TODO
//}

GetPublicKeyResponse IAuthenticationService::GetPublicKey(const ::GetPublicKey& input)
{
	xml::Document rq;
	auto bd = CreateEnvelope(rq, "GetPublicKey");
	AddChild(rq, bd, "GetPublicKey", "t");
	auto rs = Call(rq);
	auto xp = rs->GetRootNode().GetChild("Body").GetChild("GetPublicKeyResponse");
	return GetPublicKeyResponse::fromXml(xp);
}
//
//RefreshTokenResponse IAuthenticationService::RefreshToken(const RefreshToken& input)
//{
//	return {}; // TODO
//}
//
//GetVersionInformationResponse IAuthenticationService::GetVersionInformation(const GetVersionInformation& input)
//{
//	return {}; // TODO
//}
//
//GetDbVersionInformationResponse IAuthenticationService::GetDbVersionInformation(const GetDbVersionInformation& input)
//{
//	return {}; // TODO
//}
//
//GetLogLevelResponse IAuthenticationService::GetLogLevel(const GetLogLevel& input)
//{
//	return {}; // TODO
//}
//
//SetLogLevelResponse IAuthenticationService::SetLogLevel(const SetLogLevel& input)
//{
//	return {}; // TODO
//}
//
//GetLogOnStatResponse IAuthenticationService::GetLogOnStat(const GetLogOnStat& input)
//{
//	return {}; // TODO
//}
//
//GetConfiguredLdapCodesResponse IAuthenticationService::GetConfiguredLdapCodes(const GetConfiguredLdapCodes& input)
//{
//	return {}; // TODO
//}
