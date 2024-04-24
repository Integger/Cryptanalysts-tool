#include "safeFunctions.h"

void* safeCalloc(size_t size, size_t type) {
	void* toReturn = calloc(size, type);

	if (toReturn == NULL) exit(0); // Calloc error

	return toReturn;
}

void* safeRealloc(void* ptr, size_t size) {
	void* toReturn = realloc(ptr, size);

	if (toReturn == NULL) exit(0); // Realloc error

	return toReturn;
}

void safeAddCharToArr(wchar_t** addTo, wchar_t toAdd, int* currentCapacity, int* currentIndex) {
	if (*currentCapacity <= *currentIndex) {
		*currentCapacity += MIN_WORD_SIZE;

		*addTo = (wchar_t*)safeRealloc(*addTo, *currentCapacity * sizeof(wchar_t));
	}

	*((*addTo) + (*currentIndex)++) = toAdd;
}