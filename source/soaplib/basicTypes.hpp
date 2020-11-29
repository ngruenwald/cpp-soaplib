#pragma once

#include <chrono>
#include <cstdint>
#include <memory>
#include <string>
#include <optional>
#include <vector>

#include <soaplib/types/soapBaseType.hpp>
#include <soaplib/types/simpleTypes.hpp>
#include <soaplib/types/uuid.hpp>
#include <soaplib/types/timestamp.hpp>
#include <soaplib/types/duration.hpp>
#include <soaplib/types/base64.h>

#if 0

namespace xml { class Node; }

//namespace soaplib {


std::int8_t int8_tFromXml(
    const xml::Node& node);

void int8_tToXml(
    xml::Node& node,
    std::int8_t value);

std::int16_t int16_tFromXml(
    const xml::Node& node);

void int16_tToXml(
    xml::Node& node,
    std::int16_t value);

std::int32_t int32_tFromXml(
    const xml::Node& node);

void int32_tToXml(
    xml::Node& node,
    std::int32_t value);

std::int64_t int64_tFromXml(
    const xml::Node& node);

void int64_tToXml(
    xml::Node& node,
    std::int64_t value);


std::uint8_t uint8_tFromXml(
    const xml::Node& node);

void uint8_tToXml(
    xml::Node& node,
    std::uint8_t value);

std::uint16_t uint16_tFromXml(
    const xml::Node& node);

void uint16_tToXml(
    xml::Node& node,
    std::uint16_t value);

std::uint32_t uint32_tFromXml(
    const xml::Node& node);

void uint32_tToXml(
    xml::Node& node,
    std::uint32_t value);

std::uint64_t uint64_tFromXml(
    const xml::Node& node);

void uint64_tToXml(
    xml::Node& node,
    std::uint64_t value);


std::string stringFromXml(
    const xml::Node& node);

void stringToXml(
    xml::Node& node,
    const std::string& value);


bool boolFromXml(
    const xml::Node& node);

void boolToXml(
    xml::Node& node,
    bool value);


float floatFromXml(
    const xml::Node& node);

void floatToXml(
    xml::Node& node,
    float value);

double doubleFromXml(
    const xml::Node& node);

void doubleToXml(
    xml::Node& node,
    double value);


soaplib::Base64 Base64FromXml(
	const xml::Node& node);

void Base64ToXml(
    xml::Node& node,
    const soaplib::Base64& value);


soaplib::Timestamp TimestampFromXml(
    const xml::Node& node);

void TimestampToXml(
    xml::Node& node,
    const soaplib::Timestamp& value);

soaplib::Duration DurationFromXml(
    const xml::Node& node);

void DurationToXml(
    xml::Node& node,
    const soaplib::Duration& value);


soaplib::uuid uuidFromXml(
    const xml::Node& node);

void uuidToXml(
    xml::Node& node,
    const soaplib::uuid& value);

//} // namespace soaplib
#endif
