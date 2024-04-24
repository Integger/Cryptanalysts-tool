#include "vector.h"

VectorElement* initVectorElement(void) {
	VectorElement* vectorElement = (VectorElement*)safeCalloc(1, sizeof(VectorElement));

	vectorElement->element = NULL;
	vectorElement->next = NULL;

	return vectorElement;
}

Vector* initVector(void) {
	Vector* vector = (Vector*)safeCalloc(1, sizeof(Vector));

	vector->top = NULL;
	vector->current = NULL;

	return vector;
}

void pushBack(Vector* vector, VectorObject object) {
	VectorElement* newVectorElement = initVectorElement();

	newVectorElement->element = safeCalloc(1, sizeof(VectorObject));

	memcpy(newVectorElement->element, &object, sizeof(VectorObject));

	if (vector->top == NULL) {
		vector->top = newVectorElement;
		vector->current = vector->top;
	}
	else if (vector->current->next == NULL) {
		vector->current->next = newVectorElement;

		vector->current = vector->current->next;
	}
}