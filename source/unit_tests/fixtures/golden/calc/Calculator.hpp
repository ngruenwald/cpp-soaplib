// Calculator

#pragma once

#include <soaplib/soapService.hpp>

#include "types/Add.hpp"
#include "types/AddResponse.hpp"
#include "types/Subtract.hpp"
#include "types/SubtractResponse.hpp"
#include "types/Multiply.hpp"
#include "types/MultiplyResponse.hpp"
#include "types/Divide.hpp"
#include "types/DivideResponse.hpp"

namespace calc {

/// The "Calculator" SOAP service
class Calculator
    : public soaplib::SoapService
{
public:
    /// Constructs a new instance of the "Calculator" service.
    /// @param[in] serviceAddress URL of the SOAP service
    /// @param[in] config Optional HTTP configuration
    Calculator(
        const std::string& serviceAddress,
        const soaplib::HttpConfig& config = soaplib::HttpConfig());

    /// Destructs the service instance.
    ~Calculator();

public:
    ::calc::AddResponse Add(
        const ::calc::Add& input);

    ::calc::SubtractResponse Subtract(
        const ::calc::Subtract& input);

    ::calc::MultiplyResponse Multiply(
        const ::calc::Multiply& input);

    ::calc::DivideResponse Divide(
        const ::calc::Divide& input);

};

} // namespace calc
