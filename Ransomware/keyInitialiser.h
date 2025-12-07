#include <stdio.h>
#include <Windows.h>
#include "keyDef.h"

key setupKeys() {

	key keyData;
	if (CryptAcquireContextA(&keyData.hCryptoProvider, NULL, NULL, PROV_RSA_AES, CRYPT_NEWKEYSET | CRYPT_VERIFYCONTEXT)) {
		printf("\n(+) Succesfully acquired key container context");
	} else if (!CryptAcquireContextA(&keyData.hCryptoProvider, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)) {
		printf("\n(!) Error acquiring key container context - 0x%x", GetLastError());
	}

	keyData.hKey = 0;
	if (!CryptGenKey(keyData.hCryptoProvider, CALG_AES_256, CRYPT_EXPORTABLE, &keyData.hKey)) {
		printf("\n(!) Error generating key - %lu", GetLastError());
	} else {
		printf("\n(+) Succesfully generated key", keyData.hKey);
	}

	return keyData;
}
