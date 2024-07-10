struct PointerLabel {
	char* name;
	void* pointer;
};


extern void initalizePointerLabels();
extern void createPointerLabel(void* pointer, char* tempName);
extern void deletePointerLabel(char name[64]);
extern void* getPointerAtLabel(char name[64]);
#define maxPointerSize 128
