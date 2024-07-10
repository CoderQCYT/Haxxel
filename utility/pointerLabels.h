#ifndef pointerLabelHeader
#define pointerLabelHeader

typedef struct PointerLabel {
	char* name;
	void* pointer;
};

struct PointerLabel pointerLabels[128];
#define maxPointerSize 128
#endif 