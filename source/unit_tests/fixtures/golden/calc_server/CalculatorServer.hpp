// CalculatorServer

#pragma once

#include <soaplib/SoapServer.hpp>

#include "types/Add.hpp"
#include "types/AddResponse.hpp"
#include "types/Subtract.hpp"
#include "types/SubtractResponse.hpp"
#include "types/Multiply.hpp"
#include "types/MultiplyResponse.hpp"
#include "types/Divide.hpp"
#include "types/DivideResponse.hpp"

namespace calc {

/// The "Calculator" SOAP service server stub
class CalculatorServer
    : public soaplib::SoapServer
{
public:
    /// Constructs a new instance of the "Calculator" service server stub.
    CalculatorServer();

    /// Destructs the service instance.
    virtual ~CalculatorServer();

public:
    /// Handles a SOAP request.
    /// @param[in] request The request XML document
    /// @returns The response XML document
    std::unique_ptr<soaplib::xml::Document> HandleRequest(
        const soaplib::xml::Document& request) override;

protected:
    /// Implementation of the "Add" operation.
    virtual ::calc::AddResponse Add(
        const ::calc::Add& input) = 0;

    /// Implementation of the "Subtract" operation.
    virtual ::calc::SubtractResponse Subtract(
        const ::calc::Subtract& input) = 0;

    /// Implementation of the "Multiply" operation.
    virtual ::calc::MultiplyResponse Multiply(
        const ::calc::Multiply& input) = 0;

    /// Implementation of the "Divide" operation.
    virtual ::calc::DivideResponse Divide(
        const ::calc::Divide& input) = 0;

};

} // namespace calc
