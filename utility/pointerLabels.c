#include "headers.h"
#include "pointerLabels.h"

struct PointerLabel *pointerLabels;
unsigned int nextPointerLabel = 0;

extern void initalizePointerLabels() {
	pointerLabels = (char*)calloc(128, sizeof(struct PointerLabel));
	

	if (pointerLabels == NULL) {
		printf("Failed to allocate memory for pointer labels!");
	}
}

extern void createPointerLabel(void* pointer, char* tempName) {
	char* name = calloc(1, 64);
	strncpy(name, tempName, 64);
	struct PointerLabel pLabel = { name, pointer };
	pointerLabels[nextPointerLabel] = pLabel;
	nextPointerLabel++;
}

extern void deletePointerLabel(char name[64]) {
	unsigned short i;
	for (i = 0; i < nextPointerLabel; i++) {
		struct PointerLabel tempPointerLabel = pointerLabels[i];
		if (strcmp((tempPointerLabel.name), name) == 0) {
			memset(&tempPointerLabel, 0x0, sizeof(tempPointerLabel));
		}
	}
}

extern void* getPointerAtLabel(char* name) {
	for (unsigned short i = 0; i < maxPointerSize; i++) {
		struct PointerLabel tempPointerLabel = pointerLabels[i];
		if (tempPointerLabel.name == NULL && tempPointerLabel.pointer == NULL) {
			continue;
		}
		if (strcmp((tempPointerLabel.name), name) == 0) {
			return tempPointerLabel.pointer;
		}
	}
	return 0x0;
}