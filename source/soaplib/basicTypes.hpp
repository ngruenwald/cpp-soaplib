#pragma once

#include <chrono>
#include <cstdint>
#include <memory>
#include <string>
#include <optional>
#include <vector>

#include <soaplib/types/uuid.hpp>
#include <soaplib/types/timestamp.hpp>
#include <soaplib/types/duration.hpp>
#include <soaplib/types/base64.h>

namespace xml { class Node; }

//namespace soaplib {

std::int8_t int8_tFromXml(
    const xml::Node& node);

std::int16_t int16_tFromXml(
    const xml::Node& node);

std::int32_t int32_tFromXml(
    const xml::Node& node);

std::int64_t int64_tFromXml(
    const xml::Node& node);


std::uint8_t uint8_tFromXml(
    const xml::Node& node);

std::uint16_t uint16_tFromXml(
    const xml::Node& node);

std::uint32_t uint32_tFromXml(
    const xml::Node& node);

std::uint64_t uint64_tFromXml(
    const xml::Node& node);

std::string stringFromXml(
    const xml::Node& node);


bool boolFromXml(
    const xml::Node& node);


float floatFromXml(
    const xml::Node& node);

double doubleFromXml(
    const xml::Node& node);


soaplib::Base64 Base64FromXml(
	const xml::Node& node);

soaplib::Timestamp TimestampFromXml(
    const xml::Node& node);

soaplib::Duration DurationFromXml(
    const xml::Node& node);

soaplib::uuid uuidFromXml(
    const xml::Node& node);

//} // namespace soaplib
