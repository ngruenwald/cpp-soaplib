🧼  cpp-soaplib
===================

This is a toolkit for C++ to create SOAP client and server applications.

It includes a generator _(soapgen)_ that will turn a service defintion (WSDL)
into C++ code. Currently it only supports single file definitions.

The library itself uses [cpp-httplib][1] for the basic HTTP handling (optional).
[LibXml2][2] is used for the XML processing.

__WARNING__:
_This project was written to access some custom WCF SOAP services.
I have never read any spec, nor have I tested this against other services.
Do not expect this to work!_ ¯\\_(ツ)_/¯


[[_TOC_]]


## Dependencies

* [cpp-httplib][1] (soaplib, _included_, optional transport)
* [Catch2][5] (unit tests, _included_)
* [LibXml2][2] (soapgen, soaplib)
* [OpenSSL][6] (soaplib)


## TODO

- [x]  all types (int, string, etc.) should be derived from _SoapLibBaseType_
- [~]  add operators for direct value access on simple types (single member and value types)
- [ ]  implement missing data types
- [ ]  abort if unknown type is detected
- [x]  use templates for code generation ([inja][3])
- [x]  use command line parameters for configuration ([argparse][4])
- [x]  use libxml2 directly, remove xmlwrp dependency
- [x]  replace shared_ptr with unique_ptr where possible (soaplib and generated code)
- [x]  add unit tests
- [x]  support for SOAP server stub generation
- [x]  decouple transport layer (HTTP, WebSockets, etc.) for client and server
- [ ]  support for split definition files
- [ ]  support file download in generator
- [ ]  auto detect and resolve cyclic references
- [ ]  cleanup cmake scripts
- [ ]  generate doxygen files

----

## soapgen

### Configuration

```xml
<config>
  <wsdl path="wsdl/FooService.wsdl" />
  <cpp>
    <output path="_output/foo" append-ns="false" types-subfolder="types" />
    <namespaces>
      <namespace>admin</namespace>
      <namespace>authentication</namespace>
    </namespaces>
    <cmake namespace="foo" export="foolib" />
    <client enable="true" />
    <server enable="true" />
    <pointer-types>
      <param>Replacements</param>
    </pointer-types>
  </cpp>
</config>
```

| Parameter                   | Description                                                    | Usage     | Default | Command line        |
|-----------------------------|----------------------------------------------------------------|-----------|---------|---------------------|
| wsdl/@path                  | Path to the input wsdl file.                                   | mandatory |         | --input             |
| cpp/output/@path            | Output path for generated files.                               | optional  | _output | --output            |
| cpp/output/@append-ns       | Create subdirectories for namespaces.                          | optional  | true    | --namespace-folders |
| cpp/output/@types-subfolder | If set, will create a subfolder for types with the given name. | optional  | types   | --types-folder      |
| cpp/namespaces              | List of namespace elements. Namespaces are applied in order.   | optional  |         |                     |
| cpp/namespaces/namespace    | Namespace value.                                               | multiple  |         | --namespace         |
| cpp/cmake/@namespace        | Namespace for generated CMake targets.                         | optional  |         | --cmake-namespace   |
| cpp/cmake/@export           | Name for CMake export.                                         | optional  |         | --cmake-export      |
| cpp/client/@enable          | Enable generation of client proxy.                             | optional  | true    |                     |
| cpp/server/@enable          | Enable generation of server stubs.                             | optional  | false   |                     |
| cpp/pointer-types           | List of types that should be generated as pointer.             | optional  |         |                     |
| cpp/pointer-types/@param    | Name of type that should be generated as a pointer.            | optional  |         | --pointer-type      |


### Usage

Run with:
```bash
# soapgen config.xml
```

## Transport Decoupling

The library is designed to be transport-agnostic. SOAP logic (XML processing and dispatching) is separated from how bytes are moved over the wire.

### Client Side

By default, the generated client uses `HttpSoapTransport`. You can provide your own by implementing the `SoapTransport` interface:

```cpp
class MyCustomTransport : public soaplib::SoapTransport {
    std::unique_ptr<xml::Document> Send(const xml::Document& req, int timeout) override {
        // Your custom logic (e.g. WebSockets, Message Queue)
    }
    // ... other methods
};

auto transport = std::make_unique<MyCustomTransport>();
MyService service(std::move(transport), "http://namespace");
```

### Server Side

The generated server stub (`HandleRequest`) takes an `xml::Document` and returns one. You can wrap this in any server:

1. **HTTP**: Use the built-in `HttpSoapServer`.
2. **Custom**: Call `myService.HandleRequest(*incomingDoc)` manually from your transport handler.

---

[logo]: docs/images/logo64.png "🧼"
[1]: https://github.com/yhirose/cpp-httplib
[2]: http://www.xmlsoft.org/
[3]: https://github.com/pantor/inja
[4]: https://github.com/p-ranav/argparse
[5]: https://github.com/catchorg/Catch2
[6]: https://www.openssl.org/
