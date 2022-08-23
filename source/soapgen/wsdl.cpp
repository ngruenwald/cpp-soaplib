#include "wsdl.hpp"

#include <iostream>
#include <limits>

#include "soaplib/xml/xml.hpp"

const xmlChar* bad_cast(const char* s)
{
    return reinterpret_cast<const xmlChar*>(s);
}

const char* bad_cast(const xmlChar* s)
{
    return reinterpret_cast<const char*>(s);
}

void LoadComplexType(
    ExtendedType& type,
    const soaplib::xml::Node& complexTypeNode,
    const std::string& targetNamespace);


std::string getNsPrefix(
    const soaplib::xml::Node& node,
    const std::string& nsFull)
{
    for (auto ns = node.GetXmlNode()->nsDef; ns; ns = ns->next)
    {
        if (xmlStrcmp(ns->href, bad_cast(nsFull.c_str())) == 0)
        {
            if (ns->prefix)
            {
                return bad_cast(ns->prefix);
            }
        }
    }

    for (auto ns = node.GetXmlNode()->ns; ns; ns = ns->next)
    {
        if (xmlStrcmp(ns->href, bad_cast(nsFull.c_str())) == 0)
        {
            if (ns->prefix)
            {
                return bad_cast(ns->prefix);
            }
        }
    }

    return {};
}

std::string getNsHref(
    const soaplib::xml::Node& node,
    const std::string& nsShort)
{
    for (auto ns = node.GetXmlNode()->nsDef; ns; ns = ns->next)
    {
        if (xmlStrcmp(ns->prefix, bad_cast(nsShort.c_str())) == 0)
        {
            return bad_cast(ns->href);
        }
    }

    for (auto ns = node.GetXmlNode()->ns; ns; ns = ns->next)
    {
        if (xmlStrcmp(ns->prefix, bad_cast(nsShort.c_str())) == 0)
        {
            return bad_cast(ns->href);
        }
    }

    return {};
}

Name getName(
    const soaplib::xml::Node& node,
    const char* attributeName,
    const std::string& targetNamespace)
{
    const auto value = node.GetStringProp(attributeName);
    auto idx = value.find(':');

    Name name;

    if (idx == std::string::npos)
    {
        name.name = value;
        name.nsHref = targetNamespace;
        name.nsPrefix = getNsPrefix(node, name.nsHref);
    }
    else if (idx + 2 < value.length() && value.substr(idx, 3) == "://")
    {
        name.name = value;
        name.nsHref = targetNamespace;
        name.nsPrefix = getNsPrefix(node, name.nsHref);
    }
    else
    {
        name.name = value.substr(idx + 1);
        name.nsPrefix = value.substr(0, idx);
        name.nsHref = getNsHref(node, name.nsPrefix);
    }

    if (name.nsHref.empty())
    {
        std::cerr << "could not find namespace for " << value << '\n';
    }

    return name;
}


Input LoadInput(
    const soaplib::xml::Node& inputNode)
{
    Input input;

    try
    {
        input.action = getName(inputNode, "Action", {});
    }
    catch (const std::exception& ex)
    {
        // std::cerr << ex.what() << '\n';
    }

    input.message = getName(inputNode, "message", {});

    return input;
}

Output LoadOutput(
    const soaplib::xml::Node& outputNode)
{
    Output output;

    try
    {
        output.action = getName(outputNode, "Action", {});
    }
    catch (const std::exception& ex)
    {
        // std::cerr << ex.what() << '\n';
    }

    output.message = getName(outputNode, "message", {});

    return output;
}

Operation LoadOperation(
    const soaplib::xml::Node& operationNode)
{
    Operation operation;

    operation.name = getName(operationNode, "name", {});

    auto inputNode = operationNode.GetChild("input");
    operation.input = LoadInput(inputNode);

    try
    {
        auto outputNode = operationNode.GetChild("output");
        operation.output = LoadOutput(outputNode);
    }
    catch(const std::exception& e)
    {
        std::cerr << "operation " << operation.name.name << " has no output" << '\n';
    }

    return operation;
}

