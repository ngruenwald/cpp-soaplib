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

    template<typename T>
    static T* ptr_cast_to(
        std::unique_ptr<BaseType>& base)
    {
        return static_cast<T*>(base.get());
    }
};

// Concrete Type

struct ConcreteType : BaseType
{
    int A;
    int B;
};

std::unique_ptr<BaseType> ConcreteType_Create()
{
    auto ptr = std::make_unique<ConcreteType>();
    ptr->A = 23;
    ptr->B = 42;
    return ptr;
}

// Any Type

struct AnyType
{
    std::unique_ptr<BaseType> Value;

    template<typename T>
    T* ValueCast()
    {
        return BaseType::ptr_cast_to<T>(Value);
    }
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


TEST_CASE("Pointer conversion", "[all][pointer]")
{
    Container container;
    Container_Create(container);

    REQUIRE(container.Content.has_value());
    REQUIRE(container.Content.value().Value);

    auto ptr = container.Content->ValueCast<ConcreteType>();

    REQUIRE(ptr);
    REQUIRE(ptr->A == 23);
    REQUIRE(ptr->B == 42);
}
