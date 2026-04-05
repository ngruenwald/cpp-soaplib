#pragma once

#include <stdexcept>

namespace soaplib {
namespace xml {

/// XML exception
class Exception
    : public std::runtime_error
{
public:
    /// Constructs XML exception
    /// @param[in] message Error message
    explicit Exception(
        const char* message)
        : std::runtime_error{message}
    {
    }

    /// Constructs XML exception
    /// @param[in] message Error message
    explicit Exception(
        const std::string& message)
        : std::runtime_error{message}
    {
    }
};

} // namespace xml
} // namespace soaplib