Port LoadPort(
    const soaplib::xml::Node& portNode)
{
    Port port;

    port.name = getName(portNode, "name", {});
    port.binding = getName(portNode, "binding", {});

    try
    {
        auto addressNode = portNode.GetChild("address");
        port.address.location = addressNode.GetStringProp("location");
    }
    catch (const soaplib::xml::Exception&)
    {
    }

    try
    {
        auto endpointReferenceNode = portNode.GetChild("EndpointReference");
        auto addressNode = endpointReferenceNode.GetChild("Address");
        port.endpointReference.address = addressNode.GetStringVal();
    }
    catch (const soaplib::xml::Exception&)
    {
    }

    return port;
}

Service LoadService(
    const soaplib::xml::Node& serviceNode)
{
    Service service;

    service.name = getName(serviceNode, "name", {});

    const auto portNodes = serviceNode.GetChildren("port");
    for (const auto& portNode : portNodes)
    {
        service.ports.push_back(LoadPort(portNode));
    }

    return service;
}

Binding LoadBinding(
    const soaplib::xml::Node& bindingNode)
{
    Binding binding;

    binding.name = getName(bindingNode, "name", {});
    binding.type = getName(bindingNode, "type", {});

    // TODO

    return binding;
}

PortType LoadPortType(
    const soaplib::xml::Node& portTypeNode)
{
    PortType portType;

    portType.name = getName(portTypeNode, "name", {});

    const auto operationNodes = portTypeNode.GetChildren("operation");
    for (const auto& operationNode : operationNodes)
    {
        portType.operations.push_back(LoadOperation(operationNode));
    }

    return portType;
}

Message LoadMessage(
    const soaplib::xml::Node& messageNode)
{
    Message message;

    message.name = getName(messageNode, "name", {});

    const auto partNodes = messageNode.GetChildren("part");
    for (const auto& partNode : partNodes)
    {
        Message::Part part;

        part.name = getName(partNode, "name", {});
        part.element = getName(partNode, "element", {});

        message.parts.push_back(part);
    }

    return message;
}

void LoadElement(
    ExtendedType& type,
    const soaplib::xml::Node& elementNode,
    const std::string& targetNamespace)
{
    Parameter parameter;
    parameter.name = getName(elementNode, "name", targetNamespace);

    int minOccurs = 1;
    int maxOccurs = 1;
    bool nillable = false;

    try
    {
        minOccurs = elementNode.GetIntProp("minOccurs");
    }
    catch (const soaplib::xml::Exception&)
    {
    }

    try
    {
        auto s = elementNode.GetStringProp("maxOccurs");
        if (s == "unbounded")
        {
            maxOccurs = std::numeric_limits<int>::max();
        }
        else
        {
            maxOccurs = std::stoi(s);
        }
    }
    catch (const soaplib::xml::Exception&)
    {
    }

    try
    {
        nillable = elementNode.GetBoolProp("nillable");
    }
    catch (const soaplib::xml::Exception&)
    {
    }

    if (maxOccurs > 1)
    {
        parameter.kind = Parameter::Multiple;
    }
    else if (nillable)
    {
        //parameter.kind = Parameter::Pointer;
        parameter.kind = Parameter::Optional;
    }
    else if (minOccurs == 0 && maxOccurs == 1)
    {
        parameter.kind = Parameter::Optional;
    }
    else
    {
        parameter.kind = Parameter::Mandatory;
    }

    try
    {
        parameter.type = getName(elementNode, "type", targetNamespace);
    }
    catch (const soaplib::xml::Exception&)
    {
        const auto childNodes = elementNode.GetChildren(nullptr);
        if (!childNodes.empty())
        {
            for (const auto& childNode : childNodes)
            {
                const std::string name = childNode.GetName();

                if (name == "complexType")
                {
                    auto innerType = std::make_shared<ExtendedType>();
                    innerType->name = parameter.name;
                    LoadComplexType(*innerType, childNode, targetNamespace);
                    type.innerTypes.push_back(innerType);
                    if (parameter.type.name.empty())
                    {
                        parameter.type = innerType->name;
                    }
                }
            }
        }
    }

    type.parameters.push_back(parameter);
}

