#include <iostream>

#include <openssl/evp.h>
#include <openssl/err.h>

#include "country-info/CountryInfoService.hpp"

int main(
    int argc,
    char** argv)
{
    OpenSSL_add_all_algorithms();
    ERR_load_crypto_strings();

    auto svc = country::CountryInfoService{"http://webservices.oorsprong.org/websamples.countryinfo/CountryInfoService.wso"};
    svc.EnableHeader(false);

    auto input = country::FullCountryInfo{};
    input.SCountryISOCode = "AT";

    auto output = svc.FullCountryInfo(input);

    std::cout
        << "ISO Code: " << output.FullCountryInfoResult.SISOCode.str() << '\n'
        << "Name: " << output.FullCountryInfoResult.SName.str() << '\n'
        << "Capital City: " << output.FullCountryInfoResult.SCapitalCity.str() << '\n'
        << "Phone Code: " << output.FullCountryInfoResult.SPhoneCode.str() << '\n'
        << "Continent Code: " << output.FullCountryInfoResult.SContinentCode.str() << '\n'
        << "Currency ISO Code: " << output.FullCountryInfoResult.SCurrencyISOCode.str() << '\n'
        << "Country Flag: " << output.FullCountryInfoResult.SCountryFlag.str() << '\n'
        ;

    std::cout << "Languages:" << '\n';
    for (const auto& language : output.FullCountryInfoResult.Languages.TLanguage)
    {
        std::cout
            << "  - " << language.SName.str()
            << " (" << language.SISOCode.str() << ")" << '\n'
            ;
    }

    return 0;
}
