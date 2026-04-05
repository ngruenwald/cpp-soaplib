#pragma once

#include <stdexcept>
#include <string>
#include <soaplib/SoapFault.hpp>

namespace soaplib {

/// Base exception for SOAP errors.
class SoapException : public std::runtime_error
{
public:
    explicit SoapException(
        const std::string& message);
};

/// Exception that carries a structured SOAP 1.2 Fault.
class SoapFaultException : public SoapException
{
public:
    explicit SoapFaultException(const SoapFault& fault)
        : SoapException("SOAP Fault: " + (fault.Reasons.empty() ? fault.GetCodeString() : fault.Reasons[0].Text))
        , fault_(fault)
    {}

    const SoapFault& GetFault() const { return fault_; }

private:
    SoapFault fault_;
};

} // namespace soaplib
