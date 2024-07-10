extern const char* ProcessCommand(const char* args[], unsigned short maxSize);
extern const char* ProcessInput(char* input, char* args[], unsigned short maxSize);
bool CheckWritePointer(void* pointer, unsigned short length);
unsigned short latestPointerLabel;