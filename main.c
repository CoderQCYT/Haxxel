#include "utility/headers.h"
#include "utility/command.h"

#include "utility/pointerLabels.h"
#include "lua_handler.h"



int main(int argc, char* argv[]) {
#ifdef _WIN32
	HANDLE handleConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTitle("Haxxel");
#endif
	char** token = (char*)calloc(1,8191);

	if (token == NULL) {
		printf("Failed to allocate enough memory to hold essential values.");
		return 137;
	}

	initalizePointerLabels();


	if (argv[1] != NULL && strcmp(argv[1], "--file") == 0) {
		if (argv[2] == NULL) {
			printf("Please specify a file.\n\n");
			exit(1);
		}
		else {
			FILE* fp;
			fp = fopen(argv[2], "r");
			if (fp == NULL) {
				perror("Failed: ");
				exit(1);
			}

			char buffer[8191];
			while (fgets(buffer, 8191, fp))
			{
				// Remove trailing newline
				buffer[strcspn(buffer, "\n")] = 0;
				ProcessInput(buffer, token, 8191);
			}

			fclose(fp);
			exit(0);
		}
	}

	if (argv[1] != NULL && strcmp(argv[1], "--lua") == 0) {
		if (argv[2] == NULL) {
			printf("Please specify a Lua script.\n\n");
			exit(1);
		}
		else {
			FILE* fp;
			fp = fopen(argv[2], "r");
			if (fp == NULL) {
				perror("Failed: ");
				exit(1);
			}

			RunHaxxelLuaFile(argv[2]);

			fclose(fp);
			exit(0);
		}
	}

	printf("Haxxel Beta\n");
	printf("By: @CoderQC\n"); // Replace this line once55 it's open source :D
	char* input = calloc(1,8191);
	while (true) {

		// Maybe handle arrow keys for Linux sometime soon, but ncurses doesn't work for me.

		printf("> ");
		fgets(input, 8191, stdin);
		ProcessInput(input, token, 8191);
		memset(token, 0x0, 8191);
	}
	free(input);
	free(token);
	return 0;
}
