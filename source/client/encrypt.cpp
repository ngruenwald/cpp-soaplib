#include "encrypt.hpp"

#include <string.h>

#include <openssl/rsa.h>
#include <openssl/engine.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/rand.h>
#include <openssl/x509.h>

#include <soaplib/types/base64.h>

#if OPENSSL_VERSION_NUMBER < 0x10100005L
static int RSA_set0_key(
	RSA* r,
	BIGNUM* n,
	BIGNUM* e,
	BIGNUM* d)
{
	if (n != nullptr)
	{
		r->n = *n;
	}

	if (e != nullptr)
	{
		r->e = *e;
	}

	if (d != nullptr)
	{
		r->d = *d;
	}

	return 0;
}
#endif

unsigned char* fromBase64(
	const char* szInput,
	int* pLen)
{
	BIO *b64, *bmem;
	size_t length = strlen(szInput);
	// The length of BASE64 representation is always bigger
	// than the actual data length, so the size given to
	// the malloc below is sufficient to hold all the
	// decoded data
	unsigned char *buffer = (unsigned char*)malloc(length);

	b64 = BIO_new(BIO_f_base64());
	// No LF on the input string
	BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
	bmem = BIO_new_mem_buf((void*)szInput, length);
	bmem = BIO_push(b64, bmem);

	*pLen = BIO_read(bmem, buffer, length);
	BIO_free_all(bmem);

	return buffer;
}

BIGNUM* BN_fromBase64(
	const char* szBase64)
{
	BIGNUM* bn = NULL;
	int iLen;
	unsigned char* pbData = fromBase64(szBase64, &iLen);
	if (iLen)
	{
		bn = BN_bin2bn(pbData, iLen, NULL);
	}
	free(pbData);
	return bn;
}

RSA* RSA_fromBase64(const char* szModulus, const char* szExp)
{
	BIGNUM* n = BN_fromBase64(szModulus);
	BIGNUM* e = BN_fromBase64(szExp);

	if (!n) printf("Invalid encoding for modulus\n");
	if (!e) printf("Invalid encoding for public exponent\n");

	if (e && n)
	{
		RSA* rsa = RSA_new();
		RSA_set0_key(rsa, n, e, nullptr);
		return rsa;
	}
	else
	{
		if (n) BN_free(n);
		if (e) BN_free(e);
		return NULL;
	}
}

std::string encrypt(
	const std::string& modulus,
	const std::string& exponent,
	const std::string& plainText)
{
	auto rsa = RSA_fromBase64(modulus.c_str(), exponent.c_str());

	uint8_t plain[512];
	int idx = 0;
	for (const auto& c : plainText)
	{
		plain[idx++] = c;
		plain[idx++] = 0;
		plain[idx++] = 0;
		plain[idx++] = 0;
	}

	uint8_t cipher[512] = { 0 };

	int cipherLen =
		RSA_public_encrypt(
			idx,
			plain,
			cipher,
			rsa,
			RSA_PKCS1_PADDING
		);

	RSA_free(rsa);

	return base64_encode(cipher, cipherLen);
}
