#pragma once

#include <stdexcept>

namespace soaplib {

class SoapException
    : public std::runtime_error
{
public:
    SoapException(
        const std::string& message);

};

} // namespace soaplib
