#include "wsdl.hpp"

#include <iostream>
#include <libxmlwrp.hpp>

void LoadComplexType(
    ExtendedType& type,
    const xml::Node& complexTypeNode);

/*
xml::NodeList getChildren(
    const xml::Node& parent,
    const char* name)
{
    xml::NodeList nodes;

    for (auto xmlNode = xmlFirstElementChild(parent.GetXmlNode());
         xmlNode != nullptr;
         xmlNode = xmlNextElementSibling(xmlNode))
    {
        if (name == nullptr || xmlStrcmp(xmlNode->name, BAD_CAST name) == 0)
        {
            nodes.emplace_back(libxml2::XmlNode(xmlNode));
        }
    }

    return nodes;
}
*/

std::string getNsPrefix(
    const xml::Node& node,
    const std::string& nsFull)
{
    for (auto ns = node.GetXmlNode()->nsDef; ns; ns = ns->next)
    {
        if (xmlStrcmp(ns->href, BAD_CAST nsFull.c_str()) == 0)
        {
            return reinterpret_cast<const char*>(ns->prefix);
        }
    }

    for (auto ns = node.GetXmlNode()->ns; ns; ns = ns->next)
    {
        if (xmlStrcmp(ns->href, BAD_CAST nsFull.c_str()) == 0)
        {
            return reinterpret_cast<const char*>(ns->prefix);
        }
    }

    return {};
}

std::string getNsHref(
    const xml::Node& node,
    const std::string& nsShort)
{
    for (auto ns = node.GetXmlNode()->nsDef; ns; ns = ns->next)
    {
        if (xmlStrcmp(ns->prefix, BAD_CAST nsShort.c_str()) == 0)
        {
            return reinterpret_cast<const char*>(ns->href);
        }
    }

    for (auto ns = node.GetXmlNode()->ns; ns; ns = ns->next)
    {
        if (xmlStrcmp(ns->prefix, BAD_CAST nsShort.c_str()) == 0)
        {
            return reinterpret_cast<const char*>(ns->href);
        }
    }

    return {};
}

Name getName(
    const xml::Node& node,
    const char* attributeName)
{
    const auto value = node.GetStringProp(attributeName);
    auto idx = value.find(':');

    if (idx == std::string::npos)
    {
        Name name;
        name.name = value;
        return name;
    }

    if (idx + 2 < value.length() && value.substr(idx, 3) == "://")
    {
        Name name;
        name.name = value;
        return name;
    }

    Name name;
    name.name = value.substr(idx + 1);
    name.nsPrefix = value.substr(0, idx);
    name.nsHref = getNsHref(node, name.nsPrefix);

    if (name.nsHref.empty())
    {
        std::cerr << "could not find namespace for " << value << '\n';
    }

    return name;
}


Input LoadInput(
    const xml::Node& inputNode)
{
    Input input;

    input.action = getName(inputNode, "Action");
    input.message = getName(inputNode, "message");

    return input;
}

Output LoadOutput(
    const xml::Node& outputNode)
{
    Output output;

    output.action = getName(outputNode, "Action");
    output.message = getName(outputNode, "message");

    return output;
}

Operation LoadOperation(
    const xml::Node& operationNode)
{
    Operation operation;

    operation.name = getName(operationNode, "name");

    auto inputNode = operationNode.GetChild("input");
    operation.input = LoadInput(inputNode);

    auto outputNode = operationNode.GetChild("output");
    operation.output = LoadOutput(outputNode);

    return operation;
}

Port LoadPort(
    const xml::Node& portNode)
{
    Port port;

    port.name = getName(portNode, "name");
    port.binding = getName(portNode, "binding");

    try
    {
        auto addressNode = portNode.GetChild("address");
        port.address.location = addressNode.GetStringProp("location");
    }
    catch (const xml::Exception&)
    {
    }

    try
    {
        auto endpointReferenceNode = portNode.GetChild("EndpointReference");
        auto addressNode = endpointReferenceNode.GetChild("Address");
        port.endpointReference.address = addressNode.GetStringVal();
    }
    catch (const xml::Exception&)
    {
    }

    return port;
}