void LoadSequence(
    ExtendedType& type,
    const soaplib::xml::Node& sequenceNode,
    const std::string& targetNamespace)
{
    const auto elementNodes = sequenceNode.GetChildren("element");
    for (const auto& elementNode : elementNodes)
    {
        LoadElement(type, elementNode, targetNamespace);
    }
}

void LoadExtension(
    ExtendedType& type,
    const soaplib::xml::Node& extensionNode,
    const std::string& targetNamespace)
{
    type.base = getName(extensionNode, "base", targetNamespace);

    const auto childNodes = extensionNode.GetChildren(nullptr);
    for (const auto& childNode : childNodes)
    {
        const std::string name = childNode.GetName();

        if (name == "sequence")
        {
            LoadSequence(type, childNode, targetNamespace);
        }
        else
        {
            std::cerr << "extension::" << name << " not implemented" << '\n';
        }
    }
}

void LoadComplexContent(
    ExtendedType& type,
    const soaplib::xml::Node& complexContentNode,
    const std::string& targetNamespace)
{
    const auto childNodes = complexContentNode.GetChildren(nullptr);
    for (const auto& childNode : childNodes)
    {
        const std::string name = childNode.GetName();

        if (name == "extension")
        {
            LoadExtension(type, childNode, targetNamespace);
        }
        else
        {
            std::cerr << "complexContent::" << name << " not implemented" << '\n';
        }
    }
}

void LoadComplexType(
    ExtendedType& type,
    const soaplib::xml::Node& complexTypeNode,
    const std::string& targetNamespace)
{
    const auto childNodes = complexTypeNode.GetChildren(nullptr);
    for (const auto& childNode : childNodes)
    {
        const std::string name = childNode.GetName();

        if (name == "complexContent")
        {
            LoadComplexContent(type, childNode, targetNamespace);
        }
        else if (name == "sequence")
        {
            LoadSequence(type, childNode, targetNamespace);
        }
        else
        {
            std::cerr << "complexType::" << name << " not implemented" << '\n';
        }
    }
}


TypePtr LoadComplexType(
    const soaplib::xml::Node& complexTypeNode,
    const Name* name,
    const std::string& targetNamespace)
{
    auto type = std::make_shared<ExtendedType>();
    if (name)
    {
        type->name = *name;
    }
    else
    {
        type->name = getName(complexTypeNode, "name", targetNamespace);
    }

    LoadComplexType(*type, complexTypeNode, targetNamespace);

    return type;
}

TypePtr LoadSimpleType(
    const soaplib::xml::Node& simpleTypeNode,
    const Name* name_,
    const std::string& targetNamespace)
{
    Name name;
    std::optional<Name> base;

    if (name_)
    {
        name = *name_;
    }
    else
    {
        name = getName(simpleTypeNode, "name", targetNamespace);
    }

    // optional base type

    try
    {
        const auto& restrictionNode = simpleTypeNode.GetChild("restriction");
        base = getName(restrictionNode, "base", targetNamespace);
    }
    catch (const soaplib::xml::Exception&)
    {
    }

    // try enum first

    try
    {
        std::vector<EnumType::Enumeration> enumerations;

        const auto& restrictionNode = simpleTypeNode.GetChild("restriction");

        const auto enumerationNodes = restrictionNode.GetChildren("enumeration");
        for (const auto& enumerationNode : enumerationNodes)
        {
            EnumType::Enumeration enumeration;
            enumeration.text = enumerationNode.GetStringProp("value");

            try
            {
                auto annotationNode = enumerationNode.GetChild("annotation");
                auto appinfoNode = annotationNode.GetChild("appinfo");
                auto enumerationValueNode = appinfoNode.GetChild("EnumerationValue");
                enumeration.value = enumerationValueNode.GetIntVal();
            }
            catch (const soaplib::xml::Exception&)
            {
            }

            enumerations.push_back(enumeration);
        }

        if (!enumerations.empty())
        {
            auto enumType = std::make_shared<EnumType>();
            enumType->name = name;
            enumType->enumerations = enumerations;
            return enumType;
        }
    }
    catch (const soaplib::xml::Exception& ex)
    {
        std::cerr << ex.what() << std::endl;
    }

    auto basicType = std::make_shared<BasicType>();
    basicType->name = name;
    basicType->base = base;
    return basicType;
}

