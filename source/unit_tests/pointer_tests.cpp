#include "unit_tests.hpp"

#include <iostream>
#include <optional>


template<typename T>
std::optional<T> GetOptional(T (*create)())
{
    return std::optional<T>(create());
}

// Base Type

struct BaseType
{
    virtual ~BaseType() {};
};

// Concrete Type

struct ConcreteType : BaseType
{
    int A;
    int B;
};

std::shared_ptr<BaseType> ConcreteType_Create()
{
    auto obj = std::make_shared<ConcreteType>();
    obj->A = 23;
    obj->B = 42;
    return std::static_pointer_cast<BaseType>(obj);
}

// Any Type

struct AnyType
{
    std::shared_ptr<BaseType> Value;
};

void AnyType_Create(
    AnyType& obj)
{
    obj.Value = ConcreteType_Create();
}

AnyType AnyType_Create()
{
    AnyType obj;
    AnyType_Create(obj);
    return obj;
}

// Container

struct ContainerBase : BaseType
{
};

struct Container : ContainerBase
{
    std::optional<AnyType> Content;
};

void Container_Create(Container& container)
{
    container.Content = GetOptional<AnyType>(AnyType_Create);
}


TEST_CASE("Shared pointer conversion", "[all][pointer]")
{
    Container container;
    Container_Create(container);

    REQUIRE(container.Content.has_value());
    REQUIRE(container.Content.value().Value);

    auto ptr = std::dynamic_pointer_cast<ConcreteType>(container.Content.value().Value);
    REQUIRE(ptr);
    REQUIRE(ptr->A == 23);
    REQUIRE(ptr->B == 42);
}
