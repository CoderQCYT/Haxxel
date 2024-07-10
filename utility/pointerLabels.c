#include "headers.h"
#include "pointerLabels.h"

void initalizePointerLabels() {
	int latestPointerLabel = 0;

	char** pointerLabels = (char*)calloc(128, sizeof(struct PointerLabel));
	if (pointerLabels == NULL) {
		printf("Failed to allocate memory for pointer labels!");
	}
}