# SOAP Specification Compliance (1.1 and 1.2)

This document tracks the support for SOAP 1.1 and 1.2 in `cpp-soaplib`.

## Strategy
Support both versions by making the core library version-agnostic and allowing the generator to select the appropriate version based on WSDL detection or explicit configuration.

## Roadmap & Tasks

### Priority 1: Multi-Version Foundation (Library)
- [x] Define `SoapVersion` enum in `soaplib`.
- [x] Update `SoapBase` to use version-specific namespaces (`http://schemas.xmlsoap.org/soap/envelope/` vs `http://www.w3.org/2003/05/soap-envelope`).
- [x] Implement structured `SoapFault` support for both 1.1 and 1.2.
- [x] Implement automatic version detection in `HttpSoapTransport` via root namespace.
- [x] Update `HttpSoapTransport` to use correct `Content-Type` for each version.

### Priority 2: Generator Updates
- [x] Add `soapVersion` to `cppgen::Options` (Soap11, Soap12, Auto).
- [x] Update `main.cpp` to parse `soap-version` from CLI and config.
- [x] Update `ServerImpl.tpl` to use the configured version when generating Faults.
- [x] Update `ServiceImpl.tpl` to set the correct version in the base class.
- [x] Implement `SOAPAction` HTTP header support for SOAP 1.1 in `HttpSoapTransport`.

### Priority 3: Specification Gaps
- [x] Implement `mustUnderstand` attribute validation for both versions.
- [x] Add support for `role` (1.2) / `actor` (1.1) attributes.
- [x] Add support for `relay` attribute (1.2).
- [x] Implement support for HTTP `GET` (SOAP 1.2 Web Method Feature).

### Priority 4: Binding & Transport
- [x] Update `HttpSoapTransport` to handle HTTP 202 (Accepted) for one-way messages.
- [x] Ensure `Content-Type` is strictly compliant with `application/soap+xml`.
- [x] Support HTTP 400/500 status codes correctly as potentially containing valid SOAP Faults.

### Priority 5: Advanced Features
- [x] Hierarchical Subcodes (SOAP 1.2).
- [x] Response-Only MEP.
- [ ] Section 5 Encoding support (Not Planned - focus on Document/Literal).

---
*Updated on 2026-03-30*