TypePtr LoadElement(
    const soaplib::xml::Node& elementNode,
    const std::string& targetNamespace)
{
    auto typeName = getName(elementNode, "name", targetNamespace);

    const auto childNodes = elementNode.GetChildren(nullptr);
    for (const auto& childNode : childNodes)
    {
        const std::string name = childNode.GetName();

        if (name == "complexType")
        {
            return LoadComplexType(childNode, &typeName, targetNamespace);
        }
        else if (name == "simpleType")
        {
            return LoadSimpleType(childNode, &typeName, targetNamespace);
        }
        else
        {
            std::cerr << "element::" << name << " not implemented" << '\n';
        }
    }

    try
    {
        auto basicType = std::make_shared<BasicType>();
        basicType->name = typeName;
        basicType->base = getName(elementNode, "type", targetNamespace);
        return basicType;
    }
    catch (...)
    {
    }

    return {};
}

std::vector<TypePtr> LoadSchemaTypes(
    const soaplib::xml::Node& schemaNode)
{
    std::vector<TypePtr> types;

    const auto targetNamespace = schemaNode.GetStringProp("targetNamespace");

    const auto elementNodes = schemaNode.GetChildren("element");
    for (const auto& elementNode : elementNodes)
    {
        types.push_back(LoadElement(elementNode, targetNamespace));
    }

    const auto complexTypeNodes = schemaNode.GetChildren("complexType");
    for (const auto& complexTypeNode : complexTypeNodes)
    {
        types.push_back(LoadComplexType(complexTypeNode, nullptr, targetNamespace));
    }

    const auto simpleTypeNodes = schemaNode.GetChildren("simpleType");
    for (const auto& simpleTypeNode : simpleTypeNodes)
    {
        types.push_back(LoadSimpleType(simpleTypeNode, nullptr, targetNamespace));
    }

    return types;
}

std::vector<TypePtr> LoadTypes(
    const soaplib::xml::Node& typesNode)
{
    std::vector<TypePtr> types;

    const auto schemaNodes = typesNode.GetChildren("schema");//getChildren(typesNode, "schema");
    for (const auto& schemaNode : schemaNodes)
    {
        auto schemaTypes = LoadSchemaTypes(schemaNode);

        types.insert(
            std::end(types),
            std::begin(schemaTypes),
            std::end(schemaTypes)
        );
    }

    return types;
}

std::shared_ptr<Definition> LoadDefinition(
    const soaplib::xml::Node& definitionNode)
{
    auto definition = std::make_shared<Definition>();

    try
    {
        definition->name = getName(definitionNode, "name", {});
    }
    catch (const std::exception& ex)
    {
        std::cerr << "LoadDefinition: " << ex.what() << '\n';
    }

    const auto serviceNodes = definitionNode.GetChildren("service");//getChildren(definitionNode, "service");
    for (const auto& serviceNode : serviceNodes)
    {
        definition->services.push_back(LoadService(serviceNode));
    }

    const auto bindingNodes = definitionNode.GetChildren("binding");//getChildren(definitionNode, "binding");
    for (const auto& bindingNode : bindingNodes)
    {
        definition->bindings.push_back(LoadBinding(bindingNode));
    }

    const auto portTypeNodes = definitionNode.GetChildren("portType");//getChildren(definitionNode, "portType");
    for (const auto& portTypeNode : portTypeNodes)
    {
        definition->portTypes.push_back(LoadPortType(portTypeNode));
    }

    const auto messageNodes = definitionNode.GetChildren("message");//getChildren(definitionNode, "message");
    for (const auto& messageNode : messageNodes)
    {
        definition->messages.push_back(LoadMessage(messageNode));
    }

    const auto typesNodes = definitionNode.GetChildren("types");//getChildren(definitionNode, "types");
    for (const auto& typesNode : typesNodes)
    {
        auto types = LoadTypes(typesNode);

        definition->types.insert(
            std::end(definition->types),
            std::begin(types),
            std::end(types)
        );
    }

    return definition;
}

