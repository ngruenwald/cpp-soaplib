// ListOfContinentsByNameResponse
#pragma once

#include <memory>
#include <soaplib/basicTypes.hpp>

#include "ArrayOftContinent.hpp"

namespace soaplib { namespace xml { class Document; } }
namespace soaplib { namespace xml { class Node; } }

namespace country {

struct ListOfContinentsByNameResponse
    : soaplib::SoapBaseType
{
    ::country::ArrayOftContinent ListOfContinentsByNameResult;

    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& node) const override;
};

void ListOfContinentsByNameResponseFromXml(
    const soaplib::xml::Node& objNode,
    ListOfContinentsByNameResponse& obj);

ListOfContinentsByNameResponse ListOfContinentsByNameResponseFromXml(
    const soaplib::xml::Node& objNode);

std::unique_ptr<soaplib::SoapBaseType> ListOfContinentsByNameResponsePtrFromXml(
    const soaplib::xml::Node& objNode);

void ListOfContinentsByNameResponseToXml(
    const ListOfContinentsByNameResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode);

} // namespace country
