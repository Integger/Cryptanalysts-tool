#include "functions.h"

bool compareWChar(ObjectMap* first, void* second) {
	return first->Wchar_t == *((wchar_t*) second);
}

bool compareInt(ObjectMap* first, void* second) {
	return first->Int == *((int*) second);
}

bool isWCharInUpperCase(wchar_t wch) {
	return wch >= L'À' && wch <= L'ß';
}

bool isWCharInLowerCase(wchar_t wch) {
	return wch >= L'à' && wch <= L'ÿ';
}

wchar_t getWChar() {
	wchar_t result = getwchar();

	return result;
}

bool isDigit(wchar_t wch) {
	return wch >= L'0' && wch <= L'9';
}

bool isSpecialSymbol(wchar_t wch) {
	return wch == L' ' || wch == L'.' || wch == L'!' || wch == L',' || wch == L'\n' || wch == L'*' || wch == L'/' || wch == L'&' || wch == L';' || wch == L'-' || wch == L':' || wch == L'(' || wch == L')' || wch == L'?';
}

bool canBeProcessed(wchar_t wch) {
	return isDigit(wch) || isWCharInLowerCase(wch) || isWCharInUpperCase(wch) || isSpecialSymbol(wch);
}

void copyArr(char* from, char* to, int count) {
	for (int i = 0; i < count; ++i) {
		to[i] = from[i];
	}
}