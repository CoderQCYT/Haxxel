#ifndef headers
#define headers
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <direct.h>
#include <io.h>
#include <wtypes.h>
#include <crtdbg.h>
#include <process.h>
#include "../libraries/dirent.h"

#define getcwd _getcwd
#define chdir _chdir
#define access _access
#elif __unix__
#include <dirent.h>
#include <curses.h>
#include <unistd.h>
#include <fcntl.h>
#include <ncurses.h>
#endif
#include <stdint.h>
#endif