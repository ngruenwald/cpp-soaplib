// CalculatorServer

#include "CalculatorServer.hpp"

#include <soaplib/soapException.hpp>
#include <soaplib/parseHelper.hpp>

namespace calc {

using namespace ::soaplib;

CalculatorServer::CalculatorServer()
{
}

CalculatorServer::~CalculatorServer()
{
}

std::unique_ptr<soaplib::xml::Document> CalculatorServer::HandleRequest(
    const soaplib::xml::Document& request)
{
    auto envelope = request.GetRootNode();
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
}

} // namespace calc
