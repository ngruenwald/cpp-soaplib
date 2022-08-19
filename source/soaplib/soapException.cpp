#include "soapException.hpp"

namespace soaplib {

SoapException::SoapException(
    const std::string& message)
    : std::runtime_error(message)
{
}

} // namespace soaplib
