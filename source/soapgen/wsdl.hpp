#pragma once

#include <memory>
#include <optional>
#include <string>
#include <vector>

//#define DEPCHECK

struct Name
{
    std::string name;
    std::string nsPrefix;
    std::string nsHref;
};

inline bool operator==(const Name& a, const Name& b)
{
    return (a.name == b.name && a.nsPrefix == b.nsPrefix && a.nsHref == b.nsHref);
}

struct Type
{
    enum Kind
    {
        Basic,
        Extended,
        Enum
    };

    Kind kind;
    Name name;

    Type(Type::Kind k): kind(k) {}
};

typedef std::shared_ptr<Type> TypePtr;

struct Parameter
{
    enum Kind
    {
        Mandatory,
        Pointer,
        Optional,
        Multiple
    };

    Name name;
    Name type;
    Kind kind;

#ifdef DEPCHECK
    bool isRedundant = false;
#endif

    Parameter() : kind(Parameter::Kind::Mandatory) {}
};

struct BasicType : Type
{
    std::optional<Name> base;

    BasicType() : Type(Type::Basic) {}
};

struct ExtendedType : Type
{
    std::optional<Name> base;
    std::vector<Parameter> parameters;
    std::vector<TypePtr> innerTypes;

    ExtendedType() : Type(Type::Extended) {}
};

struct EnumType : Type
{
    struct Enumeration
    {
        std::string text;
        std::optional<int> value;
    };

    std::vector<EnumType::Enumeration> enumerations;

    EnumType() : Type(Type::Enum) {}
};


struct Message
{
    Name name;

    struct Part
    {
        Name name;
        Name element;
    };

    std::vector<Part> parts;
};

struct Port
{
    Name name;
    Name binding;

    struct Address
    {
        std::string location;
    };
    Address address;

    struct EndpointReference
    {
        std::string address;
    };
    EndpointReference endpointReference;
};

struct Service
{
    Name name;
    std::vector<Port> ports;
};

struct Input
{
    Name action;
    Name message;
};

struct Output
{
    Name action;
    Name message;
};

struct Operation
{
    Name name;
    Input input;
    Output output;
};

struct PortType
{
    Name name;
    std::vector<Operation> operations;
};

struct Binding
{
    Name name;
    Name type;

    // policyReference
    // binding

    std::vector<Operation> operations;
};

struct Definition
{
    Name name;
    // policies
    std::vector<TypePtr> types;
    std::vector<Message> messages;
    std::vector<PortType> portTypes;
    std::vector<Binding> bindings;
    std::vector<Service> services;
};

std::shared_ptr<Definition> LoadWsdl(
    const std::string& fileName);

#ifdef DEBCHECK
void RedundancyCheck(
    Definition& definition);
#endif