Service LoadService(
    const xml::Node& serviceNode)
{
    Service service;

    service.name = getName(serviceNode, "name");

    const auto portNodes = serviceNode.GetChildren("port"); //getChildren(serviceNode, "port");
    for (const auto& portNode : portNodes)
    {
        service.ports.push_back(LoadPort(portNode));
    }

    return service;
}

Binding LoadBinding(
    const xml::Node& bindingNode)
{
    Binding binding;

    binding.name = getName(bindingNode, "name");
    binding.type = getName(bindingNode, "type");

    // TODO

    return binding;
}

PortType LoadPortType(
    const xml::Node& portTypeNode)
{
    PortType portType;

    portType.name = getName(portTypeNode, "name");

    const auto operationNodes = portTypeNode.GetChildren("operation");  //getChildren(portTypeNode, "operation");
    for (const auto& operationNode : operationNodes)
    {
        portType.operations.push_back(LoadOperation(operationNode));
    }

    return portType;
}

Message LoadMessage(
    const xml::Node& messageNode)
{
    Message message;

    message.name = getName(messageNode, "name");

    const auto partNodes = messageNode.GetChildren("part"); //getChildren(messageNode, "part");
    for (const auto& partNode : partNodes)
    {
        Message::Part part;

        part.name = getName(partNode, "name");
        part.element = getName(partNode, "element");

        message.parts.push_back(part);
    }

    return message;
}

