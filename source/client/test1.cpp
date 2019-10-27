
#include <soaplib/uuid.hpp>

const char* testMessage1 = R"eom(
<s:Envelope xmlns:s="http://www.w3.org/2003/05/soap-envelope" xmlns:a="http://www.w3.org/2005/08/addressing">
    <s:Header>
        <a:Action s:mustUnderstand="1">http://tempuri.org/IAuthenticationService/GetPublicKey</a:Action>
        <a:MessageID>urn:uuid:b434a379-4dc8-457a-98b1-08758d66d6ee</a:MessageID>
        <a:ReplyTo>
            <a:Address>http://www.w3.org/2005/08/addressing/anonymous</a:Address>
        </a:ReplyTo>
        <a:To s:mustUnderstand="1">http://192.168.68.7:8091/SR_1_17/AWS/AuthenticationService.svc</a:To>
    </s:Header>
    <s:Body>
        <GetPublicKey xmlns="http://tempuri.org/"/>
    </s:Body>
</s:Envelope>
)eom";

const char* testMessage2 = R"eom(
<soap:Envelope
  xmlns:soap="http://www.w3.org/2003/05/soap-envelope"
  xmlns:tem="http://tempuri.org/"
  xmlns:eur="http://schemas.datacontract.org/2004/07/Eurofunk.Idds.Administration.Authentication.DC">
   <soap:Header/>
   <soap:Body>
      <tem:LogOnBasic>
         <!--Optional:-->
         <tem:logonRequest>
            <!--Optional:-->
            <eur:EncodedPassword>pwd</eur:EncodedPassword>
            <!--Optional:-->
            <eur:RequestingApp>tt</eur:RequestingApp>
            <!--Optional:-->
            <eur:UserAgent>soapUI</eur:UserAgent>
            <!--Optional:-->
            <eur:UserName>admin</eur:UserName>
         </tem:logonRequest>
      </tem:LogOnBasic>
   </soap:Body>
</soap:Envelope>
)eom";

const char* testMessage3 = R"eom(
<soap:Envelope
  xmlns:soap="http://www.w3.org/2003/05/soap-envelope">
  <soap:Header>
  </soap:Header
  <soap:Body>
    <GetPublicKey xmlns="http://tempuri.org/"/>
  </soap:Body>
</soap:Envelope>
)eom";





// create envelope and return body node
pugi::xml_node createEnvelope(
	pugi::xml_document& doc,
	const std::string& service,
	const std::string& action)
{
	auto envelope = doc.append_child("s:Envelope");
	envelope.append_attribute("xmlns:s").set_value("http://www.w3.org/2003/05/soap-envelope");
	envelope.append_attribute("xmlns:t").set_value("http://tempuri.org/");
	envelope.append_attribute("xmlns:e").set_value("http://schemas.datacontract.org/2004/07/Eurofunk.Idds.Administration.Authentication.DC");
	envelope.append_attribute("xmlns:a").set_value("http://www.w3.org/2005/08/addressing");

	//
	// header
	//

	auto header = envelope.append_child("s:Header");

	auto headerActionValue = "http://tempuri.org/" + service + "/" + action;
	auto headerAction = header.append_child("a:Action");
	headerAction.append_attribute("s:mustUnderstand").set_value("1");
	headerAction.append_child(pugi::node_pcdata).set_value(headerActionValue.c_str());

	auto urn = "urn:uuid:" + util::uuid::generate_string();
	auto messageID = header.append_child("a:MessageID");
	messageID.append_child(pugi::node_pcdata).set_value(urn.c_str());

	auto replyTo = header.append_child("a:ReplyTo");
	auto replyToAddress = replyTo.append_child("a:Address");
	replyToAddress.append_child(pugi::node_pcdata).set_value("http://www.w3.org/2005/08/addressing/anonymous");

	auto to = header.append_child("a:To");
	to.append_attribute("s:mustUnderstand").set_value("1");
	to.append_child(pugi::node_pcdata).set_value("http://192.168.68.7:8091/SR_1_17/AWS/AuthenticationService.svc");

	//
	// body
	//

	auto body = envelope.append_child("s:Body");
	body.append_child(action.c_str()).append_attribute("xmlns").set_value("http://tempuri.org/");

	return body;
}

void test1()
{
	try
	{
		soaplib::SoapClient client("http://192.168.68.7:8091/SR_1_17/AWS/AuthenticationService.svc");

		for (bool run = true; run; )
		{
			try
			{

				std::string command;
				std::cin >> command;

				if (command == "quit")
				{
					run = false;
				}
				else if (command == "raw")
				{
					auto response = client.SendXml(testMessage3);
					std::cout << "\n\n" << response << '\n';
				}
				else if (command == "xml")
				{
					pugi::xml_document doc;
					auto body = createEnvelope(doc, "IAuthenticationService", "GetPublicKey");

					//body.append_child("tem:GetPublicKey");

					std::ostringstream iss;
					doc.save(iss);

					client.SendXml(iss.str());
				}
			}
			catch (const std::exception& ex)
			{
				std::cerr << ex.what() << '\n';
			}
		}
	}
	catch (const std::exception& ex)
	{
		std::cerr << ex.what() << '\n';
	}
}
