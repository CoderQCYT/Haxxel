struct PointerLabel {
	char* name;
	void* pointer;
};

struct PointerLabel pointerLabels[128];
int latestPointerLabel;


extern void initalizePointerLabels();
#define maxPointerSize 128
