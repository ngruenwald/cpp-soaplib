#include <iostream>

#include "AuthenticationService.hpp"
#include "IAuthenticationService.h"

#include "encrypt.hpp"

#include <openssl/evp.h>
#include <openssl/err.h>

#include "admin/authentication/AuthenticationService.hpp"
#include "admin/authentication/types/GetPublicKeyDCResponse.hpp"

class MyAuthService
	: public admin::authentication::AuthenticationService
{
public:
	MyAuthService(
		const std::string& serviceAddress)
		: admin::authentication::AuthenticationService(serviceAddress)
	{
	}


};

void test2()
{
	OpenSSL_add_all_algorithms();
	ERR_load_crypto_strings();

	try
	{
#if 0
		MyAuthenticationService service("http://192.168.68.7:8091/SR_1_17/AWS/AuthenticationService.svc");

		GetPublicKey gpk;
		GetPublicKeyResponse gpkr = service.GetPublicKey(gpk);
		std::cout << "public key: " << gpkr.result.value << '\n';
#endif
		//service.Login("Dispatcher", "123456");
		//std::cout << "token: " << service.Token() << '\n';

		//service.LogOff();
#if 1
		MyAuthService service("http://192.168.68.7:8091/SR_1_17/AWS/AuthenticationService.svc");

		admin::authentication::GetPublicKey gpk;
		admin::authentication::GetPublicKeyResponse gpkr = service.GetPublicKey(gpk);

		std::cout << "public key: " << *gpkr.GetPublicKeyResult->PublicKey << '\n';
#endif
	}
	catch (const std::exception& ex)
	{
		std::cerr << ex.what() << '\n';
	}
}

int main(
    int argc,
    char** argv)
{
	//test1();
	test2();

    return 0;
}
