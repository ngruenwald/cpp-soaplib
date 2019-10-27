#include <iostream>
#include "wsdl.hpp"
#include "cppgen/cppgen.hpp"

int main(int argc, const char** argv)
{
    const char* wsdlFile = "service.wsdl";

    if (argc > 1)
    {
        wsdlFile = argv[1];
    }

    auto definition = LoadWsdl(wsdlFile);

    if (definition)
    {
        std::cout << "load succeeded" << '\n';

        //RedundancyCheck(*definition);

        cppgen::Options options;
        options.namespaces = { "admin", "authentication" };

        cppgen::Generate(options, *definition.get());
    }

    return 0;
}
