// Calculator

#include "Calculator.hpp"

#include <soaplib/parseHelper.hpp>

namespace calc {

using namespace ::soaplib;

Calculator::Calculator(
    const std::string& serviceAddress,
    const soaplib::HttpConfig& config)
    : soaplib::SoapService(serviceAddress, "", config)
{
SetSoapVersion(soaplib::SoapVersion::Soap11);    RegisterUnderstoodHeader("Action", "http://www.w3.org/2005/08/addressing");
    RegisterUnderstoodHeader("To", "http://www.w3.org/2005/08/addressing");
    RegisterUnderstoodHeader("MessageID", "http://www.w3.org/2005/08/addressing");
    RegisterUnderstoodHeader("ReplyTo", "http://www.w3.org/2005/08/addressing");
}

Calculator::~Calculator()
{
}

::calc::AddResponse Calculator::Add(
    const ::calc::Add& input)
{
    soaplib::xml::Document request;
    auto body = CreateEnvelope(request, "");
    AddToXml(input, request, body, true);

    auto response = Call(request, "", soaplib::HttpMethod::Post);
    auto envelope = response->GetRootNode();
    auto operation = envelope.GetChild("Body").GetChild("AddResponse");

    return ::calc::AddResponseFromXml(operation);
}

::calc::SubtractResponse Calculator::Subtract(
    const ::calc::Subtract& input)
{
    soaplib::xml::Document request;
    auto body = CreateEnvelope(request, "");
    SubtractToXml(input, request, body, true);

    auto response = Call(request, "", soaplib::HttpMethod::Post);
    auto envelope = response->GetRootNode();
    auto operation = envelope.GetChild("Body").GetChild("SubtractResponse");

    return ::calc::SubtractResponseFromXml(operation);
}

::calc::MultiplyResponse Calculator::Multiply(
    const ::calc::Multiply& input)
{
    soaplib::xml::Document request;
    auto body = CreateEnvelope(request, "");
    MultiplyToXml(input, request, body, true);

    auto response = Call(request, "", soaplib::HttpMethod::Post);
    auto envelope = response->GetRootNode();
    auto operation = envelope.GetChild("Body").GetChild("MultiplyResponse");

    return ::calc::MultiplyResponseFromXml(operation);
}

::calc::DivideResponse Calculator::Divide(
    const ::calc::Divide& input)
{
    soaplib::xml::Document request;
    auto body = CreateEnvelope(request, "");
    DivideToXml(input, request, body, true);

    auto response = Call(request, "", soaplib::HttpMethod::Post);
    auto envelope = response->GetRootNode();
    auto operation = envelope.GetChild("Body").GetChild("DivideResponse");

    return ::calc::DivideResponseFromXml(operation);
}


} // namespace calc
