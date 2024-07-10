#include "headers.h"

extern void SplitString(char* result[], char* input, char delimiter, unsigned short maxSize);
extern void SplitStringAndFormat(char* result[], char* input, char delimiter, unsigned short maxSize);
extern void ConcatenateSplitString(char* results, char* input[], char seperator, unsigned short maxSize);
extern void TakeFormattedArguments(char* newString, char* oldString);
extern char* GetNextToken(char* token);