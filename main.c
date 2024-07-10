#include "utility/headers.h"
#include "utility/command.h"

#include "utility/pointerLabels.h"




int main(int argc, char* argv[]) {

#ifdef _WIN32
	HANDLE handleConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTitle("Haxxel");
#endif
	char** token = (char*)calloc(8191, 1);

	if (token == NULL) {
		printf("Failed to allocate enough memory to hold essential values.");
		return 137;
	}

	initalizePointerLabels();

	if (argv[1] != NULL) {
		char* dot = strrchr(argv[1], '.');
		if (dot && !strcmp(argv[1], ".hxl")) {
			FILE* fp;
			fp = fopen(argv[1], "r");
			if (fp == NULL) {
				perror("Failed: ");
				return 1;
			}

			char buffer[8191];
			while (fgets(buffer, 8191, fp))
			{
				// Remove trailing newline
				buffer[strcspn(buffer, "\n")] = 0;
				ProcessInput(buffer, token, 8191);
			}

			fclose(fp);
		}
	}

	printf("Haxxel Beta\n");
	printf("By: @CoderQC\n"); // Replace this line once it's open source :D
	char* input;
	while (true) {
		input = calloc(8191,1);
		printf("> ");
		fgets(input, 8191, stdin);
		ProcessInput(input, token, 8191);
		memset(token, 0x0, 8191);
	}
	free(input);
	free(token);
	return 0;
}
