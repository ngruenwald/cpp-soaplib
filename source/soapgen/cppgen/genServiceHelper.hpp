#pragma once

#include <vector>

#include "../wsdl.hpp"

namespace cppgen {
namespace service {

const Binding* getBinding(
    const Name& portBinding,
    const Definition& definition);

const PortType* getPortType(
    const Name& bindingType,
    const Definition& definition);

const std::vector<Name> getMessagePartNames(
    const Name& messageName,
    const Definition& definition);

} // namespace service
} // namespace cppgen
