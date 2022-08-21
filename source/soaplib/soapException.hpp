#pragma once

#include <stdexcept>

namespace soaplib {

/// SOAP Exception
class SoapException
    : public std::runtime_error
{
public:
    /// Initializes a SoapException instance.
    /// @param[in] message Error message
    SoapException(
        const std::string& message);

};

} // namespace soaplib
