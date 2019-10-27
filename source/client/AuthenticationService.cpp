#include "AuthenticationService.hpp"

#include <iostream>

#include "encrypt.hpp"

AuthenticationService::AuthenticationService(
	const std::string& serviceAddress)
	: SoapService(
		serviceAddress,
		//ServiceNamespace
		"http://schemas.datacontract.org/2004/07/Eurofunk.Idds.Administration.Authentication.DC"
	)
{
}
/*
void print_tree(
	const pugi::xml_node& node,
	int indent = 0)
{
	for (const auto& child : node.children())
	{
		for (int i = 0; i < indent; i++)
		{
			std::cout << ' ';
		}

		std::cout << child.name() << '\n';

		print_tree(child, indent + 2);
	}
}
*/
std::string AuthenticationService::GetPublicKey()
{
	const std::string name = "GetPublicKey";

	xml::Document request;
	auto body = CreateEnvelope(request, name);
	AddChild(request, body, name, "t");

	auto response = Call(request);

	auto envelope = response->GetRootNode();

	return envelope
		.GetChild("Body")
		.GetChild("GetPublicKeyResponse")
		.GetChild("GetPublicKeyResult")
		.GetChild("PublicKey")
		.GetStringVal();
}

void AuthenticationService::LogOnBasic(
	const std::string& username,
	const std::string& encryptedPassword)
{
	/*
	const std::string name = "LogOnBasic";

	pugi::xml_document request;
	auto body = CreateEnvelope(request, name);

	auto logOnBasic = body.append_child(name.c_str());
	logOnBasic.append_attribute("xmlns").set_value(TempNamespace.c_str());

	auto logonRequest = logOnBasic.append_child("logonRequest");
	logonRequest.append_attribute("xmlns:b").set_value(namespace_.c_str());
	logonRequest.append_attribute("xmlns:i").set_value(InstNamespace.c_str());

	logonRequest.append_child(ns("b", "EncodedPassword").c_str()).append_child(pugi::node_pcdata).set_value(encryptedPassword.c_str());
	logonRequest.append_child(ns("b", "RequestingApp").c_str()).append_child(pugi::node_pcdata).set_value("soaplib");
	logonRequest.append_child(ns("b", "UserAgent").c_str()).append_child(pugi::node_pcdata).set_value("soaplib/0.0.1");
	logonRequest.append_child(ns("b", "UserName").c_str()).append_child(pugi::node_pcdata).set_value(username.c_str());

	pugi::xml_document response = Call(request);

	auto xn = response.select_node("/s:Envelope/s:Body/LogOnBasicResponse/LogOnBasicResult/SecurityToken");
	auto nn = xn.node().first_child();

	token_ = nn.value();
	*/
}

void AuthenticationService::LogOff()
{/*
	<xs:element name = "LogOff">
		<xs:complexType>
		<xs:sequence>
		<xs:element minOccurs = "0" name = "header" nillable = "true" type = "q5:DCHeader" xmlns : q5 = "http://schemas.datacontract.org/2004/07/Eurofunk.Idds.Administration.Common.DataContract" / >
		<xs:element minOccurs = "0" name = "logoffRequest" nillable = "true" type = "q6:LogOffDCRequest" xmlns : q6 = "http://schemas.datacontract.org/2004/07/Eurofunk.Idds.Administration.Authentication.DC" / >
		< / xs:sequence>
		< / xs:complexType>
		< / xs:element>
		*/
	/*
	const std::string name = "LogOff";

	pugi::xml_document request;
	auto body = CreateEnvelope(request, name);

	auto logOff = body.append_child(name.c_str());
	logOff.append_attribute("xmlns").set_value(TempNamespace.c_str());

	auto header = logOff.append_child("header");
	header.append_attribute("xmlns:q5").set_value("http://schemas.datacontract.org/2004/07/Eurofunk.Idds.Administration.Common.DataContract");

	auto securityToken = header.append_child(ns("q5", "SecurityToken").c_str());
	securityToken.append_child(pugi::node_pcdata).set_value(Token().c_str());

	auto logoffRequest = logOff.append_child("logoffRequest");
	logoffRequest.append_attribute("xmlns:q6").set_value(namespace_.c_str());

	auto logOffDCRequest = logoffRequest.append_child("LogOffDCRequest");
	logOffDCRequest.append_attribute("xmlns:q5").set_value("http://schemas.datacontract.org/2004/07/Eurofunk.Idds.Administration.Authentication.DC.Base");

	pugi::xml_document response = Call(request);

	token_.clear();
	*/
}


void AuthenticationService::Login(
	const std::string& username,
	const std::string& password)
{
	/*
	auto publicKey = GetPublicKey();

	pugi::xml_document doc;
	doc.load_string(publicKey.c_str());

	auto modulus = doc.select_node("/RSAKeyValue/Modulus").node().first_child().value();
	auto exponent = doc.select_node("RSAKeyValue/Exponent").node().first_child().value();

	std::string encryptedPassword = encrypt(modulus, exponent, password);

	LogOnBasic(username, encryptedPassword);
	*/
}
