#pragma once

#include "vector.h"
#include "structures.h"
#include "constants.h"
#include "safeFunctions.h"

#include <stdio.h>
#include <wchar.h>
#include <stdbool.h>

typedef union {
	char Char;
	wchar_t Wchar_t;
	int Int;
	double Double;
	Vector* vector;
} ObjectMap;

typedef struct MapElement {
	ObjectMap* key;
	ObjectMap* value;

	struct MapElement* next;
} MapElement;

typedef struct {
	MapElement* top;
	MapElement* current;
} Map;

MapElement* initMapElement(void);

Map* initMap(void);

void copyMapElement(MapElement*, MapElement*);

bool isMapEmpty(Map*);

void insertMap(Map*, ObjectMap, ObjectMap);

void freeMap(Map**);

typedef bool (*CompareFunction) (ObjectMap*, void*);

MapElement* getElementByKey(Map*, void*, CompareFunction);

Map* removeElement(MapElement*, MapElement**);

void swap(MapElement***, MapElement***);

void bubbleSortByValue(MapElement**, int);

void bubbleSortByKey(MapElement**, int);