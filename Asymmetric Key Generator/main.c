#include <stdio.h>
#include <Windows.h>
#include "PrivateKeyset.h"
#include "PublicKey.h"

int main() {

	HCRYPTPROV hCryptoProvider;
	if (CryptAcquireContextA(&hCryptoProvider, NULL, NULL, PROV_RSA_FULL, CRYPT_NEWKEYSET)) {
		printf("\n(+) Succesfully acquired key container context - %p" ,hCryptoProvider);
	} else {
		printf("\n(!) Error acquiring key container context - %x", GetLastError());
	}

	HCRYPTKEY hKey;
	if (!CryptGenKey(hCryptoProvider, CALG_RSA_KEYX, CRYPT_EXPORTABLE, &hKey)) {
		printf("\n(!) Error generating key - %lu", GetLastError());
	} else {
		printf("\n(+) Succesfully generated key - %p", hKey);
	}

	priv(hKey);
	pub(hKey);

	//cleanup
	if (CryptAcquireContextA(&hCryptoProvider, NULL, NULL, PROV_RSA_FULL, CRYPT_DELETEKEYSET)) {
		printf("\n(+) Succesfully deleted key set");
	} else {
		printf("\n(!) Error deleting key set - %x", GetLastError());
	}

	if (!CryptReleaseContext(hCryptoProvider, 0)) {
		printf("\n(!) Error releasing crypto provider context - %lu", GetLastError());
	} else {
		printf("\n(+) Succesfully released crypto provider context");
	}

	if (!CryptDestroyKey(hKey)) {
		printf("\n(!) Error destroying key - %lu", GetLastError());
	} else {
		printf("\n(+) Succesfully destroyed key");
	}

	return 0;
}
