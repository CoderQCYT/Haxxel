#include "headers.h"
#include "command.h"
#include "split.h"
#include "pointerLabels.h"


extern bool CheckWritePointer(void* pointer, unsigned short length) {
	bool success = false;
#ifdef _WIN32
	if (IsBadWritePtr(pointer, length) == 0) {
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
	return success;
}


#ifdef __unix__
static int exec_prog(const char** argv)
{
	pid_t   my_pid;
	int     status, timeout /* unused ifdef WAIT_FOR_COMPLETION */;

	if (0 == (my_pid = fork())) {
		if (-1 == execve(argv[0], (char**)argv, NULL)) {
			perror("child process execve failed [%m]");
			return -1;
		}
	}

#ifdef WAIT_FOR_COMPLETION
	timeout = 1000;

	while (0 == waitpid(my_pid, &status, WNOHANG)) {
		if (--timeout < 0) {
			perror("timeout");
			return -1;
		}
		sleep(1);
	}

	printf("%s WEXITSTATUS %d WIFEXITED %d [status %d]\n",
		argv[0], WEXITSTATUS(status), WIFEXITED(status), status);

	if (1 != WIFEXITED(status) || 0 != WEXITSTATUS(status)) {
		perror("%s failed, halt system");
		return -1;
	}

#endif
	return 0;
}
#endif


extern const char* ProcessCommand(const char* args[], const unsigned short maxSize) {
	char* output = (char*)calloc(1, 150);
	bool commandProcessed = false;
	if (strcmp(args[0], "debug") == 0) {
		unsigned short i = 0;
		while (args[i] != NULL) {
			printf("Arg %d: %s\n", i, args[i]);
			i++;
		}
		commandProcessed = true;
	}
	if (strcmp(args[0], "exit") == 0) {
		exit(0);
	}
	if (strcmp(args[0], "help") == 0) {
		printf("List of basic commands:\n");
		printf("  cd <dir> - Changes the current directory.\n");
		printf("  dir - Lists files and folders in this directory.\n");
		printf("  clear - Clears the console.\n");
		printf("  echo <message> - Prints a message to the console.\n");
		if (args[1] != NULL && strcmp(args[1], "all") == 0) {
			printf("\nList of commands involving pointers:\n");
			printf("  memset <pointer> <value> - Sets a specified memory location to a specified value.\n");
			printf("  alloc <size> - Sets a specified memory location to a specified value.\n");
			printf("  free <pointer> - Frees an allocated memory space. (can crash on Unix systems if not careful!)\n");
			printf("  plabel <pointer> <name> - Labels a pointer with a specified name.\n");
			printf("  punlabel <name> - Removes the pointer label specified.\n");
			printf("  add <pointer> <arg1> [arg2] - Adds the 2 specified numbers, or adds `arg1` to the value of `pointer`.\n");
			printf("  subtract <pointer> <arg1> [arg2] - Subtracts the 2 specified numbers, or subtracts `arg1` to the value of `pointer`.\n");
			printf("  multiply <pointer> <arg1> [arg2] - Multiplies the 2 specified numbers, or multiplies `arg1` to the value of `pointer`.\n");
			printf("  divide <pointer> <arg1> [arg2] - Divides the 2 specified numbers, or divides `arg1` to the value of `pointer`.\n");
			printf("\nText formatting:\n");
			printf(" You can use formatting codes by using <tValue> in an input, where `t` is the type, and `Value` is the value.\n");
			printf("  '}' - Prints the value to the screen\n");
			printf("  '{' - Takes input from the user.\n");
			printf("  '^lt' - Gets replaced with '<'.\n");
			printf("  '^gt' - Gets replaced with '>'.\n");
			printf("  '~' - Runs the command specified.\n");
			printf("  'xXX' - Gets replaced with the specified ASCII character.\n");
			printf("  '*' - Reads a string value from the specified pointer.\n");
			printf("  '#' - Reads an integer value from the specified pointer.\n");
			printf("  'l' - Reads a string value from the specified pointer label.\n");
			printf("  'L' - Reads an integer value from the specified pointer label.\n");
;		}
		else {
			printf("\nIf you want a list of all commands, type \"help all\".\n");
		}
		commandProcessed = true;
	}
	if (strcmp(args[0], "cd") == 0) {
		if (strcmp(args[1], "") == 0) {
			char s[256];
			printf("%s\n", getcwd(s, 256));
		}
		else {
			if (chdir(args[1]) != 0) {
				perror("");
			}
		}
		commandProcessed = true;
	}
	if (strcmp(args[0], "dir") == 0) {
		DIR* d;
		struct dirent* dir;
		d = opendir(".");
		if (d) {
			char s[256];
			printf("Directory of: %s\n", getcwd(s, 256));
			while ((dir = readdir(d)) != NULL) {
				if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0) {
					continue;
				}
				printf("	%s\n", dir->d_name);
			}
			closedir(d);
		}
		commandProcessed = true;
	}
	if (strcmp(args[0], "clear") == 0) {
#ifdef __unix__
		printf("\033[2J");
		printf("\x1b[H\x1b[J");
#elif _WIN32
		COORD startPos = { 0, 0 };
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO screen;
		DWORD written;

		GetConsoleScreenBufferInfo(hConsole, &screen);
		FillConsoleOutputCharacterA(hConsole, ' ', screen.dwSize.X * screen.dwSize.Y, startPos, &written);
		FillConsoleOutputAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE, screen.dwSize.X * screen.dwSize.Y, startPos, &written);
		SetConsoleCursorPosition(hConsole, startPos);
#endif
		commandProcessed = true;
	}
	if (strcmp(args[0], "echo") == 0) {
		strncpy(output, args[1], 150);
		printf("%s\n", args[1]);
		commandProcessed = true;
	}
	if (strcmp(args[0], "memset") == 0) {
		void* pointer = (void*)strtol(args[1], NULL, 16);
		if (pointer != NULL) {
			bool success = CheckWritePointer(pointer, strlen(args[1]));
			if (success == true) {
				memcpy(pointer, args[2], strlen(args[2]));
			}
			else {
				printf("Failed to write to memory.\n");
			}
		}
		else {
			printf("Please specify a location in memory.\n");
		}
		commandProcessed = true;
	}
	if (strcmp(args[0], "alloc") == 0) {
		commandProcessed = true;
		unsigned long size = atoi(args[1]);
		if (size > 0) {
			void* pointer = calloc(1, size);
			if (pointer != NULL) {
				printf("Allocated an address: %p\n", pointer);
				sprintf(output, "%p", pointer);
			}
			else {
				printf("Failed to allocate.\n");
			}

		}
		else {
			printf("Please enter how much memory you want to allocate.\n");
		}
	}
	if (strcmp(args[0], "free") == 0) {
		commandProcessed = true;
		void* pointer = (void*)strtol(args[1], NULL, 16);
		if (pointer != NULL) {
#ifdef _WIN32
			if (_CrtIsValidHeapPointer(pointer)) {
				free(pointer);
			}
			else {
				printf("Address is not in the heap.\n");
			}
#elif __unix__
			printf("Note: The free command is not fully complete on Unix systems.\n");
			free(pointer);
#endif
		}
		else {
			printf("Please specify a location in memory.\n");
		}
	}
	if (strcmp(args[0], "plabel") == 0) {
		commandProcessed = true;
		if (args[1] != NULL) {
			void* pointer = (void*)strtol(args[1], NULL, 16);
#ifdef _WIN32
			if (!_CrtIsValidPointer(pointer, 1, 1)) {
				printf("Pointer is invalid.\n");
				goto exitCommand;
			}
#endif
			if (args[2] != NULL) {
				if (strlen(args[2]) < 64) {
					createPointerLabel(pointer, args[2]);
				}
				else {
					printf("Label name is too long.\n");
				}

			}
			else {
				printf("Please specify a name for the label.\n");
			}
		}
		else {
			printf("Please specify a location in memory.\n");
		}
	}
	if (strcmp(args[0], "punlabel") == 0) {
		commandProcessed = true;

	}
	if (strcmp(args[0], "add") == 0) {
		commandProcessed = true;
		if (args[1] != NULL) {
			void* pointer = (void*)strtol(args[1], NULL, 16);
			if (args[2] != NULL) {
				if (args[3] != NULL) {
					unsigned int number = atoi(args[2]) + atoi(args[3]);
					bool success = CheckWritePointer(pointer, strlen(args[1]));
					if (success == true) {
						memcpy(pointer, &number, sizeof(int));
					}
					else {
						printf("Failed to write to memory.\n");
					}
				}
				else {
					bool success = CheckWritePointer(pointer, strlen(args[1]));
					if (success == true) {
						unsigned int number = *((int*)pointer) + atoi(args[2]);
						memcpy(pointer, &number, sizeof(int));
					}
					else {
						printf("Failed to write to memory.\n");
					}
				}
			}
			else {
				printf("Please specify the first number.\n");
			}
		}
		else {
			printf("Please specify a location in memory.\n");
		}
	}
	if (strcmp(args[0], "subtract") == 0) {
		commandProcessed = true;
		if (args[1] != NULL) {
			void* pointer = (void*)strtol(args[1], NULL, 16);
			if (args[2] != NULL) {
				if (args[3] != NULL) {
					unsigned int number = atoi(args[2]) - atoi(args[3]);
					bool success = CheckWritePointer(pointer, strlen(args[1]));
					if (success == true) {
						memcpy(pointer, &number, sizeof(int));
					}
					else {
						printf("Failed to write to memory.\n");
					}
				}
				else {
					bool success = CheckWritePointer(pointer, strlen(args[1]));
					if (success == true) {
						unsigned int number = *((int*)pointer) - atoi(args[2]);
						memcpy(pointer, &number, sizeof(int));
					}
					else {
						printf("Failed to write to memory.\n");
					}
				}
			}
			else {
				printf("Please specify the first number.\n");
			}
		}
		else {
			printf("Please specify a location in memory.\n");
		}
	}
	if (strcmp(args[0], "multiply") == 0) {
		commandProcessed = true;
		if (args[1] != NULL) {
			void* pointer = (void*)strtol(args[1], NULL, 16);
			if (args[2] != NULL) {
				if (args[3] != NULL) {
					unsigned int number = atoi(args[2]) * atoi(args[3]);
					bool success = CheckWritePointer(pointer, strlen(args[1]));
					if (success == true) {
						memcpy(pointer, &number, sizeof(int));
					}
					else {
						printf("Failed to write to memory.\n");
					}
				}
				else {
					bool success = CheckWritePointer(pointer, strlen(args[1]));
					if (success == true) {
						unsigned int number = *((int*)pointer) * atoi(args[2]);
						memcpy(pointer, &number, sizeof(int));
					}
					else {
						printf("Failed to write to memory.\n");
					}
				}
			}
			else {
				printf("Please specify the first number.\n");
			}
		}
		else {
			printf("Please specify a location in memory.\n");
		}
	}
	if (strcmp(args[0], "divide") == 0) {
		commandProcessed = true;
		if (args[1] != NULL) {
			void* pointer = (void*)strtol(args[1], NULL, 16);
			if (args[2] != NULL) {
				if (args[3] != NULL) {
					unsigned int number = atoi(args[2]) / atoi(args[3]);
					bool success = CheckWritePointer(pointer, strlen(args[1]));
					if (success == true) {
						memcpy(pointer, &number, sizeof(int));
					}
					else {
						printf("Failed to write to memory.\n");
					}
				}
				else {
					bool success = CheckWritePointer(pointer, strlen(args[1]));
					if (success == true) {
						unsigned int number = *((int*)pointer) / atoi(args[2]);
						memcpy(pointer, &number, sizeof(int));
					}
					else {
						printf("Failed to write to memory.\n");
					}
				}
			}
			else {
				printf("Please specify the first number.\n");
			}
		}
		else {
			printf("Please specify a location in memory.\n");
		}
	}
	if (commandProcessed == false && strcmp(args[0], "") != 0) {
		char s[256];
		getcwd(s, 256);
		bool exists = false;
		char* existingPath;
#ifdef __unix__
		strcat(s, "/");
		strcat(s, args[0]);

		char binPath[256] = "/bin/";
		strncat(binPath, args[0], 240);


		if (access(s, R_OK) == 0) {
			exists = true;
			existingPath = s;
			goto checkIfPathExists;
		}
		if (access(binPath, R_OK) == 0) {
			exists = true;
			existingPath = binPath;
			goto checkIfPathExists;
		}
		if (access(args[0], R_OK) == 0) {
			exists = true;
			existingPath = (char*)args[0];
			goto checkIfPathExists;
		}
#elif _WIN32
		strcat(s, "\\");

		strcat(s, args[0]);


		char system32Path[256] = "C:\\Windows\\System32\\";
		strncat(system32Path, args[0], 240);


		if (access(s, 0x06) == 0) {
			exists = true;
			existingPath = s;
			goto checkIfPathExists;
		}
		if (access(system32Path, 0x06) == 0) {
			exists = true;
			existingPath = system32Path;
			goto checkIfPathExists;
		}
		strcat(system32Path, ".exe");
		if (access(system32Path, 0x06) == 0) {
			exists = true;
			existingPath = system32Path;
			goto checkIfPathExists;
		}
		if (access(args[0], 0x06) == 0) {
			exists = true;
			existingPath = (char*)args[0];
			goto checkIfPathExists;
		}
#endif

	checkIfPathExists:

		if (exists) {
			commandProcessed = true;
			strcat(existingPath, " ");
#ifdef __unix__
			exec_prog(args);
#elif _WIN32
			_spawnl(0, existingPath, existingPath, args[1], args[2], args[3], args[4], NULL);
#endif
		}
	}

exitCommand:

	if (commandProcessed == false && strcmp(args[0], "") != 0) {
		printf("Command not found!\n");
	}
	return output;
}

extern const char* ProcessInput(char* input, char* args[], unsigned short maxSize) {
	input[strcspn(input, "\r\n")] = 0;

	char* newInput = calloc(maxSize, 1);
	TakeFormattedArguments(newInput, input);

	SplitString(args, newInput, ' ', maxSize);

	const char* output = ProcessCommand((const char**)args, maxSize);
	free(newInput);
	unsigned int i = 0;
	while (args[i] != NULL) {
		free(args[i]);
		i++;
	}

	return output;
}