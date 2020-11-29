🧼  cpp-soaplib
===================

This is a toolkit for C++ to create SOAP client applications.

It includes a generator _(soapgen)_ that will turn a service defintion (WSDL)
into C++ code. Currently it only supports single file definitions.

The library itself uses [cpp-httplib][1] for the basic HTTP handling.
[LibXml2][2] is used for the XML processing.


[[_TOC_]]


## Dependencies

* [cpp-httplib][1] (soaplib, _included_)
* [Catch2][5] (unit tests, _included_)
* [LibXml2][2] (soapgen, soaplib)
* [OpenSSL][6] (soaplib)


## TODO

- [ ] all types (int, string, etc.) should be derived from SoapType
- [ ] add operators for direct value access on simple types (single member and value types)
- [ ] abort if unknown type is detected
- [ ] use templates for code generation ([inja][3])
- [ ] use command line parameters for configuration ([cxxopts][4])
- [ ] use libxml2 directly, remove xmlwrp dependency
- [ ] replace shared_ptr with unique_ptr where possible
- [ ] add unit tests (remove client project)
- [ ] support for split definition files
- [ ] support file download in generator
- [ ] auto detect and resolve cyclic references
- [ ] c++ 11 support (currently built with c++ 17 enabled)

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
| cpp/pointer-types           | List of types that should be generated as pointer.             | optional  |         |                     |
| cpp/pointer-types/@param    | Name of type that should be generated as a pointer.            | optional  |         | --pointer-type      |


### Usage

Create configuration file (see [configuration section](#Configuration)).

Run with:
```bash
# soapgen config.xml
```


[logo]: docs/images/logo64.png "🧼"
[1]: https://github.com/yhirose/cpp-httplib
[2]: http://www.xmlsoft.org/
[3]: https://github.com/pantor/inja
[4]: https://github.com/jarro2783/cxxopts
[5]: https://github.com/catchorg/Catch2
[6]: https://www.openssl.org/
