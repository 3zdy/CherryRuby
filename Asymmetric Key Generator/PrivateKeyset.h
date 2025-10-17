#include <stdio.h>
#include <Windows.h>

int priv(HCRYPTKEY hKey) {

	DWORD keyLenCheck = NULL;
	if (!CryptExportKey(hKey, NULL, PRIVATEKEYBLOB, 0, NULL, &keyLenCheck)) {
		printf("\n(!) Error getting key size - 0x%x", GetLastError());
	} else {
		printf("\n(+) Succesfully got key size", keyLenCheck);
	}

	DWORD keyLen = keyLenCheck + 1;
	BYTE* keyData = (BYTE*)malloc(keyLen);
	if (!CryptExportKey(hKey, NULL, PRIVATEKEYBLOB, 0, keyData, &keyLen)) {
		printf("\n(!) Error exporting key - %i", GetLastError());
	} else {
		printf("\n(+) Succesfully exported key");
	}
	char keyHeader[] = "keyHex[] = {";
	int hexedKeySize = keyLen * 12 + sizeof(keyHeader) + 1;
	char* hexedKey = (char*)malloc(hexedKeySize);
	strcpy_s(hexedKey, hexedKeySize, keyHeader);

	for (DWORD i = 0; i < keyLen; i++) {

		BYTE hexedChar[10];
		sprintf_s(hexedChar, sizeof(hexedChar), "%02x", keyData[i]);

		strcat_s(hexedKey, hexedKeySize, " 0x");
		strcat_s(hexedKey, hexedKeySize, hexedChar);
		if ((keyLen - 1) != i){
			strcat_s(hexedKey, hexedKeySize, ",");
		} else {
			strcat_s(hexedKey, hexedKeySize, "}");
		}
	}

	char filePath[] = "C:\\Users\\iamsd\\Desktop\\PrivateKeyset.txt";
	HANDLE fileHandle = CreateFileA(filePath, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	if (fileHandle == NULL) {
		printf("\n(!) Failed to create file, Error: %lu", GetLastError());
	} else {
		printf("\n(+) Succesfully created file %s", filePath);
	}

	if (fileHandle != NULL) {
		DWORD writtenBytes = hexedKeySize;
	BOOL writeReturn = WriteFile(fileHandle, hexedKey, strlen(hexedKey), NULL, NULL);
		printf("\n(+) Succesfully put content in created file", filePath);
	} else {
		printf("\n(!) Failed put content in created file %s, Error: %lu", filePath, GetLastError());
	}

	free(keyData);
	free(hexedKey);
	CloseHandle(fileHandle);
	
	return 0;
}
