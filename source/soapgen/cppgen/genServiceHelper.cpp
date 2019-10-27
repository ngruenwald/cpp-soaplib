#include "genServiceHelper.hpp"

namespace cppgen {
namespace service {

const Binding* getBinding(
    const Name& portBinding,
    const Definition& definition)
{
    for (const auto& binding : definition.bindings)
    {
        if (binding.name.name == portBinding.name)
        {
            return &binding;
        }
    }

    return nullptr;
}

const PortType* getPortType(
    const Name& bindingType,
    const Definition& definition)
{
    for (const auto& portType : definition.portTypes)
    {
        if (portType.name.name == bindingType.name)
        {
            return &portType;
        }
    }

    return nullptr;
}

const std::vector<Name> getMessagePartNames(
    const Name& messageName,
    const Definition& definition)
{
    std::vector<Name> parts;

    for (const auto& message : definition.messages)
    {
        if (message.name.name == messageName.name)
        {
            for (const auto& part : message.parts)
            {
                parts.push_back(part.element);
            }
        }
    }

    return parts;
}

} // namespace service
} // namespace cppgen
