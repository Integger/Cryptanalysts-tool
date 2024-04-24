#pragma once

#include "vector.h"
#include "map.h"
#include "constants.h"
#include "structures.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <wchar.h>

bool compareWChar(ObjectMap*, void*);

bool compareInt(ObjectMap*, void*);

bool isWCharInUpperCase(wchar_t);

bool isWCharInLowerCase(wchar_t);

wchar_t getWChar();

bool isDigit(wchar_t);

bool isSpecialSymbol(wchar_t);

bool canBeProcessed(wchar_t);

void copyArr(char*, char*, int);