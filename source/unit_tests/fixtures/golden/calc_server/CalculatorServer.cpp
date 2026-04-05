// CalculatorServer

#include "CalculatorServer.hpp"

#include <soaplib/SoapException.hpp>
#include <soaplib/ParseHelper.hpp>

namespace calc {

using namespace ::soaplib;

CalculatorServer::CalculatorServer()
{
SetSoapVersion(soaplib::SoapVersion::Soap11);    RegisterUnderstoodHeader("Action", "http://www.w3.org/2005/08/addressing");
    RegisterUnderstoodHeader("To", "http://www.w3.org/2005/08/addressing");
    RegisterUnderstoodHeader("MessageID", "http://www.w3.org/2005/08/addressing");
    RegisterUnderstoodHeader("ReplyTo", "http://www.w3.org/2005/08/addressing");
}

CalculatorServer::~CalculatorServer()
{
}

std::unique_ptr<soaplib::xml::Document> CalculatorServer::HandleRequest(
    const soaplib::xml::Document& request)
{
    try {
        auto envelope = request.GetRootNode();
        
        // Validate headers before dispatching
        ValidateHeaders(envelope);

        auto body = envelope.GetChild("Body");
        auto operationNode = body.GetChildren()[0];
        std::string operationName = operationNode.GetName();

if (operationName == "Add")
        {
            // Parse input
            auto input = ::calc::AddFromXml(operationNode);
            
            // Call implementation
auto result = Add(input);

            // Create response
            auto response = std::make_unique<soaplib::xml::Document>();
            auto responseBody = CreateEnvelope(*response, "");
            
AddResponseToXml(result, *response, responseBody, true);
            
            return response;
        }
else if (operationName == "Subtract")
        {
            // Parse input
            auto input = ::calc::SubtractFromXml(operationNode);
            
            // Call implementation
auto result = Subtract(input);

            // Create response
            auto response = std::make_unique<soaplib::xml::Document>();
            auto responseBody = CreateEnvelope(*response, "");
            
SubtractResponseToXml(result, *response, responseBody, true);
            
            return response;
        }
else if (operationName == "Multiply")
        {
            // Parse input
            auto input = ::calc::MultiplyFromXml(operationNode);
            
            // Call implementation
auto result = Multiply(input);

            // Create response
            auto response = std::make_unique<soaplib::xml::Document>();
            auto responseBody = CreateEnvelope(*response, "");
            
MultiplyResponseToXml(result, *response, responseBody, true);
            
            return response;
        }
else if (operationName == "Divide")
        {
            // Parse input
            auto input = ::calc::DivideFromXml(operationNode);
            
            // Call implementation
auto result = Divide(input);

            // Create response
            auto response = std::make_unique<soaplib::xml::Document>();
            auto responseBody = CreateEnvelope(*response, "");
            
DivideResponseToXml(result, *response, responseBody, true);
            
            return response;
        }

        throw soaplib::SoapException("Unknown operation: " + operationName);
    } catch (const std::exception& e) {
        auto response = std::make_unique<soaplib::xml::Document>();
        auto body = CreateEnvelope(*response, "");
        auto faultNode = AddChild(*response, body, "Fault", "s");
        
        soaplib::SoapFault fault;
        fault.Code = soaplib::FaultCode::Receiver;
        fault.AddReason(e.what());
        
        soaplib::SoapFaultToXml(*response, faultNode, fault, version_);
        return response;
    } catch (...) {
        auto response = std::make_unique<soaplib::xml::Document>();
        auto body = CreateEnvelope(*response, "");
        auto faultNode = AddChild(*response, body, "Fault", "s");
        
        soaplib::SoapFault fault;
        fault.Code = soaplib::FaultCode::Receiver;
        fault.AddReason("Unknown internal error");
        
        soaplib::SoapFaultToXml(*response, faultNode, fault, version_);
        return response;
    }
}

} // namespace calc
