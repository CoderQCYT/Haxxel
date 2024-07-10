#include "headers.h"
#include "command.h"
#include "pointerLabels.h"

extern void SplitString(char* result[], char* input, char delimiter, unsigned short maxSize) {
	bool shouldSplit = true;
	unsigned int inputLength = strlen(input) + 1;
	unsigned short i = 0;
	unsigned short j = 0;
	unsigned short k = 0;
	char* tempPointer = (char*)calloc(1,inputLength);
	result[j] = tempPointer;
	input[strcspn(input, "\r\n")] = 0;
	while (input[k] != NULL) {
		if (input[k] == '\"') {
			shouldSplit = !shouldSplit;
		}
		else if (input[k] == delimiter && shouldSplit == true) {
			memset(&tempPointer[i], 0x00, 1);
			tempPointer = (char*)calloc(1, inputLength);
			i = 0;
			j++;
			result[j] = tempPointer;
		}
		else {
			memset(&tempPointer[i], input[k], 1);
			i++;
		}
		k++;
	}
}

extern void TakeFormattedArguments(char* newString, char* oldString) {
	unsigned int i = 0;
	bool isPartOfTag = false;
	unsigned char commandType = 0x0;
	char* commandValue = (char*)calloc(1, 300);
	commandValue[0] = '\00';

	if (oldString == NULL) {
		return;
	}

	for (i = 0; i < strlen(oldString); i++) {
		if (isPartOfTag == true) {
			if (*(oldString + i) == '>') {
				isPartOfTag = false;
				if (commandType == '}') {
					printf("%s", commandValue);
				}
				if (commandType == '^') {
					if (strcmp(commandValue, "lt") == 0) {
						strncat(newString, (char*)"<", 1);
					}
					if (strcmp(commandValue, "gt") == 0) {
						strncat(newString, (char*)">", 1);
					}

				}
				if (commandType == '~') {
					char** token = (char**)calloc(300, 1);

					const char* output = ProcessInput(commandValue, token, (unsigned short)300);
					strcat(newString, output);
				}
				if (commandType == 'x') {
					char* ans;

					sscanf(commandValue, "%x", &ans);
					strncat(newString, (char*)&ans, 1);
				}
				if (commandType == '{') {
					char input[64];
					printf("%s: ",commandValue);
					fgets(input, 64, stdin);
					input[strcspn(input, "\n")] = 0;
					strcat(newString, input);
				}
				if (commandType == '$') {
					for (unsigned short i = 0; i < maxPointerSize; i++) {
						struct PointerLabel tempPointerLabel = pointerLabels[i];
						if (tempPointerLabel.name == NULL && tempPointerLabel.pointer == NULL) {
							continue;
						}
						if (strcmp((tempPointerLabel.name), commandValue) == 0) {

							char pointer[12];
							sprintf(pointer, "0x%p", tempPointerLabel.pointer);
							strncat(newString, pointer, 9);
						}
					}
				}
				if (commandType == '*') {
					void* pointer = (void*)strtol(commandValue, NULL, 16);

					bool success = false;
#ifdef _WIN32
					if (IsBadReadPtr(pointer, strlen(commandValue)) == 0) {
						success = true;
					}
#elif __unix
					int nullfd = open("/dev/random", O_WRONLY);

					if (write(nullfd, pointer, 1) >= 0)
					{
						success = true;
					}
					close(nullfd);
#endif
					if (pointer == NULL) { success = false; }
					if (success == true) {
						strcat(newString, (char*)pointer);
					}
					else {
						strcat(newString, "????");
					}
				}
				continue;
			}
			if (commandType == NULL) {
				commandType = *(oldString + i);
			}
			else {
				strncat(commandValue, oldString + i, 1);
			}
		}
		else {
			if (*(oldString + i) == '<' && isPartOfTag == false) {
				isPartOfTag = true;
				continue;
			}
			strncat(newString, oldString + i, 1);
		}
	}
	free(commandValue);
}

extern void SplitStringAndFormat(char* result[], char* input, char delimiter, unsigned short maxSize) {
	bool shouldSplit = true;
	unsigned int inputLength = strlen(input) + 1;
	unsigned short i = 0; // index of temp sring
	unsigned short j = 0; // index into result
	unsigned short k = 0; // index of whole input string
	char* tempPointer = (char*)calloc(1, inputLength);
	input[strcspn(input, "\r\n")] = 0;
	while (input[k - 1] != NULL) {
		if (input[k] == '\"') {
			shouldSplit = !shouldSplit;
		}
		else if (input[k] == NULL || (input[k] == delimiter && shouldSplit == true)) {
			memset(&tempPointer[i], 0x00, 1);
			char* pointer = (char*)calloc(1, inputLength);
			TakeFormattedArguments(pointer, tempPointer);
			free(tempPointer);
			result[j] = pointer;
			tempPointer = (char*)calloc(1, inputLength);
			i = 0;
			j++;
		}
		else {
			memset(&tempPointer[i], input[k], 1);
			i++;
		}
		k++;
	}
}

const char quotationMark = '\"';

extern void ConcatenateSplitString(char* results, char* input[], char seperator, unsigned short maxSize) {
	unsigned int i = 0;
	while (input[i] != NULL) {
		strcat(results, input[i]);
		strcat(results, &seperator);
		i++;
	}
}


extern char* GetNextToken(char* token) {
	return token + strlen(token) + 1;
}