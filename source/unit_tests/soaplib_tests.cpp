#include "unit_tests.hpp"
#include <soaplib/SoapBase.hpp>
#include <soaplib/SoapServer.hpp>
#include <soaplib/HttpSoapServer.hpp>
#include <soaplib/SoapTransport.hpp>
#include <soaplib/soapService.hpp>
#include <soaplib/xml/xml.hpp>
#include <httplib.h>
#include <future>
#include <thread>
#include <chrono>

namespace soaplib {

class MockSoapTransport : public SoapTransport {
public:
    std::unique_ptr<xml::Document> Send(const xml::Document& /*request*/, int /*timeoutSeconds*/) override {
        auto doc = std::make_unique<xml::Document>();
        auto body = doc->CreateRootNode("Envelope").AddChild("Body");
        body.AddChild("TestResponse");
        return doc;
    }
    void SetReadTimeout(int /*timeoutSeconds*/) override {}
    void EnableLogging(bool /*enable*/) override {}
};

class TestSoapService : public SoapService {
public:
    TestSoapService(std::unique_ptr<SoapTransport> transport)
        : SoapService(std::move(transport), "http://tempuri.org/") {}
    
    std::unique_ptr<xml::Document> TestCall(const xml::Document& request) {
        return Call(request);
    }
};

class TestSoapBase : public SoapBase {
public:
    using SoapBase::CreateEnvelope;
    using SoapBase::AddNamespace;
    using SoapBase::AddChild;
    using SoapBase::ns;
};

class MockSoapServer : public SoapServer {
public:
    std::unique_ptr<xml::Document> HandleRequest(const xml::Document& /*request*/) override {
        auto response = std::make_unique<xml::Document>();
        CreateEnvelope(*response, "http://tempuri.org/TestResponse");
        return response;
    }
};

TEST_CASE("SoapBase: CreateEnvelope", "[soaplib][SoapBase]") {
    TestSoapBase base;
    xml::Document doc;
    auto body = base.CreateEnvelope(doc, "http://tempuri.org/TestAction");

    REQUIRE(std::string(doc.GetRootNode().GetName()) == "Envelope");
    
    // Check for Body child
    auto root = doc.GetRootNode();
    auto bodyNodes = root.GetChildren("Body");
    REQUIRE(bodyNodes.size() == 1);
    
    // Check for Header and SoapAction
    auto headerNodes = root.GetChildren("Header");
    REQUIRE(headerNodes.size() == 1);
    
    auto actionNodes = headerNodes[0].GetChildren("Action");
    REQUIRE(actionNodes.size() == 1);
    REQUIRE(actionNodes[0].GetStringVal() == "http://tempuri.org/TestAction");
}

TEST_CASE("SoapServer: HandleRequest", "[soaplib][SoapServer]") {
    MockSoapServer server;
    xml::Document request;
    auto response = server.HandleRequest(request);
    
    REQUIRE(response != nullptr);
    REQUIRE(std::string(response->GetRootNode().GetName()) == "Envelope");
}

TEST_CASE("HttpSoapServer: Basic Request", "[soaplib][transport][http]") {
    MockSoapServer soapLogic;
    HttpSoapServer server(soapLogic, "/soap");
    
    // Start server in a separate thread
    std::promise<void> serverStarted;
    auto serverFuture = std::async(std::launch::async, [&]() {
        serverStarted.set_value();
        server.Listen("localhost", 8080);
    });

    serverStarted.get_future().wait();
    std::this_thread::sleep_for(std::chrono::milliseconds(200)); // Give it a moment to bind

    // Send a request using httplib::Client
    httplib::Client cli("localhost", 8080);
    
    std::string soapRequest = R"(<?xml version="1.0" encoding="utf-8"?>
<s:Envelope xmlns:s="http://www.w3.org/2003/05/soap-envelope">
  <s:Body>
    <TestAction xmlns="http://tempuri.org/"/>
  </s:Body>
</s:Envelope>)";

    auto res = cli.Post("/soap", soapRequest, "application/soap+xml");
    
    REQUIRE(res != nullptr);
    REQUIRE(res->status == 200);
    REQUIRE(res->body.find("Envelope") != std::string::npos);
    REQUIRE(res->body.find("TestResponse") != std::string::npos);

    server.Stop();
    serverFuture.wait();
}

TEST_CASE("SoapService: Custom Transport", "[soaplib][client][transport]") {
    auto transport = std::make_unique<MockSoapTransport>();
    TestSoapService service(std::move(transport));
    
    xml::Document request;
    auto response = service.TestCall(request);
    
    REQUIRE(response != nullptr);
    REQUIRE(std::string(response->GetRootNode().GetName()) == "Envelope");
    REQUIRE(response->GetRootNode().GetChild("Body").GetChildren("TestResponse").size() == 1);
}

} // namespace soaplib
