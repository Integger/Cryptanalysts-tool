#pragma once

#include "safeFunctions.h"

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

typedef union {
	char Char;
	wchar_t Wchar_t;
	wchar_t* Word;
	int Int;
	double Double;
} VectorObject;

typedef struct VectorElement {
	VectorObject* element;

	struct VectorElement* next;
} VectorElement;

typedef struct {
	VectorElement* top;
	VectorElement* current;
} Vector;

VectorElement* initVectorElement(void);

Vector* initVector(void);

void pushBack(Vector*, VectorObject);