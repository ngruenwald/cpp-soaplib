#include <iostream>

#include <openssl/evp.h>
#include <openssl/err.h>

#include "calc/Calculator.hpp"

int main(
    int argc,
    char** argv)
{
    OpenSSL_add_all_algorithms();
    ERR_load_crypto_strings();

    auto calc = calc::Calculator{"http://www.dneonline.com/calculator.asmx"};
    calc.EnableHeader(false);

    auto input = calc::Add{};
    input.IntA = 3;
    input.IntB = 5;
    auto output = calc.Add(input);
    std::cout << output.AddResult << '\n';

    return 0;
}
