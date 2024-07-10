#include "headers.h"


#ifndef commandHeaders
#define commandHeaders
extern const char* ProcessCommand(const char* args[], unsigned short maxSize);
extern const char* ProcessInput(char* input, char* args[], unsigned short maxSize);
extern const char* ProcessInput(char* input, char** args, unsigned short maxSize);
extern bool CheckWritePointer(void* pointer, unsigned short length);
#endif