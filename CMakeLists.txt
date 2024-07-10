# CMakeList.txt : CMake project for QShell, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS_DEBUG "-g")


project ("Haxxel")


# Add source to this project's executable.
add_executable (Haxxel "main.c" "libraries/dirent.h" "utility/split.h" "utility/command.c" "utility/split.c" "utility/headers.h" "utility/command.h" "utility/pointerLabels.c" "libraries/itoa.c")


# TODO: Add tests and install targets if needed.
