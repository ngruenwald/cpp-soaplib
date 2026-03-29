// ListOfContinentsByCodeResponse
#pragma once

#include <memory>
#include <soaplib/basicTypes.hpp>

#include "ArrayOftContinent.hpp"

namespace soaplib { namespace xml { class Document; } }
namespace soaplib { namespace xml { class Node; } }

namespace country {

struct ListOfContinentsByCodeResponse
    : soaplib::SoapBaseType
{
    ::country::ArrayOftContinent ListOfContinentsByCodeResult;

    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& node) const override;
};

void ListOfContinentsByCodeResponseFromXml(
    const soaplib::xml::Node& objNode,
    ListOfContinentsByCodeResponse& obj);

ListOfContinentsByCodeResponse ListOfContinentsByCodeResponseFromXml(
    const soaplib::xml::Node& objNode);

std::unique_ptr<soaplib::SoapBaseType> ListOfContinentsByCodeResponsePtrFromXml(
    const soaplib::xml::Node& objNode);

void ListOfContinentsByCodeResponseToXml(
    const ListOfContinentsByCodeResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode);

} // namespace country