void LoadElement(
    ExtendedType& type,
    const xml::Node& elementNode)
{
    Parameter parameter;
    parameter.name = getName(elementNode, "name");

    int minOccurs = 1;
    int maxOccurs = 1;
    bool nillable = false;

    try
    {
        minOccurs = elementNode.GetIntProp("minOccurs");
    }
    catch (const xml::Exception&)
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
    catch (const xml::Exception&)
    {
    }

    try
    {
        nillable = elementNode.GetBoolProp("nillable");
    }
    catch (const xml::Exception&)
    {
    }

    if (maxOccurs > 1)
    {
        parameter.kind = Parameter::Multiple;
    }
    else if (nillable)
    {
        parameter.kind = Parameter::Pointer;
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
        parameter.type = getName(elementNode, "type");
    }
    catch (const xml::Exception&)
    {
        const auto childNodes = elementNode.GetChildren(nullptr);   //getChildren(elementNode, nullptr);
        if (!childNodes.empty())
        {
            for (const auto& childNode : childNodes)
            {
                const std::string name = childNode.GetName();

                if (name == "complexType")
                {
                    auto innerType = std::make_shared<ExtendedType>();
                    innerType->name = parameter.name;
                    innerType->name.name;
                    LoadComplexType(*innerType, childNode);
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
    const xml::Node& sequenceNode)
{
    const auto elementNodes = sequenceNode.GetChildren("element");  //getChildren(sequenceNode, "element");
    for (const auto& elementNode : elementNodes)
    {
        LoadElement(type, elementNode);
    }
}

void LoadExtension(
    ExtendedType& type,
    const xml::Node& extensionNode)
{
    type.base = getName(extensionNode, "base");

    const auto childNodes = extensionNode.GetChildren(nullptr); //getChildren(extensionNode, nullptr);
    for (const auto& childNode : childNodes)
    {
        const std::string name = childNode.GetName();

        if (name == "sequence")
        {
            LoadSequence(type, childNode);
        }
        else
        {
            std::cout << "extension::" << name << " not implemented" << '\n';
        }
    }
}

void LoadComplexContent(
    ExtendedType& type,
    const xml::Node& complexContentNode)
{
    const auto childNodes = complexContentNode.GetChildren(nullptr); //getChildren(complexContentNode, nullptr);
    for (const auto& childNode : childNodes)
    {
        const std::string name = childNode.GetName();

        if (name == "extension")
        {
            LoadExtension(type, childNode);
        }
        else
        {
            std::cerr << "complexContent::" << name << " not implemented" << '\n';
        }
    }
}

void LoadComplexType(
    ExtendedType& type,
    const xml::Node& complexTypeNode)
{
    const auto childNodes = complexTypeNode.GetChildren(nullptr);//getChildren(complexTypeNode, nullptr);
    for (const auto& childNode : childNodes)
    {
        const std::string name = childNode.GetName();

        if (name == "complexContent")
        {
            LoadComplexContent(type, childNode);
        }
        else if (name == "sequence")
        {
            LoadSequence(type, childNode);
        }
        else
        {
            std::cerr << "complexType::" << name << " not implemented" << '\n';
        }
    }
}


TypePtr LoadComplexType(
    const xml::Node& complexTypeNode,
    const Name* name = nullptr)
{
    auto type = std::make_shared<ExtendedType>();
    if (name)
    {
        type->name = *name;
    }
    else
    {
        type->name = getName(complexTypeNode, "name");
    }

    LoadComplexType(*type, complexTypeNode);

    return type;
}

TypePtr LoadSimpleType(
    const xml::Node& simpleTypeNode,
    const Name* name_ = nullptr)
{
    Name name;
    std::optional<Name> base;

    if (name_)
    {
        name = *name_;
    }
    else
    {
        name = getName(simpleTypeNode, "name");
    }

    // optional base type

    try
    {
        const auto& restrictionNode = simpleTypeNode.GetChild("restriction");
        base = getName(restrictionNode, "base");
    }
    catch (const xml::Exception&)
    {
    }

    // try enum first

    try
    {
        std::vector<EnumType::Enumeration> enumerations;

        const auto& restrictionNode = simpleTypeNode.GetChild("restriction");

        const auto enumerationNodes = restrictionNode.GetChildren("enumeration");//getChildren(restrictionNode, "enumeration");
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
            catch (const xml::Exception&)
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
    catch (const xml::Exception& ex)
    {
        std::cerr << ex.what() << std::endl;
    }

    auto basicType = std::make_shared<BasicType>();
    basicType->name = name;
    basicType->base = base;
    return basicType;
}

TypePtr LoadElement(
    const xml::Node& elementNode)
{
    auto typeName = getName(elementNode, "name");

    const auto childNodes = elementNode.GetChildren(nullptr);//getChildren(elementNode, nullptr);
    for (const auto& childNode : childNodes)
    {
        const std::string name = childNode.GetName();

        if (name == "complexType")
        {
            return LoadComplexType(childNode, &typeName);
        }
        else if (name == "simpleType")
        {
            return LoadSimpleType(childNode, &typeName);
        }
        else
        {
            std::cerr << "element::" << name << " not implemented" << '\n';
        }
    }

    return {};
}

std::vector<TypePtr> LoadSchemaTypes(
    const xml::Node& schemaNode)
{
    std::vector<TypePtr> types;

    const auto elementNodes = schemaNode.GetChildren("element"); // getChildren(schemaNode, "element");
    for (const auto& elementNode : elementNodes)
    {
        types.push_back(LoadElement(elementNode));
    }

    const auto complexTypeNodes = schemaNode.GetChildren("complexType");//getChildren(schemaNode, "complexType");
    for (const auto& complexTypeNode : complexTypeNodes)
    {
        types.push_back(LoadComplexType(complexTypeNode));
    }

    const auto simpleTypeNodes = schemaNode.GetChildren("simpleType");//getChildren(schemaNode, "simpleType");
    for (const auto& simpleTypeNode : simpleTypeNodes)
    {
        types.push_back(LoadSimpleType(simpleTypeNode));
    }

    return types;
}

std::vector<TypePtr> LoadTypes(
    const xml::Node& typesNode)
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
    const xml::Node& definitionNode)
{
    auto definition = std::make_shared<Definition>();

    definition->name = getName(definitionNode, "name");

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
    for (const auto typesNode : typesNodes)
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
        xml::Document doc(fileName.c_str());
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
    if (type)
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