.. cpp-soaplib documentation master file, created by
   sphinx-quickstart on Fri Aug 19 20:08:03 2022.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Welcome to cpp-soaplib's documentation!
=======================================

.. toctree::
   :maxdepth: 2
   :caption: Contents:

   api/library_root

   README
   CHANGELOG
   LICENSE

:ref:`genindex`


🧼  cpp-soaplib
===============

This is a toolkit for C++ to create SOAP client applications.

It includes a generator *(soapgen)* that will turn a service defintion (WSDL)
into C++ code. Currently it only supports single file definitions.

The library itself uses `cpp-httplib` for the basic HTTP handling.
`LibXml2` is used for the XML processing.


| **WARNING**
| This project was written to access some custom WCF SOAP services.
| I have never read any spec, nor have I tested this against other services.
| Do not expect this to work!  ¯\\_(ツ)_/¯


.. _cpp-httplib: https://github.com/yhirose/cpp-httplib
.. _LibXml2: http://www.xmlsoft.org/
