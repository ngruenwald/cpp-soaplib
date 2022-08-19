#pragma once

#include <chrono>
#include <cstdint>
#include <memory>
#include <string>
#include <optional>
#include <vector>

#include <soaplib/types/soapBaseType.hpp>
#include <soaplib/types/simpleTypes.hpp>
#include <soaplib/types/dateTypes.hpp>
#include <soaplib/types/uuid.hpp>
//#include <soaplib/types/timestamp.hpp>
//#include <soaplib/types/duration.hpp>
#include <soaplib/types/base64.h>

#if 0

namespace soaplib { namespace xml { class Node; } }

//namespace soaplib {


std::int8_t int8_tFromXml(
    const soaplib::xml::Node& node);

void int8_tToXml(
    soaplib::xml::Node& node,
    std::int8_t value);

std::int16_t int16_tFromXml(
    const soaplib::xml::Node& node);

void int16_tToXml(
    soaplib::xml::Node& node,
    std::int16_t value);

std::int32_t int32_tFromXml(
    const soaplib::xml::Node& node);

void int32_tToXml(
    soaplib::xml::Node& node,
    std::int32_t value);

std::int64_t int64_tFromXml(
    const soaplib::xml::Node& node);

void int64_tToXml(
    soaplib::xml::Node& node,
    std::int64_t value);


std::uint8_t uint8_tFromXml(
    const soaplib::xml::Node& node);

void uint8_tToXml(
    soaplib::xml::Node& node,
    std::uint8_t value);

std::uint16_t uint16_tFromXml(
    const soaplib::xml::Node& node);

void uint16_tToXml(
    soaplib::xml::Node& node,
    std::uint16_t value);

std::uint32_t uint32_tFromXml(
    const soaplib::xml::Node& node);

void uint32_tToXml(
    soaplib::xml::Node& node,
    std::uint32_t value);

std::uint64_t uint64_tFromXml(
    const soaplib::xml::Node& node);

void uint64_tToXml(
    soaplib::xml::Node& node,
    std::uint64_t value);


std::string stringFromXml(
    const soaplib::xml::Node& node);

void stringToXml(
    soaplib::xml::Node& node,
    const std::string& value);


bool boolFromXml(
    const soaplib::xml::Node& node);

void boolToXml(
    soaplib::xml::Node& node,
    bool value);


float floatFromXml(
    const soaplib::xml::Node& node);

void floatToXml(
    soaplib::xml::Node& node,
    float value);

double doubleFromXml(
    const soaplib::xml::Node& node);

void doubleToXml(
    soaplib::xml::Node& node,
    double value);


soaplib::Base64 Base64FromXml(
    const soaplib::xml::Node& node);

void Base64ToXml(
    soaplib::xml::Node& node,
    const soaplib::Base64& value);


soaplib::Timestamp TimestampFromXml(
    const soaplib::xml::Node& node);

void TimestampToXml(
    soaplib::xml::Node& node,
    const soaplib::Timestamp& value);

soaplib::Duration DurationFromXml(
    const soaplib::xml::Node& node);

void DurationToXml(
    soaplib::xml::Node& node,
    const soaplib::Duration& value);


soaplib::uuid uuidFromXml(
    const soaplib::xml::Node& node);

void uuidToXml(
    soaplib::xml::Node& node,
    const soaplib::uuid& value);

//} // namespace soaplib
#endif
