#include "utility/headers.h"
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include "utility/split.h"
#include "utility/command.h"

static int luaI_ProcessInput(lua_State *L)
{
        const char *input = lua_tostring(L, 1);
        char **token = (char **)calloc(1, 1000);
        const char *output = ProcessInput(input, token, 1000);
        free(token);
        lua_pushstring(L, output);
        return 1;
}

static int luaI_FormatString(lua_State *L)
{
        const char *input = lua_tostring(L, 1);
        char *newInput = calloc(1000, 1);
        TakeFormattedArguments(newInput, input);
        lua_pushstring(L, newInput);
        return 1;
}

static int luaI_Exit(lua_State *L)
{
        exit(0);
}

int RunHaxxelLuaFile(const char *filePath)
{
        int status;
        lua_State *L = luaL_newstate();
        luaL_openlibs(L);
        lua_newtable(L);
        lua_pushcfunction(L, luaI_ProcessInput);
        lua_setfield(L, -2, "run");
        lua_pushcfunction(L, luaI_Exit);
        lua_setfield(L, -2, "exit");
        lua_pushcfunction(L, luaI_FormatString);
        lua_setfield(L, -2, "format");
        lua_setglobal(L, "haxxel");

        status = luaL_dofile(L, filePath);
        if (status)
        {
                printf("%s\n", lua_tostring(L, -1));
                return 1;
        }
        return 0;
}