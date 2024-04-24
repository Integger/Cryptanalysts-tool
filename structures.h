#pragma once

#include "constants.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct {
	wchar_t** text;
	int textSize;

	int alreadySolvedIndex;
	wchar_t alreadySolved[SIZE_OF_ALPHABET];

	int replaceCapacity;
	wchar_t*** prevReplaces;
	int countOfReplaces;
} Config;