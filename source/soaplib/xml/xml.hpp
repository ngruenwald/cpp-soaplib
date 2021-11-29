#pragma once

#include <soaplib/xml/document.hpp>
#include <soaplib/xml/node.hpp>
#include <soaplib/xml/exception.hpp>

// some helper macros
#define XML_MANDATORY(__X__) __X__
#define XML_OPTIONAL(__X__) try { __X__; } catch (soaplib::xml::Exception&) { }

namespace soaplib {
namespace xml {

// Sets the logger callback
void SetErrorLogger(
    void(*logHandler)(void*, const char*, ...),
    void* logContext);

// Initializes the XML parser
void Init(
    void(*logHandler)(void*, const char*, ...) = NULL,
    void* logContext = NULL);

// Cleans up the allocated memory
void Cleanup();

} // namespace xml
} // namespace soaplib
