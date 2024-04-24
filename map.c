#include "map.h"

MapElement* initMapElement(void) {
	MapElement* mapElement = (MapElement*) safeCalloc(1, sizeof(MapElement));

	mapElement->key = NULL;
	mapElement->value = NULL;

	mapElement->next = NULL;

	return mapElement;
}

Map* initMap(void) {
	Map* map = (Map*) safeCalloc(1, sizeof(Map));

	map->top = NULL;
	map->current = NULL;

	return map;
}


void copyMapElement(MapElement* from, MapElement* to) {
	to->key = from->key;
	to->value = from->value;
	to->next = from->next;
}

bool isMapEmpty(Map* map) {
	return map->top == NULL;
}

void insertMap(Map* map, ObjectMap key, ObjectMap value) {
	MapElement* newMapElement = initMapElement();

	newMapElement->key = safeCalloc(1, sizeof(ObjectMap));
	newMapElement->value = safeCalloc(1, sizeof(ObjectMap));

	memcpy(newMapElement->key, &key, sizeof(ObjectMap));
	memcpy(newMapElement->value, &value, sizeof(ObjectMap));

	if (map->top == NULL) {
		map->top = newMapElement;
		map->current = map->top;
	}
	else if (map->current->next == NULL) {
		map->current->next = newMapElement;

		map->current = map->current->next;
	}
}

void freeMap(Map** map) {
	(*map)->current = NULL;

	while ((*map)->top != NULL) {
		MapElement* tmp = (*map)->top->next;

		free((*map)->top->key);
		free((*map)->top->value);

		(*map)->top = tmp;
	}

	free((*map));

	*map = NULL;
}

typedef bool (*CompareFunction) (ObjectMap* data, void* value);

MapElement* getElementByKey(Map* map, void* key, CompareFunction compare) {
	MapElement* iterator = map->top;

	while (iterator != NULL) {
		if (compare(iterator->key, key)) return iterator;

		iterator = iterator->next;
	}

	return NULL;
}

Map* removeElement(MapElement* top, MapElement** toRemove) {
	if (*toRemove == NULL) exit(0); // If we want to remove element from NULL (empty List)

	if ((*toRemove)->next == NULL) {
		if (top == NULL) return NULL;

		if (*toRemove != top) {
			MapElement* iterator = top;

			while (iterator->next != *toRemove) iterator = iterator->next;

			iterator->next = NULL;
		}

		// freeLinkedListElement(toRemove);

		return NULL;
	}
	else {
		MapElement* tmp = (*toRemove)->next;

		copyMapElement((*toRemove)->next, (*toRemove));

		(*toRemove)->next = tmp->next;

		// freeMapElement(&tmp);

		return *toRemove;
	}
}

void swap(MapElement*** first, MapElement*** second) {
	MapElement** tmp = *first;
	*first = *second;
	*second = tmp;
}

void bubbleSortByValue(MapElement** arr, int n) {
	for (int i = 0; i < n - 1; i++) {
		for (int j = 0; j < n - i - 1; j++) {
			if (arr[j]->value->Int < arr[j + 1]->value->Int) {
				swap(&arr[j], &arr[j + 1]);
			}
		}
	}
}

void bubbleSortByKey(MapElement** arr, int n) {
	for (int i = 0; i < n - 1; i++) {
		for (int j = 0; j < n - i - 1; j++) {
			if (arr[j]->key->Int < arr[j + 1]->key->Int) {
				swap(&arr[j], &arr[j + 1]);
			}
		}
	}
}