std::shared_ptr<Definition> LoadWsdl(
    const std::string& fileName)
{
    try
    {
        auto doc = soaplib::xml::Document::ParseFile(fileName.c_str());
        auto root = doc.GetRootNode();
        return LoadDefinition(root);
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
    }

    return {};
}


#ifdef DEPCHECK
bool NameCompare(
    const Name& a,
    const Name& b,
    bool checkNamespace)
{
    if (a.name != b.name)
    {
        return false;
    }

    if (checkNamespace)
    {
        if (a.nsHref != b.nsHref)
        {
            return false;
        }
        /*
        if (a.nsPrefix != b.nsPrefix)
        {
            return false;
        }
        */
    }

    return true;
}

TypePtr FindType(
    const Name& name,
    const Definition& definition,
    bool checkNamespace)
{
    for (auto& type : definition.types)
    {
        if (!type)
        {
            continue;
        }

        if (NameCompare(name, type->name, checkNamespace))
        {
            return type;
        }
    }

    return {};
}

TypePtr GetBaseType(
    TypePtr type,
    const Definition& definition,
    bool checkNs)
{
    if (!type)
    {
        return {};
    }

    if (type->kind == Type::Basic)
    {
        auto btype = reinterpret_cast<BasicType&>(*type);
        if (btype.base.has_value())
        {
            return FindType(btype.base.value(), definition, checkNs);
        }
        return {};
    }

    if (type->kind == Type::Extended)
    {
        auto etype = reinterpret_cast<ExtendedType&>(*type);
        if (etype.base.has_value())
        {
            return FindType(etype.base.value(), definition, checkNs);
        }
        return {};
    }

    return {};
}

void GetTypeHistory(
    TypePtr type,
    const Definition& definition,
    bool checkNamespaces,
    std::vector<Name>& history)
{
    if (!type)
    {
        return;
    }

    history.push_back(type->name);

    GetTypeHistory(
        GetBaseType(type, definition, checkNamespaces),
        definition,
        checkNamespaces,
        history
    );
}

void RedundancyCheck(
    Definition& definition)
{
    const bool checkNamespace = false;

    for (auto& type : definition.types)
    {
        if (!type)
        {
            continue;
        }

        if (type->kind != Type::Extended)
        {
            continue;
        }

        auto& extType = reinterpret_cast<ExtendedType&>(*type);

        if (extType.parameters.empty())
        {
            continue;
        }

        std::vector<Name> typeHistory;
        GetTypeHistory(type, definition, checkNamespace, typeHistory);

        if (typeHistory.size() > 1)
        {
            std::cout << "* " << type->name.name << '\n';
            for (const auto name : typeHistory)
            {
                std::cout << "  - " << name.name << '\n';
            }
        }

        //
        // check if this type's name shows up in the type history of the parameter types
        //

        for (auto& parameter : extType.parameters)
        {
            bool isRedundant = false;

            TypePtr ptype = FindType(parameter.type, definition, checkNamespace);

            std::vector<Name> ptypeHistory;
            GetTypeHistory(ptype, definition, checkNamespace, ptypeHistory);

            for (const auto& name : typeHistory)
            {
                for (const auto& pname : ptypeHistory)
                {
                    if (NameCompare(type->name, ptype->name, checkNamespace))
                    {
                        std::cout
                            << "type " << type->name.name
                            << ": redundancy detected"
                            << ": parameter " << parameter.name.name
                            << ": " << name.name
                            << " - " << ptype->name.name
                            << '\n';

                        parameter.isRedundant = true;
                        break;
                    }
                }
            }


            /*
            while (ptype)
            {
                if (NameCompare(type->name, ptype->name, checkNamespace))
                {
                    std::cout
                        << "type " << type->name.name
                        << ": redundancy detected"
                        << ": parameter " << parameter.name.name
                        << ": " << ptype->name.name
                        << '\n';

                    parameter.isRedundant = true;
                    break;
                }

                ptype = GetBaseType(ptype, definition, checkNamespace);
            }
            */
        }
    }
}
#endif
