#include "unit_tests.hpp"
#include <soaplib/SoapBase.hpp>
#include <soaplib/SoapServer.hpp>
#include <soaplib/HttpSoapServer.hpp>
#include <soaplib/SoapTransport.hpp>
#include <soaplib/HttpSoapTransport.hpp>
#include <soaplib/WebSocketSoapTransport.hpp>
#include <soaplib/WebSocketSoapServer.hpp>
#include <soaplib/SoapFault.hpp>
#include <soaplib/soapException.hpp>
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
        try {
            auto response = std::make_unique<xml::Document>();
            CreateEnvelope(*response, "http://tempuri.org/TestResponse");
            return response;
        } catch (const std::exception& e) {
            auto response = std::make_unique<xml::Document>();
            auto body = CreateEnvelope(*response, "");
            auto faultNode = AddChild(*response, body, "Fault", "s");
            SoapFault fault;
            fault.Code = FaultCode::Receiver;
            fault.AddReason(e.what());
            SoapFaultToXml(*response, faultNode, fault);
            return response;
        }
    }
};

class FailingMockSoapServer : public SoapServer {
public:
    std::unique_ptr<xml::Document> HandleRequest(const xml::Document& /*request*/) override {
        try {
            throw std::runtime_error("Server crashed");
        } catch (const std::exception& e) {
            auto response = std::make_unique<xml::Document>();
            auto body = CreateEnvelope(*response, "");
            auto faultNode = AddChild(*response, body, "Fault", "s");
            SoapFault fault;
            fault.Code = FaultCode::Receiver;
            fault.AddReason(e.what());
            SoapFaultToXml(*response, faultNode, fault);
            return response;
        }
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
    REQUIRE(response->GetRootNode().GetChild("Body").IsValid());
}

TEST_CASE("WebSocketSoapServer/Transport: Basic Request", "[soaplib][transport][ws]") {
    MockSoapServer soapLogic;
    WebSocketSoapServer server(soapLogic, "/ws");
    
    // Start server in a separate thread
    std::thread serverThread([&]() {
        server.Listen("localhost", 8081);
    });

    int retry = 0;
    while (!server.IsRunning() && retry < 100) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        retry++;
    }

    WebSocketSoapTransport transport("ws://localhost:8081/ws");
    transport.EnableLogging(false);
    
    xml::Document request;
    request.CreateRootNode("Envelope").AddChild("Body").AddChild("TestAction");
    
    auto response = transport.Send(request, 5);
    
    REQUIRE(response != nullptr);
    REQUIRE(std::string(response->GetRootNode().GetName()) == "Envelope");
    REQUIRE(response->GetRootNode().GetChild("Body").IsValid());

    transport.Close();
    server.Stop();
    if (serverThread.joinable()) serverThread.join();
}

TEST_CASE("SoapFault: Serialization", "[soaplib][fault]") {
    SoapFault fault;
    fault.Code = FaultCode::Sender;
    fault.AddReason("Invalid request", "en");
    fault.AddReason("Requête invalide", "fr");
    fault.Detail = "<MyError>Some detail</MyError>";

    xml::Document doc;
    TestSoapBase base;
    auto body = base.CreateEnvelope(doc, "");
    auto faultNode = base.AddChild(doc, body, "Fault", "s");
    
    SoapFaultToXml(doc, faultNode, fault);

    auto codeNode = faultNode.GetChild("Code");
    REQUIRE(codeNode.GetChild("Value").GetStringVal().find("Sender") != std::string::npos);
    
    auto reasonNodes = faultNode.GetChild("Reason").GetChildren("Text");
    REQUIRE(reasonNodes.size() == 2);
    REQUIRE(reasonNodes[0].GetStringVal() == "Invalid request");
    REQUIRE(reasonNodes[1].GetStringVal() == "Requête invalide");

    // Round-trip
    SoapFault fault2;
    SoapFaultFromXml(faultNode, fault2);
    REQUIRE(fault2.Code == FaultCode::Sender);
    REQUIRE(fault2.Reasons.size() == 2);
    REQUIRE(fault2.Reasons[0].Text == "Invalid request");
    REQUIRE(fault2.Reasons[0].Language == "en");
    REQUIRE(fault2.Detail.find("Some detail") != std::string::npos);
}

TEST_CASE("SOAP Fault: End-to-End", "[soaplib][fault][http]") {
    FailingMockSoapServer soapLogic;
    HttpSoapServer server(soapLogic, "/faulty");
    
    std::thread serverThread([&]() {
        server.Listen("localhost", 8082);
    });

    // Wait for server
    int retry = 0;
    while (!server.IsRunning() && retry < 100) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        retry++;
    }

    HttpSoapTransport transport("http://localhost:8082/faulty");
    
    xml::Document request;
    request.CreateRootNode("Envelope").AddChild("Body").AddChild("FailAction");
    
    REQUIRE_THROWS_AS(transport.Send(request, 5), SoapFaultException);
    
    try {
        transport.Send(request, 5);
    } catch (const SoapFaultException& e) {
        REQUIRE(e.GetFault().Code == FaultCode::Receiver);
        REQUIRE(e.GetFault().Reasons[0].Text == "Server crashed");
    }

    server.Stop();
    if (serverThread.joinable()) serverThread.join();
}

} // namespace soaplib
