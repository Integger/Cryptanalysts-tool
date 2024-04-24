#pragma once

#include "constants.h"

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

void* safeCalloc(size_t, size_t);

void* safeRealloc(void*, size_t);

void safeAddCharToArr(wchar_t**, wchar_t, int*, int*);
