#pragma once
#include <Windows.h>

typedef struct keyStructure {
	HCRYPTPROV hCryptoProvider;
	HCRYPTKEY hKey;
}key;
